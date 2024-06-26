# This is an example of LLM Function calling that I took from this video and this repository
# not much of this code is mine but this was for learning
# https://www.youtube.com/watch?v=MQmfSBdIfno
# https://github.com/AIAnytime/Function-Calling-Mistral-7B

import torch
import numpy as np
from sklearn.metrics.pairwise import cosine_similarity
import time
import json

import inspect
import builtins
import traceback

import random

from transformers import AutoTokenizer, AutoModelForCausalLM
from InstructorEmbedding import INSTRUCTOR

import os
os.environ['PYTORCH_CUDA_ALLOC_CONF'] = 'expandable_segments:True'
import re

import gc, inspect, json, re
from typing import get_type_hints

import xml.etree.ElementTree as ET
# from functools import partial
# from langchain.pydantic_v1 import BaseModel, Field, validator
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
    
    def LoadText(filename: str) -> str:
        """loads a text file as a string and returns it."""
        # global counter += 1
        print("CALLING LOAD TEXT FILE")
        with open(filename, "r") as f:
            res = f.read()
        return res
        

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
   

def extract_function_name(completion):
    # print("INSIDE EXTRACT FUNC NAME")
    completion = completion.strip()
    pattern = r"<function_name>(.*?)</function_name>"
    match = re.search(pattern, completion, re.DOTALL)
    # print(match)
    if not match:
        return None

    multiplefn = match.group(1)
    # print(multiplefn)
    res = json.loads(multiplefn)
    return res

def extract_single_function_prompt(completion):
    # gets last prompt in function call list
    completion = completion.strip()
    pattern = r"<function_prompt>(.*?)</function_prompt>"
    matches = re.findall(pattern, completion, re.DOTALL)
    if not matches:  # Check if the list is empty
        return None
    # Take the last match found
    last_match = matches[-1]
    # print(last_match)
    res = json.loads(last_match)
    return res

def extract_model_thoughts(completion):
    completion = completion.strip()
    pattern = r"<THINKING>(.*?)</THINKING>"
    match = re.search(pattern, completion, re.DOTALL)
    if not match:
        return None
    res = match.group(1)
    # print(res)
    return res

def extract_response_after_thinking(input_string):
    # Define the regex pattern to find the tag and capture everything after it
    pattern = r'</THINKING>(.*)'
    # Use re.DOTALL to make '.' match any character including newline
    match = re.search(pattern, input_string, re.DOTALL)
    if match:
        # Return the part of the string after the tag
        return match.group(1)
    else:
        # If the tag isn't found, you might want to return None or the original string
        return input_string

def extract_function_prompt(completion):
    completion = completion.strip()
    pattern = r"(<multiple_prompts>(.*?)<\/multiple_prompts>)"
    match = re.search(pattern, completion, re.DOTALL)
    if not match:
        return None
    # print(match)
    multiplefn = match.group(1)
    # print(multiplefn)
    root = ET.fromstring(multiplefn)
    functions = root.findall("function_prompt")
    return [json.loads(fn.text) for fn in functions]

    
def generate_function_call(input_prompt, model, tokenizer, generation_config_overrides={}):
    fn = """{"name": "module_name.function_name", "arguments": {"arg_1": "value_1", "arg_2": value_2, ...}}"""
    # dynamically get function signatures/declarations by getting function name from LLM, and including it as Openai function in next prompt
    prompt_get_function_name = f"""<|im_start|>system
    You are a helpful assistant who is an expert at choosing the correct function to call for a given task.
    You know about only the following modules and functions. If none of these functions will explicitly and
    exactly solve the prompt, you will return 'No relevant function for this prompt, you're on your own.'.

        Functions that can be called from the test_functions module:
            BookRecommendation
            Joke
            SongRecommendation
            LoadText

        All functions in the numpy module. Refer to these functions like 'np.function_name'.
        Built in python function. Refer to them functions as 'builtins.function_name'

    
    1. If there is a function that fits the prompt, you will respond with only the name of the function,
        formatted as a python dict, wrapped in this tag <function_name></function_name>. Follow this example:
        Example:
            ME(User): I am looking to load the file 'test6.csv' using the LoadText function from the test_functions module
            YOU(Assistant):  <function_name>{{"name":"test_functions.LoadText"}}</function_name>
    
    2. If there is a no function that fits the prompt, you will respond with "No relevant function for this prompt, you're on your own.":
        Example:
            ME(User): I want the flangbop transformation of flibbity
            YOU(Assistant):  <function_name>{{"name":"No relevant function for this prompt, you're on your own."}}</function_name><|im_end|>


<|im_start|>user{input_prompt}<|im_end|>
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
                "max_new_tokens": 2048,
                "eos_token_id": tokenizer.eos_token_id,
                "pad_token_id": tokenizer.eos_token_id,
            },
            **generation_config_overrides,
        }
    )

    model = model.eval()
    inputs = tokenizer(prompt_get_function_name, return_tensors="pt").to(model.device)
    n_tokens = inputs.input_ids.numel()

    with torch.no_grad(), torch.backends.cuda.sdp_kernel(enable_flash=False, enable_math=False, enable_mem_efficient=True):
        generated_tokens = model.generate(**inputs, generation_config=generation_config)

    output = tokenizer.decode(
        generated_tokens.squeeze()[n_tokens:], skip_special_tokens=False
    )
    print("FUNCTION NAMING OUTPUT IS {}".format(output))

    func_name = extract_function_name(output)
    print(func_name)
    print(func_name["name"])
    if func_name["name"] == "No relevant function for this prompt, you're on your own.":
        print("WE GOT A PROBLEMO")
        return func_name["name"]
    # numpy alias to np
    if func_name["name"].split(".", 1)[0] == "numpy":
        # print("\n\n\n\n\nWE ARE REPLACING THE NAME NUMPY WITH NP\n\n\n\n\n")
        func_name["name"] = "np."+ func_name["name"].split(".", 1)[1]
        # print(func_name["name"])
    # print(inspect.getsource(eval(func_name["name"])))
    func_ref = eval(func_name["name"])
    if func_ref:
        func_context_text = "Use this function\n"
        # print(func_name["name"])
        openai_func_ref = convert_to_openai_function(func_ref)
        openai_func_ref["name"] = func_name["name"]
    else:
        func_context_text = ""
        openai_func_ref = ""
    # print("Func ref {}".format(func_ref))
    # print(func_context_text)
    # print(openai_func_ref)

    prompt = f"""<|im_start|>system
You are a helpful assistant and are skilled at putting together runction calls. 

{func_context_text}
{openai_func_ref}

To use functions respond with:

<multiplefunctions>
    <functioncall> {fn} </functioncall>
    <functioncall> {fn} </functioncall>
    ...
</multiplefunctions>

Always wrap in the <multiplefunctions> tag, even if there is only one function.

Edge cases you must handle:
- If the use responds with a failure message from a previous attempt, you will analyze the error, and attempt to generate a function call that fixes it.<|im_end|>
<|im_start|>user
{input_prompt}<|im_end|>
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
                "max_new_tokens": 2048,
                "eos_token_id": tokenizer.eos_token_id,
                "pad_token_id": tokenizer.eos_token_id,
            },
            **generation_config_overrides,
        }
    )


    # print("THIS IS THE INPUT PROMPT:\n\n {}".format(prompt))
    # print("^^^ THAT WAS THE PROMPT")
    model = model.eval()
    inputs = tokenizer(prompt, return_tensors="pt").to(model.device)
    n_tokens = inputs.input_ids.numel()

    with torch.no_grad(), torch.backends.cuda.sdp_kernel(enable_flash=False, enable_math=False, enable_mem_efficient=True):
        generated_tokens = model.generate(**inputs, generation_config=generation_config)


    output = tokenizer.decode(
        generated_tokens.squeeze()[n_tokens:], skip_special_tokens=False
    )
    return output


def generate_response_with_function_calls(input_prompt, model, tokenizer, generation_config_overrides={}):
    prompt=f"""<|im_start|>system
    You are an expert at generating a natural language prompts 
    for calling specified functions, and if a function is available,
    you always prefer to generate a prompt to call it.

    Here are functions that can be called from the test_functions module:
        BookRecommendation
        Joke
        SongRecommendation
        LoadText

    You also have access to all functions in the numpy module. Refer to numpy functions as 'np.function_name'
    You also have access to all functions in the pandas module. Refer to pandas functions as 'pd.function_name'
        
    
    Here is how you must respond to my messages:
            
    1. When you use a tool, you will generate a natural language prompt that describes what you are looking to get with the tool. Only have one tool use or instruction per prompt. You can generate multiple prompts at once if it makes sense, but you will always wrap all prompts in between this tag <multiple_prompts></multiple_prompts> even if there is only one tool use prompt. When you respond, you must respond with each tool use prompt contained in JSON format and wrapped between this tag '<function_prompt></function_prompt>'
        Example:
            ME(User): "I really like country music and I want to listen to more. I also like jokes sometimes."
            YOU(Assistant): 
<multiple_prompts>
    <function_prompt>{{"name":"prompt_for_function_call_1", "prompt": "I am looking for song recommendations for country music"}}</function_prompt>,
    <function_prompt>{{"name":"prompt_for_function_call_2", "prompt": "I am looking for a good joke"}}</function_prompt>,
    ...
</multiple_prompts>

    2. If you choose not to generate a prompt because it is not available to you, explain why you chose not
        to generate a prompt, and write a function that achieves the desired functionality.
            Example:
            ME(User): "I want to know the flibbity transformation of the value flangbob"
            YOU(Assistant): "I am sorry, but I cannot generate a function calling prompt for your input, because I dont have access to a function related to calculating a 'flibbity transformation'. Here is my best interpretation of the function, 'flibbity_transformation'
'''
def flibbity_transformation(flangbop):
    return ((flangbop/10) + 3) * flangbop"
'''<|im_end|>
<|im_start|>user
{input_prompt}<|im_end|>
<|im_start|>assistant
"""

    generation_config = model.generation_config
    generation_config.update(
        **{
            **{
                "use_cache": True,
                "do_sample": True,
                "temperature": 0.2,
                "top_p": 1.0,
                "top_k": 0,
                "max_new_tokens": 2048,
                "eos_token_id": tokenizer.eos_token_id,
                "pad_token_id": tokenizer.eos_token_id,
            },
            **generation_config_overrides,
        }
    )

    model = model.eval()
    inputs = tokenizer(prompt, return_tensors="pt").to(model.device)
    n_tokens = inputs.input_ids.numel()

    with torch.no_grad(), torch.backends.cuda.sdp_kernel(enable_flash=False, enable_math=False, enable_mem_efficient=True):
        generated_tokens = model.generate(**inputs, generation_config=generation_config)


    output = tokenizer.decode(
        generated_tokens.squeeze()[n_tokens:], skip_special_tokens=False
    )
    # print("\n\nPROMPTING MODEL OUTPUT\n\n{}".format(output))
    # print("END RAW MODEL OUTPUT")
    function_prompts = extract_function_prompt(output)
    single_f_prompt_output = extract_single_function_prompt(output)
    results = {}

    if function_prompts:
        for function_prompt in function_prompts:
            # wrap the whole thing in a try block, if anything goes wrong still get an output for the user
            try:
                prompt_f = function_prompt["prompt"]
                # print("RAW FUNCTION PROMPT is: {}".format(prompt_f))
                # LOOP TO GENERATE VALID FUNCTION SIGNATURES
                attempts = 0 # tracker for trying to generate valid function signatures
                called_function = False
                while attempts < 5 and called_function == False:
                    # print("INPUT FUNCTION PROMPT is: {}".format(prompt_f))

                    try:
                        response = generate_function_call(prompt_f, model, tokenizer)
                    except Exception as e:
                        attempts += 1
                        # print("Did not properly generate a function call for prompt: {}".format(prompt_f))
                        # print(e)
                        continue

                    # print("GENERATED RESPONSE TO PROMPT: {}".format(response))
                    functions = extract_function_calls(response)
                    
                    for function in functions:
                        try:
                            # print(function["name"])
                            # print(function["name"].split("."[0], 1))
                            # print("now calling function called {}.{}".format(function["name"].split(".", 1)[0], function["name"].split(".", 1)[1]))
                            # print(function)
                            results[function_prompt["name"]] = function
                            results[function_prompt["name"]]["return_value"] = getattr(
                                                            eval(function["name"].split(".", 1)[0]),
                                                            function["name"].split(".", 1)[1])(**function["arguments"])
                            called_function = True
                        # print(results)
                        except NameError as e:
                            # print("WE DIDNT CALL THE FUNCTION CORRECTLY, NAME ERROR")
                            # print(e)
                            prompt_f += f"""
{response}<|im_end|>
<|im_start|>user
FAILURE!! Name Error
{traceback.format_exc()}
Please try again to generate a valid function call.<|im_end|>
<|im_end|>assistant
"""
                            attempts += 1
                            continue

                        except ValueError as e:
                            # print("Something is wrong with the file, or we chose the wrong function")
                            # print(e)
                            prompt_f += f"""
{response}<|im_end|>
<|im_start|>user
FAILURE!! ValueError
{traceback.format_exc()}
Please try again to generate a valid function call.<|im_end|>
<|im_end|>assistant
"""
                            attempts += 1
                            continue

                        except Exception as e:
                            # print("Something is wrong with the file, or we chose the wrong function")
                            # print(e)
                            prompt_f += f"""
{response}<|im_end|>
<|im_start|>user
FAILURE!!
{traceback.format_exc()}
Please try again to generate a valid function call.<|im_end|>
<|im_end|>assistant
"""
                            attempts += 1
                            continue

            except Exception as e:
                # print("SOMETHING WENT VERY WRONG OUTSIDE THE NORMAL ERROR HANDLING BLOCKS")
                # print(e)
                results[function_prompt["name"]]["error"] = traceback.format_exc()


    else:
        # print("WE DID NOT MAKE A FUNCTION")
        # print(function_prompts)
        pass

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
            YOU(Assistant):<Response to prompt relating to function call 1>
<Response to prompt relating to function call 2>

    3. If there was an error in using the function/tool, the 
        user prompt will be followed by an empty set of curly braces {{}}, or the error message will be given.
        Inform the user that there was a problem calling a function, and try to give a reason why as well as a path forward.
            Example:
            ME(User): "I want to know the flibbity transformation of the value flangbob"
            YOU(Assistant):
"I am sorry, but there was a problem trying to generate a function call calling prompt for your input, because I dont have access to a function related to calculating a 'flibbity transformation'. Here is my best interpretation of the function, 'flibbity_transformation
'''
    def flibbity_transformation(flangbop):
        return ((flangbop/10) + 3) * flangbop"
''' 

    Using the function calls and their return values, please respond to all parts of the user prompt.
<|im_end|>
<|im_start|>user
{input_prompt}
{results}
<|im_end|>
<|im_start|>assistant
"""
    model = model.eval()
    inputs = tokenizer(return_prompt, return_tensors="pt").to(model.device)
    n_tokens = inputs.input_ids.numel()

    with torch.no_grad(), torch.backends.cuda.sdp_kernel(enable_flash=False, enable_math=False, enable_mem_efficient=True):
        generated_tokens = model.generate(**inputs, pad_token_id=tokenizer.eos_token_id, max_new_tokens=2048, do_sample=True)


    output = tokenizer.decode(
        generated_tokens.squeeze()[n_tokens:], skip_special_tokens=False
    )


    return output, results

def chat_with_function_calling(input_prompt, model, tokenizer, generation_config_overrides={}, MAX_FUNCTION_CALLS=3):
    def get_prompt(input_prompt):
        prompt=f"""<|im_start|>system
You are a chatbot assistant designed to efficiently integrate tools, like API calls or file loading functions, into your responses when needed. Your responses are concise and directly address the user's query, either by providing information directly or by indicating a specific tool action required to obtain the answer.

Here's how to proceed with user prompts:

0. **Planning**: 
   - Upon receiving a prompt, think about your response strategy and summarize it within <THINKING></THINKING> tags.
   - If a previous plan is provided (<PREVIOUS THOUGHTS>), consider it and any tool results (<FUNCTION CALLS AND RESULTS>) in your planning.

1. **Responding Without Tools**:
   - If the query is subjective or doesn't benefit from tool usage (like opinions or summaries), respond normally.

2. **Using Tools**:
   - When a specific answer requires a tool (e.g., to fetch data, perform calculations), instruct the tool use in a single, clear statement.
   - Format this instruction in JSON, enclosed in `<function_prompt></function_prompt>` tags. Specify full input arguments and ensure only one discrete action per instruction.

3. **Interpreting Tool Results**:
   - The user will use the `<FUNCTION CALLS AND RESULTS>` tag to understand previous tool outputs. Decide if you need further tool action or if you can now respond effectively.

4. **Final Response**:
   - If no new tool action is needed, use the tool results to provide a complete, helpful answer. Conclude with an offer of further assistance.
```
Example Response Structure:
<PREVIOUS THOUGHTS>
- Summary of previous planning or tool results considered.
</PREVIOUS THOUGHTS>
<THINKING>
- New plan based on the user prompt and any previous tool results.
</THINKING>
If a new tool action is needed:
<function_prompt>{{"prompt": "Specific tool action in JSON format"}}</function_prompt>
Otherwise, respond directly to the user, using tool results if applicable, and conclude with an offer of further help.
```<|im_end|>
<|im_start|>user
{input_prompt}<|im_end|>
<|im_start|>assistant
"""
        return prompt


    generation_config = model.generation_config
    generation_config.update(
        **{
            **{
                "use_cache": True,
                "do_sample": True,
                "temperature": 0.2,
                "top_p": 1.0,
                "top_k": 0,
                "max_new_tokens": 2048,
                "eos_token_id": tokenizer.eos_token_id,
                "pad_token_id": tokenizer.eos_token_id,
            },
            **generation_config_overrides,
        }
    )


    extracted_function = None
    chat_function_return = {}
    exception_message = ""
    # get first prompt based on user input
    for attempt in range (5):
        if attempt>0 : print("GENERATION ATTEMPT NUM {}".format(attempt))
        try:
            model_input = input_prompt + str(exception_message)

            prompt = get_prompt(model_input)

            model = model.eval()
            inputs = tokenizer(prompt, return_tensors="pt").to(model.device)

            n_tokens = inputs.input_ids.numel()

            # print("NUM INPUT TOKENS: {}".format(n_tokens))
            with torch.no_grad(), torch.backends.cuda.sdp_kernel(enable_flash=False, enable_math=False, enable_mem_efficient=True):
                generated_tokens = model.generate(**inputs, generation_config=generation_config)

            output = tokenizer.decode(
                generated_tokens.squeeze()[n_tokens:], skip_special_tokens=False
            )
            # print("MODEL INPUT IS: \n{}".format(model_input))
            print("TOP PART")
            print("MODEL OUTPUT IS: \n{}".format(output))
            model_thinking = extract_model_thoughts(output)
            # print("MODEL THOUGHTS ARE: \n{}".format(model_thinking))
            extracted_function = extract_single_function_prompt(output)
            print("EXTRACTED FUNCTION IS {}".format(extracted_function))
            if extracted_function is None:
                # print("EXTRACTED FUNCTION IS NONE, so no function called")
                # print("OUTPUT IS {}".format(output))
                break
            if not "prompt" in extracted_function:
                raise KeyError("Prompt")
            break # if we made it here then we extracted a function without error. break for loop and move on
        except Exception as E:
            # print("Exception in top part generation {}".format(E))
            exception_message = f"""\nAttempted to generate instruction for function call, did not receive a valid response. {traceback.format_exc()}. \n Please try again\n"""

    # print(extracted_function)
    total_function_calls = 0
    chat_function_return = {}
    while extracted_function and total_function_calls < MAX_FUNCTION_CALLS:
        if total_function_calls>0 : print("TOTAL FUNCTION CALLS  NUM {}".format(total_function_calls))

        total_function_calls += 1   # only allow up to MAX_FUNCTION_CALLS for valid LLM function calls
        initial_prompt = extracted_function["prompt"]
        num_func_calls = 0
        while num_func_calls < MAX_FUNCTION_CALLS: # retry up to MAX_FUNCTION_CALLS times to get a valid response
            if num_func_calls>0 : print("NUM RETRY TO GET A VALID FUNCTION {}".format(num_func_calls))
            num_func_calls += 1
            try:
                # print("INPUT TO GENERATE FUNCTION CALL IS {}".format(extracted_function["prompt"]))
                function_response = generate_function_call(extracted_function["prompt"], model, tokenizer)
                print("RESPONSE WHEN ASKING FOR FUNCTION CALL IS:\n{}".format(function_response))
                if function_response == "No relevant function for this prompt, you're on your own.":
                    chat_function_return[extracted_function["name"]] = {"prompt":initial_prompt, "return_value": function_response}
                    break
                else:
                    try:
                        function = extract_function_calls(function_response)[0]
                        function["prompt"] = initial_prompt
                        # print("calling function: {}".format(function))
                        # going through each argument, and if it contains something that looks like indexing, lets eval
                        evaluated_arguments = function["arguments"]
                        for arg_key in evaluated_arguments:
                            if bool(re.search(r'\[[^\]]+\]', evaluated_arguments[arg_key])):
                                evaluated_arguments[arg_key] = eval(evaluated_arguments[arg_key])
                        # execute extracted function
                        try:
                            function["return_value"] = getattr(eval(function["name"].split(".", 1)[0]), function["name"].split(".", 1)[1])(**evaluated_arguments)
                        except TypeError:
                            # trying for positional arguments?
                            function["return_value"] = getattr(eval(function["name"].split(".", 1)[0]), function["name"].split(".", 1)[1])(*evaluated_arguments.values())



                        print("EXECUTED FUNCTION using getattr( {}, {} )".format(function["name"].split(".", 1)[0], function["name"].split(".", 1)[1]))
                        print("ARGS WERE {}".format(function["arguments"]))
                        chat_function_return[extracted_function["name"]] = function
                        # print(chat_function_return)
                        # print("added function output to chat return")
                        break   #if we call a function and get a return, then we did it and we can stop trying
                    except Exception as E:
                        # adding error to function prompt to function generator does a better job
                        extracted_function["prompt"] += f"""\nAttempted function call with function {function["name"]}\n and with arguments {function["arguments"]} \nERROR, exception thrown. Exception is: {traceback.format_exc()}"""
            except Exception as E:
                extracted_function["prompt"] += f"""\nAttempted to generate function call, did not receive a valid response. Exception is {traceback.format_exc()}. \n Please try again"""
                # print(E)
                pass
                
            
        # now that we have either a return value, or a blank from not getting a valid response enough times, 
        # query the original model and continue initial response generation
        exception_message = ""
        for attempt in range (5):
            if attempt>0 : print("GENERATION ATTEMPT NUM {}".format(attempt))
            # print("WE ARE IN THE BOTTOM GENERATION PART")
            try:
                model_input = input_prompt + "<FUNCTION CALLS AND RESULTS>\n" + str(chat_function_return) + "</FUNCTION CALLS AND RESULTS>\n" + "<PREVIOUS THINKING>\n" + model_thinking + "</PREVIOUS THINKING>\n" + exception_message

                prompt = get_prompt(model_input)

                model = model.eval()
                inputs = tokenizer(prompt, return_tensors="pt").to(model.device)
                n_tokens = inputs.input_ids.numel()

                # print("NUM INPUT TOKENS when generating bottom part: {}".format(n_tokens))
                with torch.no_grad(), torch.backends.cuda.sdp_kernel(enable_flash=False, enable_math=False, enable_mem_efficient=True):
                    generated_tokens = model.generate(**inputs, generation_config=generation_config)

                output = tokenizer.decode(
                    generated_tokens.squeeze()[n_tokens:], skip_special_tokens=False
                )
                # print("MODEL INPUT IS: \n{}".format(model_input))
                print("WE ARE IN THE BOTTOM GENERATION PART")
                print("MODEL OUTPUT IS: \n{}".format(output))
                model_thinking = extract_model_thoughts(output)
                # print("MODEL THOUGHTS ARE: \n{}".format(model_thinking))
                extracted_function = extract_single_function_prompt(output)
                # print("EXTRACTED FUNCTION IS {}".format(extracted_function))
                if extracted_function is None or extracted_function == "":
                    # print("EXTRACTED FUNCTION IS NONE")
                    # print("OUTPUT IS {}".format(output))
                    break
                if not "prompt" in extracted_function:
                    raise KeyError("Prompt")
                break # if we made it here then we extracted a function without error. break for loop and move on
            except Exception as E:
                exception_message = f"""\nAttempted to generate instruction for function call, did not receive a valid response. {traceback.format_exc()}. \n Please try again\n"""



    return extract_response_after_thinking(output), str(chat_function_return), model_input
        
    

        

if __name__=="__main__":
    # mixtral
    # MODELS DECLARATION
    model_name = "../LLM_Models/Mistral/OpenHermes-2.5-Mistral-7B"    # Path to chat model and 
    # model_name = "../LLM_Models/Mistral/Mistral-7B-Instruct-v0.2"    # Path to chat model and 
    # model_name = "../LLM_Models/Mixtral/Mixtral-8x7B-Instruct-v0.1"            # Text model and 
    print("loading tokenizer")
    tokenizer = AutoTokenizer.from_pretrained(model_name, local_files_only=True)
    # https://github.com/chujiezheng/chat_templates
    # if using MISTRAL INSTRUCT, which does not accept a system prompt, use this instead?
    # print('###### Corrected Chat Template ######')
    # chat_template = open('./chat_templates/mistral-instruct.jinja').read()
    # chat_template = chat_template.replace('    ', '').replace('\n', '')
    # tokenizer.chat_template = chat_template

    tokenizer.pad_token = tokenizer.eos_token

    embedding_model_name = "Embedding_Models/instructor-xl"         # path embedding model
    print("loading model")

    model = AutoModelForCausalLM.from_pretrained(model_name, local_files_only=True, device_map="auto")
    print("MODEL LOADED")

    # generation_func = partial(generate_function_call, model=model, tokenizer=tokenizer)

    # prompts = [
    #     "give me a fantasy book recommendation and then give me a good joke",
    #     "load these files using the test_functions module ['test1.csv', 'test2.csv', test6.csv']",
    #     "Load this text file [./B_Cell_Model/B Cell Abm Netlogo Notes.txt]. It is a description of an implementation of a simlation. Then print the text, and summarize the meaning of the text"
    # ]


    # for prompt in prompts:
    #     # completion = generate_function_call(prompt, model, tokenizer)
    #     # functions = extract_function_calls(completion)

    #     completion, function_calls = generate_response_with_function_calls(prompt, model, tokenizer)
    #     print("TOTAL GENERATION TIME IS {:.2f}".format(time.time() - start))
    #     print("THIS IS THE COMPLETED RESPONSE FROM THE MODEL\n\n{}".format(completion))
    #     print("\n\n\n")
    #     print("THESE ARE THE FUNCTION CALLS")
    #     print(function_calls)
    import time

    chat_max_length = 1
    for chat_num in range(chat_max_length):
        start = time.time()
        # prompt = input("user: ")
        # prompt = "I really like rap music and I want to learn new songs. I wanna hear a new joke too."
        prompt = "Load this file, give me a summary of its contents then tell me the exact length of its contents. ./B_Cell_Model/B Cell Abm Netlogo Notes.txt"   # len is 2704
        if prompt == "quit":
            break
        output, function_calls, model_input = chat_with_function_calling(prompt, model, tokenizer)
        print("WITH INPUT TO THE MODEL AS: ")
        print(model_input)
        print("-"*100)
        print("THE FINAL OUTPUT OF THE MODEL IS: ")
        print(output)
        # print("-"*100)
        # print("WE CALLED THESE FUNCTIONS")
        # print(function_calls)
        print("TOTAL GENERATION TIME IS {:.2f}".format(time.time() - start))
        print("%"*100)
