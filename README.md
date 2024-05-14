# LLM_Model_Conversion
## Installation notes

In order to run the LLM model interaction, you must install the python conda environment and activate it. Create the env with all its dependencies by running .`conda env create -f LLM_Environment.yml` at the command line.

Activate it by running `conda activate LLM`


In order to render the simulation graphically so that you can see what is happening, this uses SDL2.
The simulation can be paused by focusing on the simulation graphics window and pressing spacebar.
To change the render size, change the `GRID_SIZE` parameter in `rendering.h` to be smaller for a smaller rendering, or larger for a bigger one.


This implementation requires SDL2 to be installed. This is a graphics rendering library for C++.
It can be installed with the following commands. Instructions were taken from https://wiki.libsdl.org/SDL2/Installation
```
git clone https://github.com/libsdl-org/SDL.git -b SDL2
cd SDL
mkdir build
cd build
../configure
make
sudo make install
```


## Running the LLM
On a machine with apropriate resources, you should be able to interact with the LLM for chatting by simply running `python3 run_llm_chat`, or you can attempt to use a version more finely tuned for converting and incorporating netlogo code into the C++ framework.

To attempt to convert a model, first start by copying the `base_implementation` folder into a new folder in Model_Implementations, named whatever you want to name the new model. 
Next, make sure to change the save name in the `run_llm_conversion_chat.py` file to the same as the new folder you just made.

The LLM Model requires files to be named in a specific format. The main files that the end user will be editing will be `world.h` for variable and function declarations, and `world_custom_implementations.cpp` for implementations of those functions and other code.

Compile and implementation of your translated model by going into the folder for the new model and entering  `source ./compile.sh` at the command line to compile, and and run using `./test.o`.

## Model Conversion and interaction
Interacting with the model is semi-conversational, usually something like a small instruction followed by a block of netlogo to translate. Type your response into the command line interface, and hit enter as many times as you want. You can also paste text in no problem. However, backspace deleting doesnt seem to always work, especially if the user prompt goes onto multiple lines.
To generate a model response, type `generate` and hit enter, and the model will start responding.

After the model responds with some code, it should also tell you roughly where to place that code in the codebase. This part is manual, but copy and paste the model's reponse into the codebase to implement its suggestions.
The model only reads certain parts of the codebase, namely the header files `"parameters.h", "agent.h", "patch.h", "turtle.h", "world.h"` and the .cpp file `world_custom_implementations.cpp`. 
It is important that the LLM has some of the .cpp implementations to base its code style and conventions off of, but at the same time when the file gets too long, it can slow down responses and degrade performance. I will sometimes copy code that is no longer relevant to future generations (the implementation of the `diffuse` function for example) and move it to another file, either putting it at the bottom of `world.cpp` or making another file called `world_custom_implementations2.cpp` (if you make another file, make sure to put `#include 'world.h'` at the top!)

chat history is only about 5 responses long, so it definitely drifts, and sometimes needs to be corrected. Sometimes it drifts way too far to be useful, and its easier to delete the chat history and start over. The chat history is stored at `/LLM_output_files/{save_file_name}`

## Style
One of the biggest parts of this is that everything is managed by the world class. Turtles are passed as arguments to functions to have the turtle execute the function inside the world. When you prompt the model and you want a particular turtle to perform an action, when you write the action function tell the model that the function should take a `shared_ptr` to a turtle.
Additionally, when defining new breeds, you must create a helper function to create a new turtle and add it to a tracking vector for the apropriate type. Examples of this are shown in `Model_Implementations/hand_written_example/` and in the [developer guide](https://github.com/An-Cockrell/LLM_Model_Conversion/blob/main/General_NetLogo_Framework/General_Netlogo_C%2B%2B/Developer_guide.md)

Also, read the model responses! Its often incorrect or assumes something, and needs to be corrected. Telling it what is wrong usually leads to a good response, and it works the second time, but not always.
Compile often to test and make sure that added code doesnt break things, and good luck!


##
https://pytorch.org/docs/stable/generated/torch.nn.functional.scaled_dot_product_attention.html
get a futureWarning when using 
`    with torch.no_grad(), torch.backends.cuda.sdp_kernel(enable_flash=True, enable_math=False, enable_mem_efficient=True):`
but according to the docs, it seems this is the correct useage?



