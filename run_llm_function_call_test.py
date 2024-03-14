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
from langchain.pydantic_v1 import BaseModel, Field, validator
from langchain.chains.openai_functions import convert_to_openai_function


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
    
    
def generate_response(prompt, model, tokenizer, generation_config_overrides={}):
    fn = """{"name": "function_name", "arguments": {"arg_1": "value_1", "arg_2": value_2, ...}}"""
    prompt = f"""<|im_start|>system
You are a helpful assistant with access to the following functions:

{convert_to_openai_function(Joke)}

{convert_to_openai_function(BookRecommendation)}

{convert_to_openai_function(SongRecommendation)}

To use these functions respond with:

     {fn} 
     {fn} 
    ...


Edge cases you must handle:
- If there are no functions that match the user request, you will respond politely that you cannot help.<|im_end|>
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
        generated_tokens = model.generate(**inputs, generation_config=generation_config)


    output = tokenizer.decode(
        generated_tokens.squeeze()[n_tokens:], skip_special_tokens=False
    )
    print(output)
    print("INISIDE THE FUCNCTION^^^")
    return output

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
    # print('###### Corrected Chat Template ######')
    # chat_template = open('./chat_templates/mistral-instruct.jinja').read()
    # chat_template = chat_template.replace('    ', '').replace('\n', '')
    # tokenizer.chat_template = chat_template


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
        print(completion)
        print("STARTING EXTRACTION NOW")
        functions = extract_function_calls(completion)

        if functions:
            print(functions)
        else:
            print(completion.strip())
        print("="*100)

