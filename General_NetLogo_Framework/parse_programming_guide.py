from html.parser import HTMLParser
import re

class MyHTMLParser(HTMLParser):
    capture_letter_category_flag = False
    start_section_record = False
    final_output = {}
    entry_index = 0
    output_buffer = ""
    entry_name = None
    letter_name = None
    next_entry_name = None
    next_letter_name = None
    section_title = None
    section_output = None

    def handle_starttag(self, tag, attrs):
        # if self.start_section_record:
        #     print("Start  " + str(tag))
                


        # if self.start_section_record:
            # print(attrs)
            # print(tag)
            # self.output_buffer += "</"+tag +"> " + str(attrs)
            # if len(attrs) > 0:
            #     self.output_buffer += str(attrs)
            #     print("Encountered a start tag:", tag)
        
        if tag == "h2":
            print("HEADER 2")
            self.start_section_record = True
            # print(attrs)
            if len(self.output_buffer )> 0:
                section_title_word_index = self.output_buffer.find("\n")
                if section_title_word_index != -1:
                    self.section_title = self.output_buffer[:section_title_word_index]
                self.section_output = {"Section_title" : self.section_title,
                 "Section_data" : self.output_buffer}
                
                self.final_output[self.entry_index] = self.section_output
                self.entry_index += 1
                print(self.section_output)


            self.output_buffer = ""

    def handle_endtag(self, tag):
        # if self.start_section_record:
        #     print("Encountered an end tag :", tag)
        # if capture_letter_category_flag:
        #     letter_category = tag
        # if self.record:
            # self.output_buffer += " <" + tag +"/>" + "\n"
        pass

    def handle_data(self, data):
        # if self.start_section_record:
        #     print("Encountered some data  :", data)
        if self.start_section_record:
            self.output_buffer += data

    def save(self):
        import json
        # Serializing json
        json_object = json.dumps(self.final_output, indent=4)
        
        # Writing to sample.json
        with open("programming_entries.json", "w") as outfile:
            outfile.write(json_object)


parser = MyHTMLParser()

with open("programming.html", "r") as file:
    context = file.read()
    # context = context[:22000]
    parser.feed(context)
    parser.save()


