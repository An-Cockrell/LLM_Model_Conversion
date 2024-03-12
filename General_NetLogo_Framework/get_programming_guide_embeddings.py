# https://huggingface.co/hkunlp/instructor-xl


import time
from InstructorEmbedding import INSTRUCTOR
print("Loading model")
start = time.time()
model = INSTRUCTOR('../../Embedding_Models/instructor-xl')
print("model loaded")
print("Time taken: " + str(time.time() - start))

import json

with open("programming_entries.json", "r") as f:
    data = json.load(f)
    # print(data)

    text_instruction_pairs = []
    for entry_num in data.keys():
        entry_name = data[entry_num]["Section_title"]
        entry_data = data[entry_num]["Section_data"]
        # print(entry_data)
        text_instruction_pairs.append({"instruction": "Represent the Documentation Section for retrieval:", "text": "Section Name: " + entry_name + " \nEntry Data: " + entry_data})
    
        # print(text_instruction_pairs)
    texts_with_instructions = []
    for pair in text_instruction_pairs:
        texts_with_instructions.append([pair["instruction"], pair["text"]])
    import numpy as np
    print(np.array(texts_with_instructions))
    print("Number of input samples")
    print(np.array(texts_with_instructions).shape)
     
    print("getting embeddings")
    start = time.time()        
    customized_embeddings = model.encode(texts_with_instructions)
    #print(np.array(customized_embeddings))
    print("Embeddings shape")
    print(np.array(customized_embeddings).shape)

    print("got Embeddings")
    print("Time taken: " + str(time.time() - start))
    import numpy as np
    np.save("Netlogo_programming_guide_embeddings", customized_embeddings)

