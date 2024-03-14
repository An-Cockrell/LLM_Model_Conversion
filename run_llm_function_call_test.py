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

import xml.etree.ElementTree as ET
from functools import partial
from pydantic import BaseModel, Field, validator
from langchain_core.utils.function_calling import convert_to_openai_function


class BookRecommendation(BaseModel):
    """Provides book recommendations based on specified interest."""
    interest: str = Field(description="question of user interest about a book.")
    recommended_book: str = Field(description="answer to recommend a book")

    @validator("interest")
    def interests_must_not_be_empty(cls, field):
        if not field:
            raise ValueError("Interest cannot be empty.")
        return field

class Joke(BaseModel):
    """Get a joke that includes the setup and punchline"""
    setup: str = Field(description="question to set up a joke")
    punchline: str = Field(description="answer to resolve the joke")

    # You can add custom validation logic easily with Pydantic.
    @validator("setup")
    def question_ends_with_question_mark(cls, field):
        if field[-1] != "?":
            raise ValueError("Badly formed question!")
        return field
    
class SongRecommendation(BaseModel):
    """Provides song recommendations based on specified genre."""
    genre: str = Field(description="question to recommend a song.")
    song: str = Field(description="answer to recommend a song")

    @validator("genre")
    def genre_must_not_be_empty(cls, field):
        if not field:
            raise ValueError("genre cannot be empty.")
        return field
    
    
def generate_response(prompt, model, tokenizer):
    fn = """{"name": "function_name", "arguments": {"arg_1": "value_1", "arg_2": value_2, ...}}"""
    prompt = f"""<|im_start|>system
You are a helpful assistant with access to the following functions:


{convert_to_openai_function(Joke)}

{convert_to_openai_function(BookRecommendation)}

{convert_to_openai_function(SongRecommendation)}

To use these functions respond with:
<multiplefunctions>
    <functioncall> {fn} </functioncall>
    <functioncall> {fn} </functioncall>
    ...
</multiplefunctions>


Edge cases you must handle:
- If there are no functions that match the user request, you will respond politely that you cannot help.<|im_end|>
<|im_start|>user
{prompt}<|im_end|>
<|im_start|>assistant"""
    
    # input_tokens = tokenizer.apply_chat_template(chat, return_tensors="pt").to(model.device)
    input_tokens = tokenizer(prompt, return_tensors="pt").to(model.device)

    model.eval()
    with torch.no_grad(), torch.backends.cuda.sdp_kernel(enable_flash=False, enable_math=False, enable_mem_efficient=True):
        # querying model
        outputs = model.generate(input_tokens, pad_token_id=tokenizer.eos_token_id, max_new_tokens=2048, do_sample=True)
        # getting just the response tokens, no input tokens
        response = outputs[0][len(input_tokens[0]):]
        # convert back to plain text
        response_plain_text = tokenizer.decode(response, skip_special_tokens=True)

    return response_plain_text


def extract_function_calls(completion):
    completion = completion.strip()
    pattern = r"((.*?))"
    match = re.search(pattern, completion, re.DOTALL)
    if not match:
        return None

    multiplefn = match.group(1)
    root = ET.fromstring(multiplefn)
    functions = root.findall("functioncall")
    return [json.loads(fn.text) for fn in functions]

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

    generation_func = partial(generate_response, model=model, tokenizer=tokenizer)

    prompts = [
        "Tell me a joke",
        "Song for inspiration.",
        "Recommend me a book on Crime Thriller."
    ]

    for prompt in prompts:
        completion = generation_func(prompt)
        functions = extract_function_calls(completion)

        if functions:
            print(functions)
        else:
            print(completion.strip())
        print("="*100)


    # system_prompt = """
    #     You are a chat bot that works for the An-Cockrell research lab at the University of Vermont (UVM). You have the ability to have a conversation that could span a wide range of topics, from general interest questions to more technical discussions. Those technical discussion will  likely include programming concepts, and I might ask you to interpret or write snippets of code in Python or C++. The technical discussions might also include discussion of medical topics. For each type of chat, here's what I'd like you to do for me:

    #     // General Chat
    #     Feel free to engage in a broad discussion on a variety of subjects. Whether it's about books, science or just sharing fun facts, I'm here for it. Make sure responses are related to the newest user prompt and relevant previous chat history. If there are topics or questions you're not sure about, I appreciate pointers on how to find more information or suggestions for resources to explore further.

    #     // Python Code:
    #     When discussing Python, I might need help understanding specific libraries, writing functions to solve problems, or optimizing existing code. Please ensure the Python code is clear, concise, and well-commented so I can follow along easily.

    #     // C++ Code:
    #     For C++ discussions, I might ask about syntax, best practices, or how to implement certain algorithms. Like with Python, please ensure the code is understandable and includes comments where necessary to clarify complex parts.
        

    #     Remember, the goal is to maintain a friendly and informative conversation. I'm here to learn and have fun, so let's make this a great experience for both of us!
    #     """

    # # hacky, but a quick test to make sure that the model can accept a system input. mistral cannot, so it goes into the exception block
    # chat = [
    #     {"role":"system", "content":system_prompt},
    # ]   


    # user_prompt = ""
    # print("interacting with LLM, type `quit` to quit")
    # while user_prompt != "quit":
        prompt = input("user: ")
        if prompt == "quit":
            break
        chat.append({"role":"user", "content": prompt})
        input_tokens = tokenizer.apply_chat_template(chat, return_tensors="pt").to(model.device)

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
