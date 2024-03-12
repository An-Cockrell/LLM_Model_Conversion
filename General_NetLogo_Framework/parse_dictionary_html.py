from html.parser import HTMLParser
import re

class MyHTMLParser(HTMLParser):
    capture_letter_category_flag = False
    record = False
    final_output = {}
    entry_index = 0
    output_buffer = ""
    entry_name = None
    letter_name = None
    next_entry_name = None
    next_letter_name = None
    end_example = False

    def handle_starttag(self, tag, attrs):

        if tag == "div":
            if len([tup for tup in attrs if tup[0] =="class" and tup[1]=="dict_entry"]) >0:
                self.next_entry_name = [tup for tup in attrs if tup[0] == "id"][0][1]


                if self.record:
                    if self.letter_name is None:
                        self.letter_name = self.next_letter_name
                    if self.entry_name is None:
                        self.entry_name = self.next_entry_name
                    # removes all the extra whitespace/blank lines
                    self.output_buffer = re.sub(r'\s+', ' ', self.output_buffer).strip()

                    # removes extra text from the next section header
                    self.output_buffer = re.sub(r"\[\('id',.*", '', self.output_buffer).strip()
                    
                    # removes leading text (like name and since version number)
                    self.output_buffer = re.sub(r".*?(\[\('class', 'prim_example'\)\])", r'\1', self.output_buffer).strip()
                    
                    # gets the example 
                    self.output_buffer = re.sub(r"\[\('class',\s*'prim_example'\)\](.*?)EXAMPLE OVER NOW", r'usage: \1\n', self.output_buffer).strip()


                    # removes all remaining HTML blocks
                    # self.output_buffer = re.sub(r"\[\('(.+?)'\)\]", '', self.output_buffer).strip()
                    
                    # self.output_buffer =  re.sub(r'(?<!\n)\n(?!\n)', '', self.output_buffer)
                    entry = {
                                "Letter": self.letter_name,
                                "Entry Name": self.entry_name,
                                "Entry Data": self.entry_name + "\n" + self.output_buffer
                             }
                    self.final_output[self.entry_index] = entry
                    self.entry_index += 1


                    print("Flushing output")
                    print("Letter: " + str(self.letter_name))
                    print("Entry Name: " + str(self.entry_name))
                    # print(self.output_buffer)
                    print(entry["Entry Data"])
                    self.output_buffer=""
                    self.entry_name = self.next_entry_name
                    self.letter_name = self.next_letter_name

                if self.record == False:
                    self.record=True
            
                # self.entry_name = [tup for tup in attrs if tup[0] == "id"][0][1]
                # print("ENTRY NAME " + str(entry_name))

        if self.record:
            # self.output_buffer += "</"+tag +"> " + str(attrs)

            if len(attrs) > 0:
                # self.output_buffer += str(attrs)
                if attrs[0][0] == "id":
                    self.output_buffer += str(attrs)
                    # self.output_buffer += str(attrs)
                    # print("Encountered a start tag:", tag)
                    pass
                if attrs[0][1] == "prim_example":
                    self.output_buffer += str(attrs)
                    pass

        
        if tag == "h2":
            # print("HEADER 2")
            # print(attrs)
            if len(attrs)<1:
                # print("Header has no entry")
                return
            self.next_letter_name = attrs[0][1]

    def handle_endtag(self, tag):
        # print("Encountered an end tag :", tag)
        # if capture_letter_category_flag:
        #     letter_category = tag
        if self.record:
            if tag == "span":
                self.end_example = True
                # self.output_buffer += "THIS IS THE END OF THE EXAMPLE"
        pass

    def handle_data(self, data):
        # print("Encountered some data  :", data)
        if self.record:
            self.output_buffer += data 
            if self.end_example:
                self.output_buffer += "EXAMPLE OVER NOW"
                self.end_example = False

    def save(self):
        import json
        # Serializing json
        json_object = json.dumps(self.final_output, indent=4)
        
        # Writing to sample.json
        with open("dictionary_entries.json", "w") as outfile:
            outfile.write(json_object)


parser = MyHTMLParser()

with open("dictionary_entries.html", "r") as file:
    context = file.read()
    # context = context[:10000]
    parser.feed(context)
    parser.save()


