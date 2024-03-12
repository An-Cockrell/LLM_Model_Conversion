import json
import re

file_path = "LLM_output_files/headers_generated_deepseek8.json"
with open(file_path,"r") as f:
    headers = json.load(f)

for query_num in headers.keys():
    response = headers[query_num]["header_response"]

    print(response)
    regex = r'```([^`]*)```?'
    # Searching in the first text
    matches1 = re.search(regex, response)
    if matches1:
        print("code:", matches1.group(1))
    # break