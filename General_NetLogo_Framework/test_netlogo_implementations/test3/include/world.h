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
  bool loop_run;
  float system_oxy;
  float oxy_deficit;
  float total_infection;
  float total_TNF;
  float total_sTNFr;
  float total_IL_10;
  float total_IL_6;
  float total_GCSF;
  float total_pro_TH1;
  float total_pro_TH2;  
  
  Patch all_patches[WORLD_WIDTH][WORLD_HEIGHT]; // Array of patches constituting the world.
  
  // Sets the seed for the random number generator.
  void set_rng_seed(int newSeed) { RNG_Engine.seed(newSeed); }
  
  // Adds a patch to the world at the specified coordinates.
  void add_patch(int x, int y);


public:
  std::vector<std::weak_ptr<Turtle>> all_turtles; // Vector storing all turtles in the world as weak pointers.

  // Tracking vectors
  std::vector<std::shared_ptr<Turtle>> all_injs;
  std::vector<std::shared_ptr<Turtle>> all_pmns;
  std::vector<std::shared_ptr<Turtle>> all_bands;
  std::vector<std::shared_ptr<Turtle>> all_pmn_marrows;
  std::vector<std::shared_ptr<Turtle>> all_monos;
  std::vector<std::shared_ptr<Turtle>> all_mono_marrows;
  std::vector<std::shared_ptr<Turtle>> all_gen_mono_marrows;
  std::vector<std::shared_ptr<Turtle>> all_th0_germs;
  std::vector<std::shared_ptr<Turtle>> all_th0s;
  std::vector<std::shared_ptr<Turtle>> all_th1s;
  std::vector<std::shared_ptr<Turtle>> all_th1_germs;
  std::vector<std::shared_ptr<Turtle>> all_th2s;
  std::vector<std::shared_ptr<Turtle>> all_th2_germs;
  std::vector<std::shared_ptr<Turtle>> all_nks;

  // Creation functions
  std::shared_ptr<Turtle> create_inj_turtle(int x, int y, int id, int heading = 0);
  std::shared_ptr<Turtle> create_pmn_turtle(int x, int y, int id, int heading = 0);
  std::shared_ptr<Turtle> create_band_turtle(int x, int y, int id, int heading = 0);
  std::shared_ptr<Turtle> create_pmn_marrow_turtle(int x, int y, int id, int heading = 0);
  std::shared_ptr<Turtle> create_mono_turtle(int x, int y, int id, int heading = 0);
  std::shared_ptr<Turtle> create_mono_marrow_turtle(int x, int y, int id, int heading = 0);
  std::shared_ptr<Turtle> create_gen_mono_marrow_turtle(int x, int y, int id, int heading = 0);
  std::shared_ptr<Turtle> create_th0_germ_turtle(int x, int y, int id, int heading = 0);
  std::shared_ptr<Turtle> create_th0_turtle(int x, int y, int id, int heading = 0);
  std::shared_ptr<Turtle> create_th1_turtle(int x, int y, int id, int heading = 0);
  std::shared_ptr<Turtle> create_th1_germ_turtle(int x, int y, int id, int heading = 0);
  std::shared_ptr<Turtle> create_th2_turtle(int x, int y, int id, int heading = 0);
  std::shared_ptr<Turtle> create_th2_germ_turtle(int x, int y, int id, int heading = 0);
  std::shared_ptr<Turtle> create_nk_turtle(int x, int y, int id, int heading = 0);

  
  World(){}; // Default constructor.
  
  // Retrieves the patch at the specified coordinates.
  Patch& get_patch(int x, int y);
  Patch& get_patch(float x, float y);

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
  void move(std::shared_ptr<Turtle> turtle){move_turtle(turtle);};
  // Moves a turtle to a random location.
  void move_turtle_random_jump(std::shared_ptr<Turtle> turtle);
  // Causes a turtle to wiggle in place then move forward.
  void turtle_wiggle(std::shared_ptr<Turtle> turtle);

  // removes a turtle from one tracking vector and adds it to a new one
  // be careful, if you pass the wrong vector to remove from, you will have one turtle doing double duty as two different cells
  void change_breed(std::vector<std::shared_ptr<Turtle>>& source, std::vector<std::shared_ptr<Turtle>>& destination, const std::shared_ptr<Turtle>& obj);

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

  void inj_function(Patch& patch_ptr);

  void ec_activate(Patch& patch_ptr);

  void patch_inj_spread(Patch& patch_ptr);

  void ec_function(Patch& patch);

  void pmn_burst(std::shared_ptr<Turtle> pmn);

  void pmn_sniff(std::shared_ptr<Turtle> pmn);

  void pmn_function(std::shared_ptr<Turtle> pmn);

  void heal(Patch& patch);

  void mono_sniff(std::shared_ptr<Turtle> mono);

  void mono_function(std::shared_ptr<Turtle> mono);

  void TH0_function(std::shared_ptr<Turtle> turtle);

  void TH1_function(std::shared_ptr<Turtle> turtle);

  void TH2_function(std::shared_ptr<Turtle> turtle);

  void pmn_marrow_function(std::shared_ptr<Turtle> pmn_marrow);

  void mono_marrow_function(std::shared_ptr<Turtle> mono_marrow);

  void TH0_germ_function(std::shared_ptr<Turtle> TH0_germ);
  
  void TH1_germ_function(std::shared_ptr<Turtle> TH1_germ);

  void TH2_germ_function(std::shared_ptr<Turtle> TH2_germ);

  void update_system_oxy();

  void set_injury_infection(int inj_number, double radius);

  void injure_sterile(int inj_number);
  
  };



#endif