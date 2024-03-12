# LLM_Model_Conversion

some helpful advice I found in a comment on someone elses coding assistant project
https://news.ycombinator.com/item?id=35970653
"""Seems like the wrong approach in the long term.
Realistically, a large code base LLM generation tool is going to look something like old-school C code.

An initial pass will generate an architecture and a series of independent code unit definitions (.h files) and then a 'detail pass' will generate the code for (.c files) for each header file.

The header files will be 'relatively independent' and small, so they fit inside the context for the LLM, and because the function definition and comments 'define' what a function is, the LLM will generate consistent multi-file code.

The anti-patterns we see at the moment in this type of project are:

1) The entire code is passed to the LLM as context using a huge number of tokens meaninglessly. (you only need the function signature)

2) 1-page spaghetti definition files are stupid and unmaintainable (just read prompt.md if you don't believe me).

3) No way of verifying the 'plan' before you go and generate the code that implements it (expensive and a waste of time; you should generate function signatures first and then verify they are correct before generating the code for them).

4) Generating unit tests for full functions instead of just the function signatures (leaks implementation details).

It's interesting to me that modern language try to move all of these domains (header, implementation, tests) into a single place (or even a single file, look at rust), but passing all of that to an LLM is wrong.

It's architecturally wrong in a way that won't scale.

Even an LLM with 1000k tokens that could process a code base like this will be prohibitively slow and expensive to do so.

I suspect we'll see a class 'generative languages' emerge in the future that walk the other direction so they are easier to use with LLM and code-gen."""


https://pytorch.org/docs/stable/generated/torch.nn.functional.scaled_dot_product_attention.html
get a futureWarning when using 
`    with torch.no_grad(), torch.backends.cuda.sdp_kernel(enable_flash=True, enable_math=False, enable_mem_efficient=True):`
but according to the docs, it seems this is the correct useage?