# This is an example of LLM Function calling that I took from this video and this repository
# not much of this code is mine but this was for learning
# https://www.youtube.com/watch?v=MQmfSBdIfno
# https://github.com/AIAnytime/Function-Calling-Mistral-7B

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

import gc, inspect, json, re
from typing import get_type_hints

import xml.etree.ElementTree as ET
from functools import partial
from langchain.pydantic_v1 import BaseModel, Field, validator
from langchain.chains.openai_functions import convert_to_openai_function

counter = 0
class test_functions():
    def BookRecommendation(interest, recommended_book):
        global counter
        counter += 1
        print("based on your interest in :\n{}".format(interest))
        print("I think this would be a good book:\n{}".format(recommended_book))
        return "book recommend return {}".format(counter)

    def Joke(setup, punchline):
        global counter
        counter += 1
        print("SETUP:\n{}".format(setup))
        print("PUNCHLINE:\n{}".format(punchline))
        return "joke return {}".format(counter)

    def SongRecommendation(genre, song):
        global counter
        counter += 1
        print("Based on your interest in: {}".format(genre))
        print("I think you would like:\n{}".format(song))
        return "song recommend return{}".format(counter)

    def LoadFile(filename):
        global counter
        counter += 1
        print("LOADING FILE {}".format(filename))
        return "file load return{}".format(counter)
        
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
        


class LoadFile(BaseModel):
    """Loads a csv file into a numpy array."""
    filename: list[str] = Field(description="path of file to load")

    @validator("filename")
    def filename_must_not_be_empty(cls, field):
        if not field:
            raise ValueError("filename cannot be empty.")
        return field
    

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
    fn = """{"name": "function_name", "arguments": {"arg_1": "value_1", "arg_2": value_2, ...}}"""
    prompt = f"""<|im_start|>system
You are a helpful assistant with access to the following functions:

{convert_to_openai_function(Joke)}

{convert_to_openai_function(BookRecommendation)}

{convert_to_openai_function(SongRecommendation)}

{convert_to_openai_function(LoadFile)}

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

    generation_config = model.generation_config
    generation_config.update(
        **{
            **{
                "use_cache": True,
                "do_sample": True,
                "temperature": 0.2,
                "top_p": 1.0,
                "top_k": 0,
                "max_new_tokens": 512,
                "eos_token_id": tokenizer.eos_token_id,
                "pad_token_id": tokenizer.eos_token_id,
            },
            **generation_config_overrides,
        }
    )

    model = model.eval()
    inputs = tokenizer(prompt, return_tensors="pt").to(model.device)
    n_tokens = inputs.input_ids.numel()

    with torch.inference_mode():
        generated_tokens = model.generate(**inputs, pad_token_id=tokenizer.eos_token_id, max_new_tokens=2048, do_sample=True)


    output = tokenizer.decode(
        generated_tokens.squeeze()[n_tokens:], skip_special_tokens=False
    )
    return output


def generate_response_with_function_calls(prompt, model, tokenizer):
    prompt=f"""<|im_start|>system
    You are an expert at generating a natural language prompt 
    for calling specified functions, and if a function is available,
    you always prefer to generate a prompt to call it.
    You are very very CONCISE in your responses.

    Here are functions that can be called:
        BookRecommendation
        Joke
        SongRecommendation
        LoadFile
    
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
                
    2. When you receive a SUCCESS message with the result from the  
        tool, you must present the result to me in a nice way (CONCISELY), 
        and ask: 'What else can I help with?'
        Example:
            ME(User): "SUCCESS! result from `I am looking for song recommendations for country music` is ['my horse is getting old', 'I sure love my farm', 'cold beer and blue jeans are all I need']"
            YOU(Assistant): "If you want song recommendations for country music, here are a few good ones! 
                1. my horse is getting old"
                2. I sure love my farm
                3. cold beer and blue jeans are all I need
            ME(User): "Thanks, but I don't really like country music that much. Do you know any rock music?"
            YOU(Assistant): 
<multiple_prompts>
    <function_prompt>{{prompt for function call in JSON format}}</function_prompt>
</multiple_prompts>

    3. When I say there was an error in using the function/tool,
        you must generate a new prompt that has the same details and meaning, but is worded differently.
            Example:
            ME(User): "There was an error in your use of the tool:..."
            YOU(Assistant):                 
<multiple_prompts>
    <function_prompt>{{prompt for function call in JSON format}}</function_prompt>
</multiple_prompts>

    4. If you choose not to generate a prompt, explain why you chose not to.
            Example:
            ME(User): "I want to know the flibbity transformation of the value flangbob"
            YOU(Assistant): "I am sorry, but I cannot generate a function calling prompt for your input, because I dont have access to a function related to calculating a 'flibbity transformation'"
    
    Please respond to the user prompt, using function calls when apropriate. If you prompt a function call, stop and wait for the results before continuing your response.
    <|im_end|>
    <|im_start|>user
    {prompt}<|im_end|>
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

    function_prompts = extract_function_prompt(output)
    results = {}
    if function_prompts:
        for function_prompt in function_prompts:
            prompt_f = function_prompt["prompt"]
            response = generate_function_call(prompt_f, model, tokenizer)
            functions = extract_function_calls(response)

            for function in functions:
                # print("now calling function called test_functions.{}".format(function["name"]))
                results[function_prompt["name"]] = getattr(test_functions, function["name"])(**function["arguments"])
            # print(results)
    else:
        print("WE DID NOT MAKE A FUNCTION")


    # TODO link this to real functions and have the model produce a polished final output
    return results

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
        "give me a fantasy book recommendation and then give me a good joke",
        "load these files ['test1.csv', 'test2.csv', test6.csv']"
    ]

    for prompt in prompts:
        completion = generate_function_call(prompt, model, tokenizer)
        functions = extract_function_calls(completion)

        completion = generate_response_with_function_calls(prompt, model, tokenizer)
        print(completion)

