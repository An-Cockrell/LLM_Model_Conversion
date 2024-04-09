// world.h
#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <memory>
#include <algorithm>

#include "parameters.h"
#include "agent.h"
#include "patch.h"
#include "turtle.h"


/**
 * Class representing the world in which the simulation takes place.
 * This class manages the entire simulation environment, including patches, turtles, and the simulation loop.
 */
class World {
private:
  std::mt19937 RNG_Engine; // Random number generator for the simulation.
  int global_ID_counter;  // Counter to assign unique IDs to new turtles.
  int step; // The current simulation step.
  int time; // The simulation time.
  int loop_run; // The loop run indicator, for controlling simulation loops.
  
  
  Patch all_patches[WORLD_WIDTH][WORLD_HEIGHT]; // Array of patches constituting the world.
  
  // Sets the seed for the random number generator.
  void set_rng_seed(int newSeed) { RNG_Engine.seed(newSeed); }
  
  // Adds a patch to the world at the specified coordinates.
  void add_patch(int x, int y);


public:
  std::vector<std::weak_ptr<Turtle>> all_turtles; // Vector storing all turtles in the world as weak pointers.

  // include vectors of shared_ptr's of other turtle sub-types here
  // for example:   std::vector<std::shared_ptr<Pmn>> all_pmns;

  
  World(){}; // Default constructor.
  
  // Retrieves the patch at the specified coordinates.
  Patch& get_patch(int x, int y);
  // Retrieves the patch located ahead of a given turtle.
  Patch& getPatchAhead(std::shared_ptr<Turtle> turtle, float distance=1);
  // Retrieves the patch located ahead and to the right of a given turtle.
  Patch& getPatchAheadRight(std::shared_ptr<Turtle> turtle, float distance=1);
  // Retrieves the patch located ahead and to the left of a given turtle.
  Patch& getPatchAheadLeft(std::shared_ptr<Turtle> turtle, float distance=1);


  // Initializes patches in the world.
  void setup_patches();
  // Displays all patches in the world.
  void display_patches();
  
  // Adds a turtle to the world at the specified coordinates.
  void add_turtle(int x, int y, int id, int heading=0);
  // Moves a turtle according to its current heading.
  void move_turtle(std::shared_ptr<Turtle> turtle);
  // Moves a turtle to a random location.
  void move_turtle_random_jump(std::shared_ptr<Turtle> turtle);
  // Causes a turtle to wiggle in place.
  void turtle_wiggle(std::shared_ptr<Turtle> turtle);
  
  // Removes a turtle from the world.
  void kill_turtle(std::shared_ptr<Turtle> turtle);

  
  // templated function to kill any turtle or agent
  template <typename T>
  void kill(std::shared_ptr<T> &ptr){
    kill_turtle(ptr);
    ptr.reset();
  }

  // Updates vectors storing turtles to remove any that have been deleted.
  void updateTurtleVectors();

  // Sets up the initial state of the world.
  void setup();
  // Executes one simulation step.
  void go();
    
  // Diffuses variables across patches.
  void diffuse();
  // Evaporates variables from patches.
  void evaporate();

  // declare more turtle specific functions below. These are functions that will apply to all turtle types, not a single breed

  
};


#endif