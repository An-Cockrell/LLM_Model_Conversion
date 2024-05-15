### Introduction

The architecture of this simulation model is inspired by NetLogo, a programmable modeling environment used for simulating complex agent-based models. This framework is designed to facilitate the creation and management of agents in a simulated world, allowing exploration of complex behaviors and interactions within a grid-based environment. The core components of this architecture are the `World`, `Agent`, `Patch`, and `Turtle` classes. The `World` class acts as the central coordinator of the simulation. Developers should use these existing classes without extending or deriving from them, as they provide all necessary functionalities for developing simulations.

### Architecture Components

1. **Agent (Abstract Class)**:
   The `Agent` class is an abstract base class that provides foundational attributes and functionalities for all agents within the simulation, both stationary and mobile. Attributes like position (x, y), color, and opacity are defined here, along with accessors and mutators for these properties. This class is not to be extended directly; use the `Patch` or `Turtle` classes instead.

2. **Patch (Final)**:
   Derived from the `Agent` class, `Patch` represents a stationary agent modeling a segment of the environment. Each `Patch` is a cell in the simulation grid and can hold environmental variables (e.g., temperature, humidity) and track mobile agents (`Turtle` objects) located within it.

3. **Turtle (Final)**:
   Extending the `Agent` class, `Turtle` represents mobile agents. Turtles can move across patches, interact with other turtles, and modify environment variables. To manage different breeds of turtles, they are tracked separately in the `World` class as distinct vectors of `Turtle` pointers.

4. **World (Final)**:
   The `World` class is responsible for handling the entire simulation. It maintains a grid of `Patch` objects and manages collections of `Turtle` instances. It is crucial for initializing the simulation environment, executing simulation steps, and tracking interactions. To create new breeds of turtles:
   - **Create a vector of `shared_ptr<Turtle>` for each new breed.**
   - **Add instances of turtles to the corresponding breed vector and to the simulation.**

### Functionality and Interactions

- **Initialization**:
  The simulation starts with the `World` initializing patches using the `setup_patches` function based on the grid size and populating the environment with Turtle agents.

- **Creation and Registration of New Turtle Breeds**:
  When introducing a new type of turtle, follow these steps:
  1. **Define a Tracking Vector**: Create a vector to track the specific breed, using `shared_ptr<Turtle>`.
  2. **Instantiate and Register**: When you create a new turtle, add it to the appropriate tracking vector and also to a general `all_turtles` vector using a `weak_ptr`.
  3. **Place Turtle in Simulation**: Use the `Patch::add_turtle` function to add the new turtle to its corresponding patch.

  Here’s an example of adding a new turtle breed, termed as 'rock':
  ```cpp
  std::shared_ptr<Turtle> World::create_rock_turtle(int x, int y, int id, int heading=0){
      auto rock = std::make_shared<Turtle>(x, y, id); // Create the rock turtle
      rock.setLabel("rock"); // labeling the specific agent type
      Patch& patch = get_patch(x, y); // Locate the patch to add the turtle
      std::weak_ptr<Turtle> rock_weak_ptr = rock; // Create a weak pointer for the all_turtles vector

      all_turtles.push_back(rock_weak_ptr); // Add weak pointer to all_turtles
      all_rocks.push_back(rock); // Add shared pointer to the turtle tracking vector
      patch.add_turtle(rock); // Add turtle to the patch

      return rock; // Return the pointer to the turtle
  }
  ```




### Agent Actions and World Interactions

The `World` class functions as the orchestration layer for the simulation, centralizing the control over agent behaviors and environmental interactions. This design approach ensures that all significant changes to the simulation state go through this class, making it a focal point for new developments.

#### Handling Agent Behaviors

Agent-specific actions, such as movement or executing particular behaviors, are managed by methods within the `World` class. These methods are designed to take a pointer to an agent—specifically, a `Turtle`—as an argument. The `World` class uses details from the `Turtle` to execute appropriate actions. This centralized handling ensures that all agent interactions are consistent with the simulation's rules and logic.

**Key Implementation Note:**
All functions where a `Turtle` interacts with the environment or other agents must be implemented as methods on the `World` class that accept references to pointers (or smart pointers) to the `Turtle`. These methods are responsible for manipulating the simulation state, thus ensuring that agents interact with the environment and each other according to the defined simulation rules.

Example of a movement function in the `World` class:
```cpp
// Moves a turtle within the world, considering capacity of target patches.
void World::move_turtle(std::shared_ptr<Turtle> &turtle){
  // moves the turtle by asking the turtle to calculate its movement, and if there is space on the target_patch, the world executes the move
  Patch &turtle_current_patch = get_patch(turtle->getX(), turtle->getY());
  auto new_coords = turtle->move(); //moving along turtle.heading for the default value of 1 unit
  Patch& target_patch = get_patch(new_coords.first, new_coords.second);

  if (turtle_current_patch == target_patch){
    return;
  } else if (target_patch.is_at_capacity()){
    turtle->execute_move(false);
  }
  else{
    turtle->execute_move(true);
    turtle_current_patch.remove_turtle(turtle);
    target_patch.add_turtle(turtle);
  }
}
```

#### Environmental Processes

The `World` class also oversees environmental processes such as diffusion and evaporation, which affect the environment variables stored within each `Patch`. Methods controlling these processes are encapsulated within the `World` class, ensuring that environmental changes are centrally managed and uniformly applied across the simulation grid.

#### Simulation Steps

Each simulation tick, or step, involves updating the states of agents based on their individual behaviors and interactions with their environment. The `World` class coordinates these updates, ensuring that each agent acts according to its defined behavioral rules. This coordination is typically triggered by a single call to a method akin to the NetLogo `go` function, which might look something like this:

```cpp
void World::go() {
    // NetLogo "tick" equivalent in C++ is simply incrementing the time counter
    step++;

    // implement netlogo Go function here
    // for (auto& turtle : all_turtle_type_1) {
    //     turtle_type_1_function(turtle);
    // }

    // end of go
    updateTurtleVectors(); // need to update turtle positions/delete dead turtles
}
```

In this function, `turtle_type_1_function` would handle the behaviors of each `Turtle` in the vector `all_turtle_type_1`, while `update_environment` would manage environmental processes like diffusion and evaporation.

