# This is an example of LLM Function calling that I took from this video and this repository
# not much of this code is mine but this was for learning
# https://www.youtube.com/watch?v=MQmfSBdIfno
# https://github.com/AIAnytime/Function-Calling-Mistral-7B

import torch
import numpy as np
from sklearn.metrics.pairwise import cosine_similarity
import time
import json

import random

from transformers import AutoTokenizer, AutoModelForCausalLM
from InstructorEmbedding import INSTRUCTOR

import os
os.environ['PYTORCH_CUDA_ALLOC_CONF'] = 'expandable_segments:True'
import re

import gc, inspect, json, re
from typing import get_type_hints

import xml.etree.ElementTree as ET
from functools import partial
from langchain.pydantic_v1 import BaseModel, Field, validator
from langchain.chains.openai_functions import convert_to_openai_function

counter = 0
class test_functions():
    def BookRecommendation(interest: str) -> str:
        """Provides book recommendations based on specified interest for a test case."""
        global counter
        counter += 1
        print("CALLING BOOK RECOMMENDATION")

        res = "Based on your interest in: {}".format(interest)
        books = ['Stars Beyond the Void', 'Whispers in the Shadows', 'The Crown of Forgotten Realms']
        res += "\nI think you would like the book {}".format(random.choice(books))
        return res + "\ncounter {}".format(counter)

    def Joke(setup: str, punchline: str) -> str:
        """Formats a joke that includes the setup and punchline for a test case"""
        global counter
        counter += 1
        print("CALLING JOKE")
        res = "Hey do you wanna hear a joke? {}".format(setup)
        res += punchline.upper() + "\nHAR HAR HAR!!!"
        # print("SETUP:\n{}".format(setup))
        # print("PUNCHLINE:\n{}".format(punchline))
        return res + "\ncounter {}".format(counter)

    def SongRecommendation(genre: str) -> str:
        """Provides song recommendations based on specified genre for a test case."""
        global counter
        counter += 1
        print("CALLING SONG RECOMMENDATION")
        res = "Based on your interest in: {}".format(genre)
        songs = ['my horse is getting old', 'I sure love my farm', 'cold beer and blue jeans are all I need']
        res += "\nI think you would like the song {}".format(random.choice(songs))
        return res + "\ncounter {}".format(counter)

    def LoadFile(filename: str) -> str:
        """pretents to load a csv file into a numpy array for a test case and returns a success message."""
        global counter
        counter += 1
        print("CALLING LOAD FILE")

        res = "LOADING FILE {}".format(filename)
        return res + "\ncounter {}".format(counter)
        
# class BookRecommendation(BaseModel):
#     """Provides book recommendations based on specified interest."""
#     interest: str = Field(description="question of user interest about a book.")
#     recommended_book: str = Field(description="answer to recommend a book")

#     @validator("interest")
#     def interests_must_not_be_empty(cls, field):
#         if not field:
#             raise ValueError("Interest cannot be empty.")
#         return field

# class Joke(BaseModel):
#     """Get a joke that includes the setup and punchline"""
#     setup: str = Field(description="question to set up a joke")
#     punchline: str = Field(description="answer to resolve the joke")

#     # You can add custom validation logic easily with Pydantic.
#     @validator("setup")
#     def question_ends_with_question_mark(cls, field):
#         if field[-1] != "?":
#             raise ValueError("Badly formed question!")
#         return field
    

# class SongRecommendation(BaseModel):
#     """Provides song recommendations based on specified genre."""
#     genre: str = Field(description="question to recommend a song.")
#     song: str = Field(description="answer to recommend a song")

#     @validator("genre")
#     def genre_must_not_be_empty(cls, field):
#         if not field:
#             raise ValueError("genre cannot be empty.")
#         return field
    
#     return "asdf"
        
# class LoadFile(BaseModel):
#     """Loads a csv file into a numpy array."""
#     filename: list[str] = Field(description="path of file to load")

#     @validator("filename")
#     def filename_must_not_be_empty(cls, field):
#         if not field:
#             raise ValueError("filename cannot be empty.")
#         return field
    

def extract_function_calls(completion):
    completion = completion.strip()
    pattern = r"(<multiplefunctions>(.*?)</multiplefunctions>)"
    match = re.search(pattern, completion, re.DOTALL)
    if not match:
        return None

    multiplefn = match.group(1)
    root = ET.fromstring(multiplefn)
    functions = root.findall("functioncall")
    return [json.loads(fn.text) for fn in functions]
   
def extract_function_prompt(completion):
    completion = completion.strip()
    pattern = r"(<multiple_prompts>(.*?)<\/multiple_prompts>)"
    match = re.search(pattern, completion, re.DOTALL)
    if not match:
        return None

    multiplefn = match.group(1)
    root = ET.fromstring(multiplefn)
    functions = root.findall("function_prompt")
    return [json.loads(fn.text) for fn in functions]

    
def generate_function_call(prompt, model, tokenizer, generation_config_overrides={}):
    fn = """{"name": "module_name.function_name", "arguments": {"arg_1": "value_1", "arg_2": value_2, ...}}"""
    prompt = f"""<|im_start|>system
You are a helpful assistant with access to the following functions from the 'test_functions' module:

{convert_to_openai_function(test_functions.Joke)}

{convert_to_openai_function(test_functions.BookRecommendation)}

{convert_to_openai_function(test_functions.SongRecommendation)}

{convert_to_openai_function(test_functions.LoadFile)}

You also have access to all of the functions in the numpy module. Please refer to numpy functions as 'np.function_name'.

To use these functions respond with:

<multiplefunctions>
    <functioncall> {fn} </functioncall>
    <functioncall> {fn} </functioncall>
    ...
</multiplefunctions>

Always wrap in the <multiplefunctions> tag, even if there is only one function.

Edge cases you must handle:
- If there are no functions that match the user request, you will respond politely that you cannot help.
<|im_end|>
<|im_start|>user
{prompt}<|im_end|>
<|im_start|>assistant"""

    # print("THIS IS THE INPUT PROMPT:\n\n {}".format(prompt))
    # print("^^^ THAT WAS THE PROMPT")
    model = model.eval()
    inputs = tokenizer(prompt, return_tensors="pt").to(model.device)
    n_tokens = inputs.input_ids.numel()

    with torch.inference_mode():
        generated_tokens = model.generate(**inputs, pad_token_id=tokenizer.eos_token_id, max_new_tokens=2048, do_sample=True)


    output = tokenizer.decode(
        generated_tokens.squeeze()[n_tokens:], skip_special_tokens=False
    )
    return output


def generate_response_with_function_calls(input_prompt, model, tokenizer):
    prompt=f"""<|im_start|>system
    You are an expert at generating a natural language prompts 
    for calling specified functions, and if a function is available,
    you always prefer to generate a prompt to call it. If the user
    asks for a function that is not available, you have the ability 
    to generate python code that achieves the desired functinality.
    You are very very CONCISE in your responses.

    Here are functions that can be called from the test_functions module:
{convert_to_openai_function(test_functions.Joke)}

{convert_to_openai_function(test_functions.BookRecommendation)}

{convert_to_openai_function(test_functions.SongRecommendation)}

{convert_to_openai_function(test_functions.LoadFile)}

    You also have access to all functions in the numpy module. 
        
    
    Here is how you must respond to my messages:
            
    1. When you use a tool, you will generate a natural language prompt that describes what you are looking to get with the tool. You can generate multiple prompts at once if it makes sense. When you respond, you must respond with the prompt contained in JSON format and wrapped between this tag '<function_prompt></function_prompt>'
        Example:
            ME(User): "I really like country music and I want to listen to more. I also like jokes sometimes."
            YOU(Assistant): 
<multiple_prompts>
    <function_prompt>{{"name":"prompt_for_function_call_1", "prompt": "I am looking for song recommendations for country music"}}</function_prompt>,
    <function_prompt>{{"name":"prompt_for_function_call_2", "prompt": "I am looking for a good joke"}}</function_prompt>,
    ...
</multiple_prompts>

    4. If you choose not to generate a prompt, explain why you chose not
        to, and generate a function that achieves the desired functionality.
            Example:
            ME(User): "I want to know the flibbity transformation of the value flangbob"
            YOU(Assistant): "I am sorry, but I cannot generate a function calling prompt for your input, because I dont have access to a function related to calculating a 'flibbity transformation'. Here is my best interpretation of the function, 'flibbity_transformation
                '''
                def flibbity_transformation(flangbop):
                    return ((flangbop/10) + 3) * flangbop"
                '''

    <|im_end|>
    <|im_start|>user
    {input_prompt}<|im_end|>
    <|im_start|>assistant
"""

    model = model.eval()
    inputs = tokenizer(prompt, return_tensors="pt").to(model.device)
    n_tokens = inputs.input_ids.numel()

    with torch.inference_mode():
        generated_tokens = model.generate(**inputs, pad_token_id=tokenizer.eos_token_id, max_new_tokens=2048, do_sample=True)


    output = tokenizer.decode(
        generated_tokens.squeeze()[n_tokens:], skip_special_tokens=False
    )
    # print("\n\nPROMPTING MODEL OUTPUT\n\n{}".format(output))
    # print("END RAW MODEL OUTPUT")
    function_prompts = extract_function_prompt(output)
    results = {}
    if function_prompts:
        for function_prompt in function_prompts:
            prompt_f = function_prompt["prompt"]
            response = generate_function_call(prompt_f, model, tokenizer)
            functions = extract_function_calls(response)

            for function in functions:
                results[function_prompt["name"]] = function
                # print(function["name"])
                # print(function["name"].split("."[0], 1))
                # print("now calling function called {}.{}".format(function["name"].split(".", 1)[0], function["name"].split(".", 1)[1]))
                # results[function_prompt["name"]]["return_value"] = getattr(test_functions, function["name"])(**function["arguments"])
                results[function_prompt["name"]]["return_value"] = getattr(
                                                eval(function["name"].split(".", 1)[0]),
                                                function["name"].split(".", 1)[1])(**function["arguments"])
            # print(results)
    else:
        print("WE DID NOT MAKE A FUNCTION")

    # print("THESE ARE THE RESUKLTS{}".format(results))
    # TODO link this to real functions and have the model produce a polished final output
    return_prompt = f"""<|im_start|>system
    You are a hepful assistant who is good at responding to user queries in a helpful way.
    You will be given a user prompt, and some function calls and their return values in json format.
    Please generate a nicely formatted response that solves the user prompt. Respond to ALL parts of the user prompt.

    Here is an example of a desired response format
    1. When you receive a message with the result from the  
        tool, you must present the result to me in a nice way (CONCISELY), 
        and ask: 'What else can I help with?'
        Example:
            ME(User): "I am looking for song recommendations for metal music.
    {{'prompt_for_function_call_1': {{'name': 'test_functions.songRecommendation', 'arguments': {{'genre': 'metal'}}, 'return_value': 'Based on your interest in: metal\nI think you would like the song 'Heavy Iron'}}}}
            YOU(Assistant): "If you want a song recommendation for metal music, here is a great one! 
                1. Heavy Iron

    2. If there were multiple functions called, try to combine
        the results of their outputs into a single unified response.
        Example:
            ME(User): <prompt that requires two function calls>
{{'prompt_for_function_call_1': {{'name': 'function_name_1', 'arguments': {{'arg_name_1': 'arg1', 'arg_name_2': 'arg_2'}}, 'return_value': 'return value 1'}}, 'prompt_for_function_call_2': {{'name': 'function_name_2', 'arguments': {{'arg_name_1': 'arg1', 'arg_name_2': 'arg_2'}}, 'return_value': 'return value 2'}}}}
            YOU(Assistant): <Response to prompt relating to function call 1>
{{Response to prompt relating to function call 2}}

    3. If there was an error in using the function/tool, the 
        user prompt will be followed by an empty set of curly braces {{}}.
        Inform the user that there was a problem calling a function, and try to give a reason why.
            Example:
            ME(User): "I want to know the flibbity transformation of the value flangbob"
            YOU(Assistant): "I am sorry, but there was a problem trying to generate a function call calling prompt for your input, because I dont have access to a function related to calculating a 'flibbity transformation'. Here is my best interpretation of the function, 'flibbity_transformation
    '''
        def flibbity_transformation(flangbop):
            return ((flangbop/10) + 3) * flangbop"
    ''' 

    <|im_end|>
    <|im_start|>user
    {input_prompt}
    {results}<|im_end|>
    <|im_start|>assistant
    """
    inputs = tokenizer(return_prompt, return_tensors="pt").to(model.device)
    n_tokens = inputs.input_ids.numel()

    with torch.inference_mode():
        generated_tokens = model.generate(**inputs, pad_token_id=tokenizer.eos_token_id, max_new_tokens=2048, do_sample=True)


    output = tokenizer.decode(
        generated_tokens.squeeze()[n_tokens:], skip_special_tokens=False
    )


    return output, results

if __name__=="__main__":
    # mixtral
    # MODELS DECLARATION
    # model_name = "../LLM_Models/Mistral/OpenHermes-2.5-Mistral-7B"    # Path to chat model and 
    model_name = "../LLM_Models/Mistral/Mistral-7B-Instruct-v0.2"    # Path to chat model and 
    print("loading tokenizer")
    tokenizer = AutoTokenizer.from_pretrained(model_name, local_files_only=True)
    # tokenizer.pad_token = tokenizer.eos_token
    # https://github.com/chujiezheng/chat_templates
    # if using MISTRAL INSTRUCT, which does not accept a system prompt, use this instead?
    print('###### Corrected Chat Template ######')
    chat_template = open('./chat_templates/mistral-instruct.jinja').read()
    chat_template = chat_template.replace('    ', '').replace('\n', '')
    tokenizer.chat_template = chat_template


    embedding_model_name = "Embedding_Models/instructor-xl"         # path embedding model
    print("loading model")

    model = AutoModelForCausalLM.from_pretrained(model_name, local_files_only=True, device_map="auto")
    print("MODEL LOADED")

    # generation_func = partial(generate_function_call, model=model, tokenizer=tokenizer)

    prompts = [
        "using test_functions, give me a fantasy book recommendation and then give me a good joke",
        "load these files using the test_functions module ['test1.csv', 'test2.csv', test6.csv']",
        "Load this text file [./B_Cell_Model/B Cell Abm Netlogo Notes.txt] with numpy, then summarize the text"
    ]


    import time
    for prompt in prompts:
        start = time.time()
        # completion = generate_function_call(prompt, model, tokenizer)
        # functions = extract_function_calls(completion)

        completion, function_calls = generate_response_with_function_calls(prompt, model, tokenizer)
        print("TOTAL GENERATION TIME IS {:.2f}".format(time.time() - start))
        print("THIS IS THE COMPLETED RESPONSE FROM THE MODEL\n\n{}".format(completion))
        print("THESE ARE THE FUNCTION CALLS")
        print(function_calls)

