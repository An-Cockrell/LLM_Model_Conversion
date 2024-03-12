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
        for entry_num in data.keys():
            section_title, section_data = list(data[entry_num].keys())[-2:]
            entry_name = data[entry_num][section_title]
            entry_data = data[entry_num][section_data]
            # print(entry_name)
            # print(entry_data)
            if instruction is None:
                instruction = ["Represent the Documentation Section for retrieval:", "Here is a section of relevant information"]
            text_instruction_pairs.append({"instruction": instruction[0], "text": str(instruction[1]) +"Section Name: " + entry_name + " \n" +  "\nEntry Data: " + entry_data})
    
        texts_with_instructions = []
        for pair in text_instruction_pairs:
            texts_with_instructions.append([pair["instruction"], pair["text"]])
    
    return texts_with_instructions


if __name__ == "__main__":
    json_files = [
	 "LLM_Model_Conversion/General_NetLogo_Framework/dictionary_entries.json",
	 "LLM_Model_Conversion/General_NetLogo_Framework/programming_entries.json",
	 "LLM_Model_Conversion/B_Cell_Model/header_files.json",
	 "LLM_Model_Conversion/B_Cell_Model/b_cell_functions.json"
]
    instruction_set = [
        ["Represent this documentation section of official Netlogo documentation for retrieval:","Here is a relevant example entry from the netlogo dictionary:\n"],
        ["Represent this documentation section of Netlogo programmer's guide for retrieval:","Here is a relevant example from the netlogo programmer's guide:\n"],
        ["Represent this header file from a custom port of netlogo to C++ for retrieval:","Here is a block of relevant header file code:\n"],
        ["Represent this C++ function from a custom port of Netlogo for retrieval:", "Here is a block of relevant code from the C++ implementation:\n"]
    ]

    texts_with_instructions = []
    texts_with_instructions_dict = {}
    counter = 0
    for json_file, instruction in zip(json_files, instruction_set):
        for instruction_text_pair in unpack_json(json_file, instruction):
            texts_with_instructions.append(instruction_text_pair)
            texts_with_instructions_dict[counter]  = instruction_text_pair
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
    np.save("LLM_Model_Conversion/Context_Data_Files/Relevant_context_embeddings", customized_embeddings)
    with open("LLM_Model_Conversion/Context_Data_Files/context_plain_text.json", "w") as f:
        json.dump(texts_with_instructions_dict, f, indent=6)

