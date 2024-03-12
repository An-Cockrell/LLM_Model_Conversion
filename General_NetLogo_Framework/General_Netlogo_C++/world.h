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


class World {
private:
  std::mt19937 RNG_Engine;
  int global_ID_counter;  //ID counter for creating new turtles to make sure each has a unique ID
  int step;
  int time;
  int loop_run;
  
  Patch all_patches[WORLD_WIDTH][WORLD_HEIGHT];
  
  void set_rng_seed(int newSeed){RNG_Engine.seed(newSeed);}
  void add_patch(int x, int y);

public:
  std::vector<std::weak_ptr<Turtle>> all_turtles;
  // include vectors of shared_ptr's of other turtle sub-types here
  // for example:   std::vector<std::shared_ptr<Pmn>> all_pmns;

  
  World(){};
  
  Patch& get_patch(int x, int y);
  Patch& getPatchAhead(std::shared_ptr<Turtle> turtle, float distance=1);
  Patch& getPatchAheadRight(std::shared_ptr<Turtle> turtle, float distance=1);
  Patch& getPatchAheadLeft(std::shared_ptr<Turtle> turtle, float distance=1);

  void setup_patches();
  void display_patches();
  
  void add_turtle(int x, int y, int id, int heading=0);
  void move_turtle(std::shared_ptr<Turtle> turtle);
  void move_turtle_random_jump(std::shared_ptr<Turtle> turtle);
  void turtle_wiggle(std::shared_ptr<Turtle> turtle);
  
  void kill_turtle(std::shared_ptr<Turtle> turtle);
  
  // templated function to kill any turtle or agent
  template <typename T>
  void kill(std::shared_ptr<T> &ptr){
    kill_turtle(ptr);
    ptr.reset();
  }

  void updateTurtleVectors();

  void setup();
  void go();
    
  void diffuse();
  void evaporate();
  
};


#endif