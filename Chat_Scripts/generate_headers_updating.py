import torch
import numpy as np
from sklearn.metrics.pairwise import cosine_similarity
import time
import json

import os
os.environ['PYTORCH_CUDA_ALLOC_CONF'] = 'expandable_segments:True'
import re

from Chat_Scripts.llm_utils import *

from transformers import AutoTokenizer, AutoModelForCausalLM
from InstructorEmbedding import INSTRUCTOR
# https://github.com/xlang-ai/instructor-embedding


def generate_headers_updating(
        model_name: str,
        save_name: str="test_header_updating",
        PRINTING: bool=True
        ) -> None:


    embedding_model_name: str="Embedding_Models/instructor-xl"
    # ------------------------------------------------------- #
    # LOADING CONTEXT/INPUT TEXT

    # notes/explanation of the overall structure of a working version of the simulation, and therefore desired structure
    notes_files = ["B_Cell_Model/B Cell Abm C++ Notes.md"]
    notes_text = ""
    for note_file in notes_files:
        with open(note_file, "r") as f:
            notes_text += f.read()
    if PRINTING: print(notes_text)

    # load netlogo to translate into headers
    netlogo_files_to_interpret = ["Context_Data_Files/netlogo_description_text.json"]
    for file_to_chunk in netlogo_files_to_interpret:
        with open (file_to_chunk, "r") as f:
            blocks_to_label = json.load(f)

    # load cpp headers to build off of/use as reference, store as a list
    header_context_data = ["Context_Data_Files/general_cpp_headers.json"]
    cpp_headers = []    # list to store headers
    general_parsed_cpp_headers = ""
    for file_to_chunk in header_context_data:
        with open (file_to_chunk, "r") as f:
            codeblocks = json.load(f)
            for key, value in codeblocks.items():
                general_parsed_cpp_headers += value["code_block"] + "\n\n"
                cpp_headers.append(value["code_block"])



    # ------------------------------------------------------- #
    # LOAD MODELS

    # LOAD EMBEDDER
    if PRINTING: print_cuda_usage("Before Loading Embedding Model")
    embedding_model = INSTRUCTOR(embedding_model_name)
    if PRINTING: print_cuda_usage("After Loading Embedding Model")
    # and embed the headers
    headers_for_embeddings = []
    for header in cpp_headers:
        headers_for_embeddings.append(["Represent the header file:", header])
    cpp_embeddings = embedding_model.encode(headers_for_embeddings)
    if PRINTING: print("starting embeddings shape")
    if PRINTING: print(cpp_embeddings.shape)

    # LOAD CHAT MODEL
    if PRINTING: print_cuda_usage("Before Loading chat Model Tokenizer")
    start = time.time()
    tokenizer = AutoTokenizer.from_pretrained(model_name, local_files_only=True)
    tokenizer.pad_token = tokenizer.eos_token
    if PRINTING: print_cuda_usage("After Loading chat Model Tokenizer")
    if PRINTING: print()
    if PRINTING: print("Loading Chat Model")
    model = AutoModelForCausalLM.from_pretrained(model_name, local_files_only=True, device_map="auto")
    if PRINTING:  print_cuda_usage("After Loading chat Model")
    if PRINTING:  print("Loading times for Chat model and tokenizer: {:.2f}".format(time.time() - start))




    # ------------------------------------------------------- #
    # DEFINING PARTS OF PROMT

    # can just explicitly put base prompt here
    system_prompt = "You are a helpful assistant who is an expert programmer. You will receive a command. Complete the prompt. Make any code written enclosed in triple backticks, like this: ```// insert your code here```. Write out code files and blocks completely and do not have comment blocks longer than 3 lines in a row."

    prompt = "You will get C++ header files from a general simulation framework that I created. Then I am going to give you an explanation of a specific implementation of the simulation that builds on the C++ header framework. You will then receive a block of netlogo code. Your job is to analyze the netlogo and add C++ code for a function signature or variable declaration to an existing header file or create a new header file for a new class so that the netlogo code is represented in the simulation framework. Please explain your choice of which file to add the code to."

    assistant_response = "Sure, I'd be happy to help you create header files for your simulation! Could you please provide me with the simulation framework and the block of netlogo you want me to include in the simulation?"


    # ------------------------------------------------------- #
    # START THE INDIVIDUAL PROMPT SPECIFIC PARTS AND RESPONSE GENERATION
    LLM_OUTPUT = {}
    all_blocks_start_time = time.time() # timer

    # Going through all blocks of netlogo from which to generate headers
    for block_id in blocks_to_label.keys():
        produced_code = ""
        num_tries = 0
        max_tries = 10
        #keep trying to generate a block of code if we are not able to extract one from the generated response
        while produced_code == "" and num_tries < max_tries:
            print("--------------------------------------------------------------------------------------------------")
            print("STARTING ON BLOCK ID NUMBER " + str(block_id))
            # do it inside the loop because cpp_headers is gonna change as we add more
            cpp_headers_combined = ""
            for header in cpp_headers:
                cpp_headers_combined += header + "\n\n"

            # adding base C++ as simulation framework
            block_message_content = "Here are the headers from the the simulation framework:\n#HEADER CODE AS CONTEXT\n" + cpp_headers_combined

            # adding notes from .md file
            block_message_content += "\nHere is the explanation of the specific implementation to create, based on the framework provided:\n#IMPLEMENTATION EXPLANATION\n" + str(notes_text)

            # getting one block of netlogo code from context dict
            block = str(blocks_to_label[block_id]["code"])
            block_message_content += "\nHere is the block of netlogo code:\n#NETLOGO CODE\n" + str(block)

            # 
            block_message_content += "\n\nNow using the framework code for reference, add the block of netlogo code to a header file."

            # final_message is:
            #   Base CPP Simulation code + CPP framework code generated by model/Code updated by model
            #   Handwritten notes about simulation 
            #   Netlogo block to translate
            #   Now ask to generate response




            # ------------------------------------------------------- #
            # PUTTING CHAT INTO TEMPLATE AND TOKENIZING
            start = time.time()  # timer for tokenization  
            chat = [
                {"role":"system", "content":system_prompt},
                {"role":"user", "content":prompt},
                {"role":"assistant", "content":assistant_response},
                {"role":"user", "content":block_message_content}
            ]
            input_tokens = tokenizer.apply_chat_template(chat, return_tensors="pt")
            input_tokens = input_tokens.to(model.device)
            input_text = tokenizer.decode(input_tokens[0])


            # # GETTING SIZES OF DIFFERENT PARTS FOR MEMORY TESTING
            # system_prompt_tokens = tokenizer.encode(system_prompt, return_tensors="pt")
            # prompt_tokens = tokenizer.encode(prompt, return_tensors="pt")
            # block_tokens = tokenizer.encode(block_message_content, return_tensors="pt")

            # print("System input tokens: " + str(system_prompt_tokens.shape[1]))
            # print("User input tokens: " + str(prompt_tokens.shape[1]))
            # print("Code Block tokens: " + str(block_tokens.shape[1]))
            # print("prompt and context tokenization time: {:.2f}".format(time.time() - start))
            # print_cuda_usage("After tokenizing Prompt")




            # ------------------------------------------------------- #
            # GENERATING MODEL RESPONSE
            print("getting model response")
            start = time.time()
            # no gradients saves memory, also need to enable mem_efficient implementation for large models
            model.eval()
            with torch.no_grad(), torch.backends.cuda.sdp_kernel(enable_flash=False, enable_math=False, enable_mem_efficient=True):
                # querying model
                outputs = model.generate(input_tokens, pad_token_id=tokenizer.eos_token_id, max_new_tokens=2048, do_sample=True)
                # getting just the response tokens, no input tokens
                response = outputs[0][len(input_tokens[0]):]
                # convert back to plain text
                response_plain_text = tokenizer.decode(response, skip_special_tokens=True)

                # # DISPLAY WHOLE PROMPT AND RESPONSE
                # print(input_text)
                # print(response_plain_text)




                # ------------------------------------------------------- #
                # CODE EXTRACTION FROM RESPONSE
                # REGEX help from chat GPT to extract the code enclosed in triple backticks and ends with #endif. Should get c++ header files
                regex = r'```(?:(?!```).)*?#endif\n```'
                # ```: Matches exactly three backticks, marking the start of the target string.
                # (?:(?!{3}).)?: This is a non-capturing group (?: ... )? that matches any character (.)*?) any number of times as few as possible, with a negative lookahead (?! ... )inside it. The negative lookahead ensures that the sequence of three backticks does not appear in the matched section. This way, we start from the last set of triple backticks before the#endif`.
                #endif\n: The specific string that indicates the end of the relevant code block, followed by a newline character (\n).
                # ```: Matches exactly three backticks again, indicating the end of the code block.


                produced_code = re.search(regex, response_plain_text, re.DOTALL)
                if produced_code:   # if there is a match
                    produced_code = produced_code.group()
                else:
                    produced_code = ""
                LLM_OUTPUT[block_id] = {}
                LLM_OUTPUT[block_id]["netlogo_input"] = block
                LLM_OUTPUT[block_id]["header_response"] = response_plain_text
                LLM_OUTPUT[block_id]["code_block"] = produced_code
                # print_cuda_usage("After Translating outputs")

                if produced_code == "":
                    print("\n\nno code produced for this input. Netlogo input was {}".format(block))
                    print("Inference Time for model: " + str(time.time() - start))
                    num_tries += 1
                    if num_tries < max_tries: print("TRYING AGAIN WITH SAME CODE BLOCK\n\n")



        if num_tries >= max_tries:  
            LLM_OUTPUT[block_id]["netlogo_input"] = block
            LLM_OUTPUT[block_id]["header_response"] = response_plain_text
            LLM_OUTPUT[block_id]["code_block"] = ""
            print("TRIED ENOUGH WITH THIS CODEBLOCK, ASSUMING BLANK CODE RESPONSE, MOVING ON TO THE NEXT")
            continue



        # ------------------------------------------------------- #
        # UPDATING CODEBASE
        # If we make it here, that means there is produced code!
        # We are updating the codebase as it is generated

        # get embedding (vector representation of a string)
        response_embedding = embedding_model.encode([["Represent the header file:", produced_code]])
        # use cosine similarity to find other similar strings
        relevancy_to_all_headers = cosine_similarity(response_embedding, cpp_embeddings).flatten()
        if PRINTING:  print(relevancy_to_all_headers)         # printing the similarity sores
        if PRINTING:  print(relevancy_to_all_headers.shape)   # number of C++ codeblocks
        # do we have an entry that is similar to our response?
        most_similar_header_score = relevancy_to_all_headers[np.argmax(relevancy_to_all_headers)]
        if most_similar_header_score < 0.985:
            # if no entries are similar, add new one
            if PRINTING:  print(cpp_embeddings.shape)
            cpp_embeddings = np.concatenate((cpp_embeddings,response_embedding), axis=0)
            if PRINTING:  print(len(cpp_headers))
            cpp_headers.append(produced_code)
            if PRINTING:  print("Inference Time for model appending code: " + str(time.time() - start))

        else:
            # otherwise update the most relevant block using a language model to combine code blocks
            # ------------------------------------------------------- #
            # RAG PROMPT GENERATION TIME
            # new base prompt
            combine_code_prompt = "Here are blocks of code that are related. Please combine them into a unified header file. Respond only with the requested code. \nHere are the blocks I would like you to combine:\n"
            combine_code_prompt += "\n#CODE BLOCK:\n" + produced_code + "\n\n"
            # getting other relevant codeblocks
            relevant_indices = np.where(relevancy_to_all_headers >= 0.985)[0] 
            # turn into a list of only relevant code
            relevant_combine_code = [cpp_headers[i] for i in relevant_indices]
            # add each to the prompt
            for code in relevant_combine_code:
                combine_code_prompt += "\n#CODE BLOCK:\n" + code + "\n\n"



            # ------------------------------------------------------- #
            # GENERATE RESPONSE pt2
            # put chat into template
            chat_combine = [
                {"role":"system", "content":system_prompt},
                {"role":"user", "content":combine_code_prompt},
            ]
            
            input_tokens_combine = tokenizer.apply_chat_template(chat_combine, return_tensors="pt")
            input_tokens_combine = input_tokens_combine.to(model.device)
            input_text_combine = tokenizer.decode(input_tokens_combine[0])

            outputs = model.generate(input_tokens, pad_token_id=tokenizer.eos_token_id, max_new_tokens=2048, do_sample=True)
            response_combine = outputs[0][len(input_tokens[0]):]
            updated_code_entry = tokenizer.decode(response, skip_special_tokens=True)



            # ------------------------------------------------------- #
            # CODE REPLACEMENT PART
            # find relevant codeblock to replace, i.e. closest match to generated code
            response_embedding = embedding_model.encode([["Represent the header file:", updated_code_entry]])
            cpp_embeddings[np.argmax(relevancy_to_all_headers), :] = response_embedding
            cpp_headers[np.argmax(relevancy_to_all_headers)] = updated_code_entry

            # add RAG updated code to output for analysis
            LLM_OUTPUT[block_id]["RAG_code"] = updated_code_entry
            if PRINTING:  print("Inference Time for model with updating codeblock: " + str(time.time() - start))

            # now do next netlogo codeblock until the end
        

        # saving final response dictionary
        with open(save_name + str(torch.cuda.device_count()) + ".json", "w") as f:
            json.dump(LLM_OUTPUT, f, indent=6)

    print("Inference Time for full codeblock is: " + str(time.time() - all_blocks_start_time))
