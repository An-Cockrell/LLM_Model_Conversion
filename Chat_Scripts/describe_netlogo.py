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


def describe_netlogo_as_header(
        model_name: str,
        save_name: str = "test_header_instructions",
        PRINTING: bool = True
    ) -> None:

    # ------------------------------------------------------- #
    # LOADING CONTEXT/INPUT TEXT

    # Load the file containing blocks of NetLogo code to label
    files_to_chunk = "Context_Data_Files/netlogo_parsed_named_clean.json"
    with open(files_to_chunk, "r") as f:
        blocks_to_label = json.load(f)

    # Load and combine CPP code blocks for context
    cpp_files = [
        "B_Cell_Model/b_cell_functions.json",
        "B_Cell_Model/header_files.json"
    ]
    cpp_code = ""
    for cpp_file in cpp_files:
        with open(cpp_file, "r") as f:
            cpp_json = json.load(f)
            for entry in cpp_json.keys():
                code = cpp_json[entry][sorted(list(cpp_json[entry].keys()))[0]]
                cpp_code += "\n\n\n" + code + "\n\n\n"

    # Location of embeddings and plain text for relevant context
    context_file = "Context_Data_Files/netlogo_dictionary_embeddings.npy"
    context = np.load(context_file)
    context_plain_text_file = "Context_Data_Files/netlogo_dictionary_plain_text.json"
    with open(context_plain_text_file, "r") as f:
        context_plain_text_dict = json.load(f)

    # ------------------------------------------------------- #
    # LOAD MODELS

    # Loading chat model and tokenizer
    if PRINTING: print_cuda_usage("Before Loading chat Model")
    start = time.time()
    if PRINTING: print("loading chat model tokenizer")
    tokenizer = AutoTokenizer.from_pretrained(model_name, trust_remote_code=True)
    tokenizer.pad_token = tokenizer.eos_token
    if PRINTING: print("Tokenizer loaded\n\nLoading Chat Model")
    model = AutoModelForCausalLM.from_pretrained(model_name, trust_remote_code=True, device_map="auto")
    if PRINTING: print("Loading times for Chat model and tokenizer: {:.2f}".format(time.time() - start))
    if PRINTING: print_cuda_usage("After Loading chat Model")

    # ------------------------------------------------------- #
    # DEFINING PARTS OF PROMPT AND RESPONSE GENERATION

    # Define the base system and user prompts
    system_prompt = "You are a helpful assistant and you will get some reference examples and a user prompt. Complete the prompt:"
    priming_prompt = "Create a codebase for a complex biomedical model in C++"

    instruction_prompt = "You going to parse the code base written in C++ and compare it to other blocks of code written in Netlogo. You will be given a single block of netlogo code that is from a netlogo implementation of the same simulation. You task is to describe in specific detail how a C++ header file could be made to define the functionality of the codeblock.There will likely be C++ code in the codebase that is directly equivalent. Only include explanations in english and not in code."



    # Generating responses for each block of NetLogo code
    LLM_OUTPUT = {}

    # print("creating inputs")
    all_blocks_start_time = time.time()
    for block_id in blocks_to_label.keys():
        block = str(blocks_to_label[block_id])    
        block_message = {"role":"user", "content":"\nHere is the block of netlogo:\n\n#NETLOGO CODE\n" + block}

        try:
            # Initial conversation setup
            chat = [
                {"role": "system", "content": system_prompt},
                {"role": "user", "content": priming_prompt},
                {"role": "assistant", "content": "\nHere is the cpp codebase.\n" + cpp_code},
                {"role": "user", "content": instruction_prompt},
                {"role": "assistant", "content": "I would be happy to help! Please provide the block of netlogo and I will explain its function step by step."}
            ]
            input_tokens = tokenizer.apply_chat_template(chat + [block_message], return_tensors="pt")
            input_tokens = input_tokens.to(model.device)
            input_text = tokenizer.decode(input_tokens[0])
        except: #If there is an error here, likely because mistral doesnt accept a system prompt, need to manually add it instead.
            start = time.time()  # timer for tokenization  
            chat = [
                {"role":"user", "content":system_prompt + "\n\n\n" + priming_prompt},
                {"role":"assistant", "content": "\nHere is the cpp codebase.\n" + cpp_code},
                {"role": "user", "content": instruction_prompt},
                {"role": "assistant", "content": "I would be happy to help! Please provide the block of netlogo and I will explain its function step by step."}
            ]
            input_tokens = tokenizer.apply_chat_template(chat + [block_message], return_tensors="pt")
            input_tokens = input_tokens.to(model.device)
            input_text = tokenizer.decode(input_tokens[0])

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
            LLM_OUTPUT[block_id] = {}
            LLM_OUTPUT[block_id]["netlogo_input"] = block
            LLM_OUTPUT[block_id]["header_instructions"] = response_plain_text

        # saving final response dictionary
        with open(save_name + str(torch.cuda.device_count()) + ".json", "w") as f:
            json.dump(LLM_OUTPUT, f, indent=6)

    print("Inference Time for full codeblock is: " + str(time.time() - all_blocks_start_time))
