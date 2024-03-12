// world.cpp
#include "world.h"


void World::add_patch(int x, int y){
  all_patches[x][y] = Patch(x,y);
}

Patch& World::get_patch(int x, int y){
  x = (x+WORLD_WIDTH)%WORLD_WIDTH;
  y = (y+WORLD_HEIGHT)%WORLD_HEIGHT;
  return all_patches[x][y];
}

Patch& World::getPatchAhead(std::shared_ptr<Turtle> turtle, float distance){
  // default value for distance=1
  // get the patch in front of the turtle, based on its current heading
  int heading = turtle->getHeading();
  int current_x = turtle->getX();
  int current_y = turtle->getY();

  float d_x = cos(heading * M_PI/180) * distance;   // converting degrees to radians and getting delta x and delta y
  float d_y = sin(heading * M_PI/180) * distance;

  int ahead_x = round(current_x + d_x);
  int ahead_y = round(current_y + d_y);

  return get_patch(ahead_x, ahead_y);
}

Patch& World::getPatchAheadRight(std::shared_ptr<Turtle> turtle, float distance){
  // default value for distance=1
  // get the patch in front and to the right of the turtle, based on its current heading
  int heading = turtle->getHeading() - 45; // to look right 45 degrees
  int current_x = turtle->getX();
  int current_y = turtle->getY();

  float d_x = cos(heading * M_PI/180) * distance;   // converting degrees to radians and getting delta x and delta y
  float d_y = sin(heading * M_PI/180) * distance;

  int ahead_x = round(current_x + d_x);
  int ahead_y = round(current_y + d_y);

  return get_patch(ahead_x, ahead_y);
}

Patch& World::getPatchAheadLeft(std::shared_ptr<Turtle> turtle, float distance){
  // default value for distance=1
  // get the patch in front and to the right of the turtle, based on its current heading
  int heading = turtle->getHeading() + 45; // to look left 45 degrees
  int current_x = turtle->getX();
  int current_y = turtle->getY();

  float d_x = cos(heading * M_PI/180) * distance;   // converting degrees to radians and getting delta x and delta y
  float d_y = sin(heading * M_PI/180) * distance;

  int ahead_x = round(current_x + d_x);
  int ahead_y = round(current_y + d_y);

  return get_patch(ahead_x, ahead_y);
}

void World::setup_patches(){
  for(int x=0; x<WORLD_WIDTH; x++){
    for(int y=0; y<WORLD_HEIGHT; y++){
      add_patch(x,y);
      // set initial values on patches here; eg patch.setOxy(100) to set patch oxy to initial value
    }
  }
}

void World::display_patches(){
  for(auto &patchArr: all_patches){
    for(auto &patch: patchArr){
      patch.display();
    }
  }
}

void World::add_turtle(int x, int y, int id, int heading){
  std::shared_ptr<Turtle> new_turtle = std::make_shared<Turtle>(x, y, id, heading);
  get_patch(x,y).add_turtle(new_turtle);
  all_turtles.emplace_back(new_turtle);
}

void World::move_turtle(std::shared_ptr<Turtle> turtle){
  // moves the turtle but asking the turtle to calculate its movement, and if there is space on the target_patch, the world executes the move
  Patch &turtle_current_patch = get_patch(turtle->getX(), turtle->getY());
  std::pair<int,int> new_coords = turtle->move(); //moving along turtle.heading for the default value of 1 unit
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

void World::move_turtle_random_jump(std::shared_ptr<Turtle> turtle){
  // same as move turtle but instead of turtle-> move() we are using turtle->jumpRandom(RNG_Engine)
  Patch &turtle_current_patch = get_patch(turtle->getX(), turtle->getY());
  std::pair<int,int> new_coords = turtle->jumpRandom(RNG_Engine);
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

void World::turtle_wiggle(std::shared_ptr<Turtle> turtle){
  // wiggles the turtles heading, then moves forward along the new heading for 1 unit
  turtle->wiggle(RNG_Engine);
  move_turtle(turtle);
}

void World::kill_turtle(std::shared_ptr<Turtle> turtle){
  // removed turtle from its patch
  Patch &turtle_current_patch = get_patch(turtle->getX(), turtle->getY());
  turtle_current_patch.remove_turtle(turtle);
}

//void kill(std::shared_ptr<T> &ptr) <-- templated function that will kill any turtle or subclass of turtle. implemented in header file

void World::updateTurtleVectors(){
    // erasing the turtle weak pointers that are now a null pointer because they have been reset
    all_turtles.erase(std::remove_if(all_turtles.begin(), all_turtles.end(),
    [](const std::weak_ptr<Turtle>& wp) {     //lambda function to check if weak pointer is expired (aka killed and reset() has been called)
        return wp.expired();
      }),
    all_turtles.end());

    // now need to erase the shared pointers that were reset with kill()
    // for example:  all_pmns.erase(std::remove(all_pmns.begin(), all_pmns.end(), nullptr), all_pmns.end());
    // implement erasure for all turtle shared_ptr vectors here
}

void World::setup(){
  set_rng_seed(RNG_SEED);
  // This function initializes the global variables and setus up the world
  step = 0;
  time = 1;
  
  setup_patches(); // Initializing the patches
  
  global_ID_counter = 0;
  // create cell types like so, using PMN as an example
  for (int i = 0; i < NUM_CELLS_TO_ADD; i++) {
    global_ID_counter++;   //increment global ID counter to give to the new cell
    int random_x = RNG_Engine() % WORLD_WIDTH; //random X and Y position
    int random_y = RNG_Engine() % WORLD_HEIGHT;
    auto pmn = std::make_shared<Pmn>(random_x, random_y, global_ID_counter);   // creating the actual PMN turtle
    Patch& patch = get_patch(random_x, random_y); // get patch to to add the turtle to
    patch.add_turtle(pmn); // add turtle to patch
    std::weak_ptr<Turtle> pmn_weak_ptr = pmn;   // creating a weak pointer to give to all turtles vector so that deleting pmn object kills it in the turtle list as well. Chat gpt didnt do this, I did
  
    // setting new cell variables
    pmn->setAge(rand() % 50); // setting age to random number no larger than 50
    pmn->setWbcRoll(1);
    pmn->setWbcStick(0);
    pmn->setWbcMigrate(0);
    pmn->setPmnPcd(10);
  
    // performing random jump to get a random space to start because thats how the netlogo did it, probably unnecessary because of random x and y starting position
    for (int j = 0; j < 10; j++) {
      move_turtle_random_jump(pmn);
    }
    // adding new turtle to all turtles vector, and specific type vector
    all_turtles.push_back(pmn_weak_ptr);
    all_pmns.push_back(pmn);
  }
    
  // repeat the above code block for all turtle types in setup
  
  // set world global variables at the end
  // eg, system_oxy = 10201.0;
}

void World::go() {
    // NetLogo "tick" equivalent in C++ is simply incrementing the time counter
    step++;

    // implement netlogo Go function here


    // end of go
    updateTurtleVectors(); // need to update turtle positions/delete dead turtles
}

void World::diffuse(){
  // need to diffuse values from patches out to neighboring patches
  float factor_for_neighbors = 1/8; //eight neighbors in a 2D grid

  // // follow this example format for each diffusing variable
  // for (auto& patch : all_patches){patch->setTempVar(0);}  // reset temp_var
  // // start with Endotoxin
  // // get totals for neighbors
  // for (auto& center_patch : all_patches){
  //   int center_patch_x_pos = center_patch->getX();
  //   int center_patch_y_pos = center_patch->getY();
  //   for (int i=-1; i<2; i++){
  //     for (int j=-1; j<2; j++){
  //       if (i==0 && j==0) { // skipping center cell
  //         continue;
  //       }
  //       int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH); // getting modulo of position + x/y to wrap around toroidal world
  //       int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);
  //       auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);
  //       neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getEndotoxin()*factor_for_neighbors*ENDOTOXIN_DIFFUSION_FACTOR); // evenly adding the amount diffused out from the center patch to neighbor patches
  //     }
  //   }
  // }
  // // set Endotoxin on each patch from the temp var, then reset tempvar
  // for (auto& patch : all_patches){
  //   patch->setEndotoxin((patch->getEndotoxin() * (1-ENDOTOXIN_DIFFUSION_FACTOR)) + patch->getTempVar());  // new total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  //   patch->setTempVar(0); // resetting
  // }
  
  //implement diffusion for other variables below
  
  
}

void World::evaporate(){
  // evaporate variables off of all patches
  for (auto& patch : all_patches) {
    // Endotoxin as an example, amount to set is current amount times the evaporation factor
    // patch->setEndotoxin(patch->getEndotoxin() * ENDOTOXIN_EVAPORATION_FACTOR);

  }
}

