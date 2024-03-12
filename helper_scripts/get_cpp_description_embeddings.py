# https://huggingface.co/hkunlp/instructor-xl


import time
import json
import numpy as np

def unpack_json(json_file=None, instruction=None):
    # print(json_file, instruction)
    if json_file is None:
        print("No file entered.")
        return
    with open(json_file, "r") as f:
        data = json.load(f)
        # print(data)
        text_instruction_pairs = []
        all_keys = sorted(data.keys())
        for entry_num in all_keys:
            raw_code = data[entry_num]["code"]
            code_desc_long = data[entry_num]["code_description"]
            code_desc_short = data[entry_num]["code_description_short"]
            block_name = data[entry_num]["name"]
            # print(code_desc_short)
            if instruction is None:
                instruction = ["Represent the Documentation Section for retrieval:", "Here is a section of relevant information"]
            text_instruction_pairs.append({"instruction": instruction[0], "text": str(instruction[1]) + ":\n" + code_desc_long, "name":block_name, "code":raw_code, "desc_short":code_desc_short, "desc_long": code_desc_long})
    
        texts_with_instructions = []
        for pair in text_instruction_pairs:
            texts_with_instructions.append([pair["instruction"], pair["text"]])
    
    return texts_with_instructions, text_instruction_pairs


if __name__ == "__main__":
    json_files = [
	 "LLM_Model_Conversion/Context_Data_Files/cpp_described.json",
]
    instruction_set = [
        ["Represent this short description of a cpp codeblock for retrieval:","Here is a description of a block of cpp code"],
    ]

    texts_with_instructions = []
    texts_with_instructions_dict = {}
    counter = 0
    for json_file, instruction in zip(json_files, instruction_set):
        # need to call this raw because texts_with_instructions alrady exists, and appendingto it will make it twice as long
        texts_with_instructions_raw, instruction_text_set = unpack_json(json_file, instruction)
        for instruction_text_pair, entry in zip(texts_with_instructions_raw, instruction_text_set):
            # print(entry)
            # print(instruction_text_pair)
            # appending pair here so that the dictionary is absolutely linked with the dict, every entry is in the same order with correct key-index pairing
            texts_with_instructions.append(instruction_text_pair)
            texts_with_instructions_dict[counter]  = entry
            counter += 1
        # print(json_file, instruction)
        # print(unpack_json(json_file, instruction)[0])


    # print(texts_with_instructions)
    print("Number of input samples")
    print(np.array(texts_with_instructions).shape)

    from InstructorEmbedding import INSTRUCTOR
    print("Loading model")
    start = time.time()
    model = INSTRUCTOR('Embedding_Models/instructor-xl')
    print("model loaded")
    print("Time taken: " + str(time.time() - start))

     
    print("getting embeddings")
    start = time.time()        
    customized_embeddings = model.encode(texts_with_instructions)
    #print(np.array(customized_embeddings))
    print("Embeddings shape")
    print(np.array(customized_embeddings).shape)

    print("got Embeddings")
    print("Time taken: " + str(time.time() - start))
    import numpy as np
    np.save("LLM_Model_Conversion/Context_Data_Files/cpp_long_description_embeddings", customized_embeddings)
    with open("LLM_Model_Conversion/Context_Data_Files/cpp_description_text.json", "w") as f:
        json.dump(texts_with_instructions_dict, f, indent=6)

