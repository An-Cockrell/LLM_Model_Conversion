import torch
import numpy as np
from sklearn.metrics.pairwise import cosine_similarity
import time
import json

from transformers import AutoTokenizer, AutoModelForCausalLM, TextStreamer

import torch
import numpy as np
from sklearn.metrics.pairwise import cosine_similarity
import time
import json

from transformers import AutoTokenizer, AutoModelForCausalLM, TextStreamer
from InstructorEmbedding import INSTRUCTOR

import os
os.environ['PYTORCH_CUDA_ALLOC_CONF'] = 'expandable_segments:True'
import re

if __name__=="__main__":
    # mixtral
    # MODELS DECLARATION
    model_name = "../LLM_Models/Mistral/Mistral-7B-Instruct-v0.2"    # Path to chat model and 
    print("loading tokenizer")
    tokenizer = AutoTokenizer.from_pretrained(model_name, local_files_only=True)
    tokenizer.pad_token = tokenizer.eos_token
    # https://github.com/chujiezheng/chat_templates
    # if using mistral, which does not accept a system prompt, use this instead?
    print('###### Corrected Chat Template ######')
    chat_template = open('./chat_templates/mistral-instruct.jinja').read()
    chat_template = chat_template.replace('    ', '').replace('\n', '')
    tokenizer.chat_template = chat_template

    streamer = TextStreamer(tokenizer)

    # embedding_model_name = "Embedding_Models/instructor-xl"         # path embedding model
    print("loading model")
    model = AutoModelForCausalLM.from_pretrained(model_name, local_files_only=True, device_map="auto", torch_dtype=torch.float16)
    print(model.hf_device_map)
    print("MODEL LOADED")

    developer_guide_text = open('./General_NetLogo_Framework/General_Netlogo_C++/Developer_guide.md').read()

    simulation_framework_txt = open('./General_NetLogo_Framework/General_Netlogo_C++/headers_commented.md').read()




    
    system_prompt = f"""
        You are a chat bot that works for the An-Cockrell research lab at the University of Vermont (UVM). Your main duty is to generate code that can be added to a specific C++ modeling simulation framework, but you can also respond to general coding prompts.

        Here are header files specific simulation framework you will be building on.
        Simulation Framework:
        {simulation_framework_txt}
        /end Simulation Framework

        Here is the developer's guide for building on top of the simulation framework.
        Developers Guide:
        {developer_guide_text}
        /end Deveopers Guide
        

        The user will give you a block of netlogo code with a plain english description of its overall function. You will then generate a C++ header file or declarations to add to header a file to outline the functionality that the given code implements. If you are inserting code into the middle of a block, signify this by telling the user what codeblock is being changes and wrapping the start of your generation with three lines with a single dot on them, then the three lines of original code where you intend to add your new code and appending in the opposite order at the end of your generation.If you would repeat essentially the same code many times, do one thorough example, and state that this process can be repeated for the other examples. You will write the class or function declaration, then add comments describing what needs to be done to implement the function.
        """

    chat = [
        {"role":"system", "content":system_prompt},
    ]   

    model_outputs = {}
    user_prompt = ""
    prompt = ""
    print("interacting with LLM, type `quit` to quit")
    while user_prompt != "quit":
        user_prompt = input("user: ")
        if user_prompt == "quit":
            break
        if user_prompt != "generate":
            prompt += user_prompt + "\n"
        if user_prompt == "generate":
            print("\nModel: ")
            chat.append({"role":"user", "content": prompt})
            input_tokens = tokenizer.apply_chat_template(chat, return_tensors="pt")
            input_tokens = input_tokens.to(model.device)

            model.eval()
            with torch.no_grad(), torch.backends.cuda.sdp_kernel(enable_flash=False, enable_math=False, enable_mem_efficient=True):
                # querying model
                outputs = model.generate(input_tokens, pad_token_id=tokenizer.eos_token_id, max_new_tokens=2048, do_sample=True, streamer=streamer)
                # getting just the response tokens, no input tokens
                response = outputs[0][len(input_tokens[0]):]
                # convert back to plain text
                response_plain_text = tokenizer.decode(response, skip_special_tokens=True)

                chat.append({"role":"assistant", "content": response_plain_text})
                # print("Model: \n{}".format(response_plain_text))

                model_outputs[prompt] = response_plain_text
                prompt = ""
                with open("./LLM_output_files/iirabm_header_responses.json", "w") as f:
                    json.dump(model_outputs, f, indent=6)


