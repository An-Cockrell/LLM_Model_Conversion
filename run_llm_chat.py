import torch
import numpy as np
from sklearn.metrics.pairwise import cosine_similarity
import time
import json

from transformers import AutoTokenizer, AutoModelForCausalLM
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


    embedding_model_name = "Embedding_Models/instructor-xl"         # path embedding model
    print("loading model")
    model = AutoModelForCausalLM.from_pretrained(model_name, local_files_only=True, device_map="auto")
    print("MODEL LOADED")




    system_prompt = """
        You are a chat bot that works for the An-Cockrell research lab at the University of Vermont (UVM). You have the ability to have a conversation that could span a wide range of topics, from general interest questions to more technical discussions. Those technical discussion will  likely include programming concepts, and I might ask you to interpret or write snippets of code in Python or C++. The technical discussions might also include discussion of medical topics. For each type of chat, here's what I'd like you to do for me:

        // General Chat
        Feel free to engage in a broad discussion on a variety of subjects. Whether it's about books, science or just sharing fun facts, I'm here for it. Make sure responses are related to the newest user prompt and relevant previous chat history. If there are topics or questions you're not sure about, I appreciate pointers on how to find more information or suggestions for resources to explore further.

        // Python Code:
        When discussing Python, I might need help understanding specific libraries, writing functions to solve problems, or optimizing existing code. Please ensure the Python code is clear, concise, and well-commented so I can follow along easily.

        // C++ Code:
        For C++ discussions, I might ask about syntax, best practices, or how to implement certain algorithms. Like with Python, please ensure the code is understandable and includes comments where necessary to clarify complex parts.
        

        Remember, the goal is to maintain a friendly and informative conversation. I'm here to learn and have fun, so let's make this a great experience for both of us!
        """

    # hacky, but a quick test to make sure that the model can accept a system input. mistral cannot, so it goes into the exception block
    try:
        chat = [
            {"role":"system", "content":system_prompt},
        ]   
        input_tokens = tokenizer.apply_chat_template(chat + {"role":"user", "content": "test"}, return_tensors="pt")
        print("ABLE TO USE IT LIKE THIS")
    except:
        chat = [
            {"role":"user", "content":system_prompt},
            {"role":"assistant", "content":"That sounds great, lets chat!"}
        ]

    user_prompt = ""
    print("interacting with LLM, type `quit` to quit")
    while user_prompt != "quit":
        prompt = input("user: ")
        if prompt == "quit":
            break
        chat.append({"role":"user", "content": prompt})
        input_tokens = tokenizer.apply_chat_template(chat, return_tensors="pt")
        input_tokens = input_tokens.to(model.device)

        model.eval()
        with torch.no_grad(), torch.backends.cuda.sdp_kernel(enable_flash=False, enable_math=False, enable_mem_efficient=True):
            # querying model
            outputs = model.generate(input_tokens, pad_token_id=tokenizer.eos_token_id, max_new_tokens=2048, do_sample=True)
            # getting just the response tokens, no input tokens
            response = outputs[0][len(input_tokens[0]):]
            # convert back to plain text
            response_plain_text = tokenizer.decode(response, skip_special_tokens=True)

            chat.append({"role":"assistant", "content": response_plain_text})
            print("Model: \n{}".format(response_plain_text))

        print(chat)
