from Chat_Scripts.generate_headers_updating import generate_headers_updating
from Chat_Scripts.describe_netlogo_as_header import describe_netlogo_as_header

if __name__=="__main__":
    # mixtral
    # MODELS DECLARATION
    model_name = "LLM_Models/Mixtral/Mixtral-8x7B-Instruct-v0.1"    # Path to chat model and 
    embedding_model_name = "Embedding_Models/instructor-xl"         # path embedding model


    describe_netlogo_as_header(model_name=model_name, save_name="describe_netlogo_as_headers_mixtral")

    generate_headers_updating(model_name=model_name, save_name="generate_headers_updating_mixtral")