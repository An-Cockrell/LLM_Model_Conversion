import json
import re

file_path = "LLM_output_files/headers_generated_deepseek8_2"
with open(file_path + ".json","r") as f:
    headers = json.load(f)

for query_num in headers.keys():
    netlogo = headers[query_num]["netlogo_input"]
    response = headers[query_num]["header_response"]

    print("\n\n\n\n\n{}\n--------------------------------------".format(query_num))
    print("NETLOGO INPUT: \n{}\n\n".format(netlogo))
    # print(response)
    regex = r'```([^`]*)```?'
    # Searching in the first text
    matches1 = re.search(regex, response)
    if matches1:
        print("extracted code:\n", matches1.group(1).split("\n", 1)[1])
    else:
        print("Raw Model Response:\n{}".format(response))
    # break
    valid: bool = input() == "y"
    if valid:
        headers[query_num]["extracted_code"] = str(matches1.group(1).split("\n", 1)[1])
    else:
        headers[query_num]["extracted_code"] = ""

    print(headers[query_num]["extracted_code"])
    with open(file_path + "_manually_verified.json", "w") as f:
        json.dump(headers, f, indent=6)