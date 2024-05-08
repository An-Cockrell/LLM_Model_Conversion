import torch
import numpy as np
# from sklearn.metrics.pairwise import cosine_similarity
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
    save_file_name = "Test3"
    from pathlib import Path
    Path("Model_Implementations/" + save_file_name).mkdir(parents=True, exist_ok=True)

    # mixtral
    # MODELS DECLARATION
    # model_name = "../LLM_Models/Mistral/Mistral-7B-Instruct-v0.2"    # Path to chat model and 
    model_name = "../LLM_Models/Mixtral/Mixtral-8x7B-Instruct-v0.1"    # Path to chat model and 
    print("loading tokenizer")
    tokenizer = AutoTokenizer.from_pretrained(model_name, local_files_only=True)
    tokenizer.pad_token = tokenizer.eos_token
    # https://github.com/chujiezheng/chat_templates
    # if using mistral, which does not accept a system prompt, use this instead?
    print('###### Corrected Chat Template ######')
    chat_template = open('./chat_templates/mistral-instruct.jinja').read()
    chat_template = chat_template.replace('    ', '').replace('\n', '')
    tokenizer.chat_template = chat_template

    streamer = TextStreamer(tokenizer, skip_prompt=True)

    # embedding_model_name = "Embedding_Models/instructor-xl"         # path embedding model
    print("loading model")
    model = AutoModelForCausalLM.from_pretrained(model_name, local_files_only=True, device_map="auto", torch_dtype=torch.float16)
    print(model.hf_device_map)
    print("MODEL LOADED")

    developer_guide_text = open('./General_NetLogo_Framework/General_Netlogo_C++/Developer_guide.md').read()

    header_filepath_prefix = "Model_Implementations/" + save_file_name + "/include/"
    header_files = ["parameters.h", "agent.h", "patch.h", "turtle.h", "world.h"]
    simulation_framework_txt = ""
    for header in header_files:
        with open(header_filepath_prefix + header, "r") as f:
            simulation_framework_txt += f.read() + "\n\n\n"
    with open("Model_Implementations/" + save_file_name + "/src/world_custom_implementations.cpp", "r") as f:
        simulation_framework_txt += f.read() + "\n\n\n"


    def generate_system_prompt(dev_guide=developer_guide_text, sim_framework=simulation_framework_txt):
        system_prompt = f"""
            You are a chat bot assisting the An-Cockrell research lab at the University of Vermont (UVM). Your primary role is to guide the development team in integrating C++ code that fits seamlessly into a specific modeling simulation framework. You'll respond to inputs that will usually include natural language instructions, and may include a block of NetLogo code accompanied by a plain English description of its functionality. 
            Your output will be modifications to existing code and header files, or additions to add to the world_implementation.cpp file. Function declarations will have comments explaining the expected input and expected output of a function, as well as a short description of what it does.
            If you are implementing a new function, be sure to include a function declaration with it to go in a header file.

            When modifications to existing files are suggested, clearly indicate the specific file and location within that file where the changes are to be considered. 

            Below is the developer's guide and the current state of the simulation framework. Use these as references to ensure consistency and strategic integration with the existing codebase.

            Developers Guide:
            {dev_guide}
            /end Developers Guide

            Simulation Framework:
            {sim_framework}
            /end Simulation Framework


        """

        return system_prompt

    system_prompt = generate_system_prompt()


    chat = [
        {"role":"system", "content":system_prompt},
    ]   

    previous_chat = "./LLM_output_files/" + save_file_name + ".json"
    # previous_chat = None
    if previous_chat is not None:
        try:
            with open(previous_chat, "r") as f:
                model_outputs = json.load(f)
            for key in model_outputs.keys():
                chat.append({"role":"user", "content": key})
                chat.append({"role":"assisstant", "content": model_outputs[key]})
        except FileNotFoundError:
            model_outputs= {}
    else:
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

            # updating system prompt in the loop to account for file updates
            updated_simulation_framework_txt = ""
            headers_update = os.listdir(header_filepath_prefix)
            for header in header_files:
                with open(header_filepath_prefix + header, "r") as f:
                    updated_simulation_framework_txt += f.read() + "\n\n\n"                
            for header in (headers_update):
                if header in header_files:
                    continue    # skipping ones we have already added
                with open(header_filepath_prefix + header, "r") as f:
                    updated_simulation_framework_txt += f.read() + "\n\n\n"    
            with open("Model_Implementations/" + save_file_name + "/src/world_custom_implementations.cpp", "r") as f:
                updated_simulation_framework_txt += f.read() + "\n\n\n"
            system_prompt = generate_system_prompt(sim_framework=updated_simulation_framework_txt)

            # gets the last 10 entires (5 prompts, 5 responses) after the input prompt
            last_5_responses = chat[1:][-10:]
            chat = [{"role":"system", "content":system_prompt}]
            chat = chat+last_5_responses

            print("\nModel: ")
            chat.append({"role":"user", "content": prompt})
            input_tokens = tokenizer.apply_chat_template(chat, return_tensors="pt")
            input_tokens = input_tokens.to(model.device)

            model.eval()
            with torch.no_grad(), torch.backends.cuda.sdp_kernel(enable_flash=False, enable_math=False, enable_mem_efficient=True):
                # querying model
                outputs = model.generate(input_tokens, pad_token_id=tokenizer.eos_token_id, max_new_tokens=8192, do_sample=True, streamer=streamer)
                # getting just the response tokens, no input tokens
                response = outputs[0][len(input_tokens[0]):]
                # convert back to plain text
                response_plain_text = tokenizer.decode(response, skip_special_tokens=True)

                chat.append({"role":"assistant", "content": response_plain_text})
                # print("Model: \n{}".format(response_plain_text))

                model_outputs[prompt] = response_plain_text
                prompt = ""
                with open("./LLM_output_files/" + save_file_name + ".json", "w") as f:
                    json.dump(model_outputs, f, indent=6)




