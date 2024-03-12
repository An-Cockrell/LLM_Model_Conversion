# ﻿B Cell Abm C++ Notes

This is a C++ implementation of the netlogo b_cell_abm created by Bryan Shin. This implementation was written by Dale Larie with the help of chatGPT. It uses SDL2 from graphical representation of the model, and it renders in real time. It can be paused by pressing the space bar.


### General Info
Structurally, this was written from the ground up to first copy as much functionality as needed from Netlogo, then creating C++ functionality that is specific to this implementation. This is set up with a little bit of polymorphism, as netlogo has a bit, but a lot of the functionality is managed through compositional programming (basically instead of a function being part of a class, most of the time a class is just handed to a function and it is handled outside the class definition).

First of all, everything is an Agent. Agents have a few parameters that everyone will have, such as an X and Y coordinate and colors etc. There is also the `angle_to` function, that will find a current agent’s angle on the global coordinate system to another agent. 
There are two main types of agents, similar to netlogo, Patches and Turtles. Patches are the immobile agents that make up the world. Patches are in charge of environmental variables, and anything that diffuses or is read across multiple turtles. Turtles are the mobile cells in an experiment, and are the things that are extended to be cell types.


The general structure of this program is that everything is controlled through the World. Patches and turtles have some variables that they track for themselves, but lists of turtles and patches are implemented as vectors of pointers on World. Additionally, cell specific functions are generally not part of the implementation of the specific cell class, but instead are methods on World that take a pointer to a cell type as an argument.  Originally this choice was made because of circular dependencies with `#include` where individual turtles would need to know about the lists they are part of, which would mean `#including world`, and the world would need to know about turtle types to make the lists, which would mean `#including turtles`, and would cause problems at compile time. 

Basically, any simulation function where a turtle interacts with its environment (reading values on patches, moving, linking to other turtles, etc) should be a part of World and take in pointers to turtles/patches or something to get pointers (e.g. coordinates) as arguments.


As mentioned above, objects are mainly created and kept track of using pointers. This is the magic part of C++ and makes it super fast, but you need to be careful not to introduce any memory leaks by losing track of objects being pointed to, or deleting objects and not pointers to them and getting seg faults. This is especially important if we are changing the breed of a cell, because it’s easy to create the new cell but not properly kill the old cell and have a leak. To do this we are using smart pointers in a couple ways. First, vectors of cells are managed with vectors of `std::shared_ptr<CELL_TYPE>`. This allows us to have a list with dynamic length of the various cell types we have. In addition, in order to have a list of just all turtles (basically everything not including patches) we have a vector of `std::weak_ptr<Turtle>`. These are stored as weak pointers so that having them in the list of everything does not increment that object’s reference count and helps make sure that objects are deleted and managed properly when they are removed from their type specific list. 


### Creating new Turtles
In order to add a new turtle properly it needs to be created and added to 3 lists, the list of this cell type, a weak pointer to the list of all turtles, and then added to the list of cells on a particular patch where the turtle is to be added. An example of this can be seen in `naive_b_cell.cpp` in the `spawnBCell()` function. A new naive b cell and pointer to it is created with this line;

```
auto b_cell = std::make_shared<NaiveBCell>(cell_x, cell_y, global_ID_counter++, 180);
```

where the auto keyword makes b_cell of type `std::shared_ptr<NaiveBCell>`, which is just a smart pointer to a new NaiveBCell object. Some variables are set on it, such as color and shape, then it is added to a list of naive_b_cells with this line;

```
all_naive_b_cells.push_back(b_cell);
```

A weak pointer is then created and added to the list of turtles in these two lines:

```
std::weak_ptr<Turtle> b_cell_weak_ptr = b_cell; // creating the weak pointer 
all_turtles.push_back(b_cell_weak_ptr); // adding weak pointer to list of all turtles
```

Finally, the original shared pointer is added to the patch where the new cell is being created:
```
get_patch(b_cell->getX(), b_cell->getY()).add_turtle(b_cell);
```
These are the three places that a turtle MUST be added to upon creation (and the only places it should be added on creation). If not, then either the turtle won't be called to do its functions, or it will end up as an immobile turtle on the screen when rendering, or the program will throw an error when it tries to move the turtle.

### Changing turtle breed
Because of the differences in languages between netlogo and c++, changing the breed of a turtle cant really be done to the original turtle object, and needs to be handled manually. 

Instead, a new object of the desired breed should be created, have its attributes copied from the original turtle and placed in the appropriate lists, then the original turtle should be killed and deleted. An example of this is in the `naiveBCellFunction()`. If a Bcell becomes activated, in the original netlogo its breed is just set to activated, and its basically ready to go. In this implementation however, there are a few more steps. First a new activated_b_cell is created based on some attributes of the original cell, then the rest of the attributes are copied from the original cell:
```
auto new_activated_b_cell = std::make_shared<ActivatedBCell>(naive_b_cell->getX(), naive_b_cell->getY(), global_ID_counter++, naive_b_cell->getHeading());
new_activated_b_cell->copy_other_turtle_attributes(naive_b_cell);
```

Then, same as before, the new cell is added to the regular list of that cell type, a weak pointer is added to the list of all cells, and then the new cell is added to the patch. The biggest difference is that the original cell is killed, and then we return from the function immediately. We must return immediately because the cell that was originally performing the function no longer exists, and if it is accessed will cause a seg fault or undefined behavior. Here is a code example that does this. 

```      
all_activated_b_cells.push_back(new_activated_b_cell);
std::weak_ptr<Turtle> new_activated_b_cell_weak_ptr = new_activated_b_cell;
all_turtles.push_back(new_activated_b_cell_weak_ptr);
get_patch(new_activated_b_cell->getX(),new_activated_b_cell->getY()).add_turtle(new_activated_b_cell);
kill(naive_b_cell);
return;
```

(I tried just re-declaring the original cell name as the new cell type and continuing with the function, and that worked actually, the new cell would do the rest of the function (moving/turn into breg maybe), but it would increase the reference count to the original cell and would cause a memory leak by leaving untracked cells in existence.)


### Killing cells
Because of the overhead required to remove an element from the middle of a list, objects are not removed from lists right when they are killed. When objects are managed properly, then memory is freed  when `updateTurtleVectors()` is called during `go()`. This is intentional because it reduces overhead by removing all the now dead turtles in one shot. All turtles should be killed by passing a std::shared_ptr to the cell as an argument to the templated function `kill` that is part of World. This will remove any linked turtles, add the turtle to a list of turtles to remove during updateTurtleVectors, then set the turtle is_Alive variable to be not alive, remove turtle from the patch, then reset references to it. As long as a pointer to the turtle is not created anywhere else, this should properly kill and free the memory associated with that turtle.