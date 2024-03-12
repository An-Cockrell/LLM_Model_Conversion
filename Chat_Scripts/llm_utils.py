import torch
import numpy as np
from sklearn.metrics.pairwise import cosine_similarity



def print_cuda_usage(optional_str = None) -> None:
    total = 0
    for device in range(torch.cuda.device_count()):
        total += torch.cuda.max_memory_allocated(device)
    if optional_str is not None:
        print(str(optional_str), end=" ")
    print("cuda usage gb: {:.2f}".format(total/ 1024 / 1024 / 1024), end="\n\n")
    torch.cuda.reset_peak_memory_stats()



def get_relevant_prompts(prompt, embedding_encoder, num=1, mode=None, context_embeddings=None, context_plain_text_dict=None) -> "tuple[list[str], list[int]]":
    # context embeddings is numpy array of pre-encoded prhrases
    # if num is 0, return empty array. should try to avoid calling it when you dont want it....
    if num==0:
        return [], []

    assert mode in ["code", "dictionary", "example", None], print("Invalid mode. Valid modes are : " + "[\"code\", \"dictionary\", \"example\", None]")
    if mode == "code":
        query = [['Represent the chat prompt for retrieving relevant code samples: ', prompt]]
    if mode == "dictionary":
        query = [['Represent the chat prompt for retrieving relevant documentation dictionary samples: ', prompt]]
    if mode == "example":
        query = [['Represent the chat prompt for retrieving relevant programming examples in Netlogo: ', prompt]]
    if mode is None:
        query = [['Represent the chat prompt for retrieval: ', prompt]]

    embedding = embedding_encoder.encode(query)

    all_relevant_entry_ids = cosine_similarity(embedding, context_embeddings)
    # https://stackoverflow.com/questions/6910641/how-do-i-get-indices-of-n-maximum-values-in-a-numpy-array
    relevant_entry_ids = np.argpartition(all_relevant_entry_ids.flatten(), (-1*num))[(-1*num):].astype(int)

    relevant_phrases = []
    for index in relevant_entry_ids:
        relevant_phrases.append(context_plain_text_dict[str(index)][1])


    return relevant_phrases, relevant_entry_ids