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
    # model_name = "../LLM_Models/Mixtral/Mixtral-8x7B-Instruct-v0.1"    # Path to chat model and 
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

    simulation_framework_txt = open('./General_NetLogo_Framework/General_Netlogo_C++/headers_generated.md.c').read()

    def generate_system_prompt(dev_guide=developer_guide_text, sim_framework=simulation_framework_txt):
        system_prompt = f"""
            You are a chat bot assisting the An-Cockrell research lab at the University of Vermont (UVM). Your primary role is to generate C++ code that integrates seamlessly into a specific modeling simulation framework. You'll respond to inputs that might include natural language instruction, and will usually include blocks of NetLogo code accompanied by a plain English description of their functionality. Your outputs will be modifications to existing code within the framework you will be given.

            When modifications to existing files are needed, clearly indicate the specific file and location within that file where the new code should be inserted. Use the following format to highlight where to insert the new code:

            ```
            {{three lines of existing code preceding the insertion point}}

            {{Here is where I want to insert the new code}}

            {{three lines of existing code following the insertion point}}
            ```

            Please specify the filename at the start of your response when modifying existing code. If the same modification pattern applies to multiple locations, illustrate it once in detail, then note specifically what other instances that the process should be applied to.

            Below is the current state of the simulation framework and the developer's guide. Use these as references to ensure consistency and integration with the existing codebase.

            Simulation Framework:
            {sim_framework}
            /end Simulation Framework

            Developers Guide:
            {dev_guide}
            /end Developers Guide

            Your code should adhere to the principles outlined in the developer's guide, maintaining the framework's conventions and overall design philosophy. Provide detailed comments on the implementation strategy for each piece of generated code.
        """
        return system_prompt

    system_prompt = generate_system_prompt()


    chat = [
        {"role":"system", "content":system_prompt},
    ]   

    # previous_chat = "./LLM_output_files/iirabm_header_responses1.json"
    previous_chat = None
    if previous_chat is not None:
        with open(previous_chat, "r") as f:
            model_outputs = json.load(f)
        for key in model_outputs.keys():
            chat.append({"role":"user", "content": key})
            chat.append({"role":"assisstant", "content": model_outputs[key]})
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
            print("\nModel: ")
            chat.append({"role":"user", "content": prompt})
            input_tokens = tokenizer.apply_chat_template(chat, return_tensors="pt")
            input_tokens = input_tokens.to(model.device)

            model.eval()
            with torch.no_grad(), torch.backends.cuda.sdp_kernel(enable_flash=False, enable_math=False, enable_mem_efficient=True):
                # querying model
                outputs = model.generate(input_tokens, pad_token_id=tokenizer.eos_token_id, max_new_tokens=4096, do_sample=True, streamer=streamer)
                # getting just the response tokens, no input tokens
                response = outputs[0][len(input_tokens[0]):]
                # convert back to plain text
                response_plain_text = tokenizer.decode(response, skip_special_tokens=True)

                chat.append({"role":"assistant", "content": response_plain_text})
                # print("Model: \n{}".format(response_plain_text))

                model_outputs[prompt] = response_plain_text
                prompt = ""
                with open("./LLM_output_files/iirabm_header_responses2.json", "w") as f:
                    json.dump(model_outputs, f, indent=6)

            updated_simulation_framework_txt = open('./General_NetLogo_Framework/General_Netlogo_C++/headers_generated.md.c').read()

            system_prompt = generate_system_prompt(sim_framework=updated_simulation_framework_txt)

            # gets the last 10 entires (5 prompts, 5 responses) after the input prompt
            last_5_responses = chat[1:][-10:]
            chat = [{"role":"system", "content":system_prompt}]
            chat = chat+last_5_responses



