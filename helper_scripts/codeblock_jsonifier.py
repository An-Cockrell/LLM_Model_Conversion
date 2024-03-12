import json
import re

if __name__=="__main__":
    loop = True

    output_file = "general_cpp_parsed.json"

    print("Enter a string")
    full_text = ""
    counter = 0
    final_output = {}
    while loop:
        text = input()
        if text =="quit":
            break
        elif text=="done":
            raw_text = r"{}".format(full_text).replace("\"", "\'")
            print(raw_text)
            char_index = raw_text.find("{")
            
            # function_name = raw_text[2:char_index]
            # full_function = raw_text[2:]
            # function_dict = {"Function_name": function_name,
            #                  "Function_body": full_function}
            # pattern = r'#ifndef\s+(\w+)'
            # match = re.search(pattern, raw_text)
            # header_file_name = match[1]
            function_dict = {
                            "code_block":raw_text
                            }

            print(function_dict)
            code_base = None
            with open(output_file, "r") as f:
                # json.dump(function_dict)
                code_base = json.load(f)
                counter = len(code_base.keys())
            with open(output_file, "w") as f:
                code_base.update({str(counter) : function_dict})
                json.dump(code_base, f, indent=6)
                counter += 1


            # print("Raw text")
            # with open("test.txt", "w") as myFile:
            #     myFile.write(raw_text)
            full_text = ""
        else:
            full_text = full_text + "\n" + text 
