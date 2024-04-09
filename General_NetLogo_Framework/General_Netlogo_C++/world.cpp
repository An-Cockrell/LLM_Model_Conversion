// world.cpp
#include "world.h"


// Adds a new patch to the world at the specified coordinates.
void World::add_patch(int x, int y) {
    all_patches[x][y] = Patch(x, y);
}

// Retrieves the patch at given (x, y) coordinates, adjusting for world wrapping.
Patch& World::get_patch(int x, int y) {
    x = (x + WORLD_WIDTH) % WORLD_WIDTH;
    y = (y + WORLD_HEIGHT) % WORLD_HEIGHT;
    return all_patches[x][y];
}


// Retrieves the patch located directly ahead of a turtle, based on its heading and distance.
Patch& World::getPatchAhead(std::shared_ptr<Turtle> turtle, float distance) {
  // get the patch in front of the turtle, based on its current heading
  // default value for distance=1
  int heading = turtle->getHeading();
  int current_x = turtle->getX();
  int current_y = turtle->getY();

  // converting degrees to radians and getting delta x and delta y
  // Calculate the displacement using trigonometry and turtle's heading.
  float d_x = cos(heading * M_PI / 180) * distance;
  float d_y = sin(heading * M_PI / 180) * distance;

  // Calculate new coordinates and fetch the corresponding patch.
  int ahead_x = round(current_x + d_x);
  int ahead_y = round(current_y + d_y);

  return get_patch(ahead_x, ahead_y);
}

// Retrieves the patch ahead and to the right of the turtle.
Patch& World::getPatchAheadRight(std::shared_ptr<Turtle> turtle, float distance) {
    int heading = turtle->getHeading() - 45; // Adjust heading to look to the right.
    float d_x = cos(heading * M_PI / 180) * distance;
    float d_y = sin(heading * M_PI / 180) * distance;

    int ahead_x = round(turtle->getX() + d_x);
    int ahead_y = round(turtle->getY() + d_y);

    return get_patch(ahead_x, ahead_y);
}

// Retrieves the patch ahead and to the left of the turtle.
Patch& World::getPatchAheadLeft(std::shared_ptr<Turtle> turtle, float distance) {
    int heading = turtle->getHeading() + 45; // Adjust heading to look to the left.
    float d_x = cos(heading * M_PI / 180) * distance;
    float d_y = sin(heading * M_PI / 180) * distance;

    int ahead_x = round(turtle->getX() + d_x);
    int ahead_y = round(turtle->getY() + d_y);

    return get_patch(ahead_x, ahead_y);
}

// Sets up the initial patches in the world.
void World::setup_patches() {
    for (int x = 0; x < WORLD_WIDTH; x++) {
        for (int y = 0; y < WORLD_HEIGHT; y++) {
            add_patch(x, y);
            // Initial setup actions for patches should be added here.
        }
    }
}

// Displays information about all patches in the world.
void World::display_patches() {
    for (auto &patchRow : all_patches) {
        for (auto &patch : patchRow) {
            patch.display();
        }
    }
}

// Adds a turtle to the world and assigns it to a patch.
void World::add_turtle(int x, int y, int id, int heading) {
    auto new_turtle = std::make_shared<Turtle>(x, y, id, heading);
    get_patch(x, y).add_turtle(new_turtle);
    all_turtles.emplace_back(new_turtle);
}

// Moves a turtle within the world, considering capacity of target patches.
void World::move_turtle(std::shared_ptr<Turtle> turtle){
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

// removes turtle from its patch
void World::kill_turtle(std::shared_ptr<Turtle> turtle){
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
  // will need to add vectors of all_cellType to world.h declaration (all_pmns in this case)
  /*
  for (int i = 0; i < NUM_CELLS_TO_ADD; i++) {
    global_ID_counter++;   //increment global ID counter to give to the new cell

    // get random position to add new cell
    int random_x = RNG_Engine() % WORLD_WIDTH; //random X and Y position
    int random_y = RNG_Engine() % WORLD_HEIGHT;
    
    // creating the actual PMN turtle
    auto pmn = std::make_shared<Pmn>(random_x, random_y, global_ID_counter);   
    
    // get patch to to add the turtle to
    Patch& patch = get_patch(random_x, random_y); 

    // add turtle to patch
    patch.add_turtle(pmn);
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
  */  
  
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


// function to diffuse cytokien from center patch to neighboring patch
void World::diffuse(){
  // need to diffuse values from patches out to neighboring patches
  float factor_for_neighbors = 1/8; //eight neighbors in a 2D grid

  // follow this example format for each diffusing variable on the patches. Using endotoxin as an example
  /*
  // start with Endotoxin
  // for all patches
  for (auto& patch : all_patches){patch->setTempVar(0);}  // reset temp_var on patch
  for (auto& center_patch : all_patches){
    int center_patch_x_pos = center_patch->getX();
    int center_patch_y_pos = center_patch->getY();
    // get totals for neighbor patches
    for (int i=-1; i<2; i++){
      for (int j=-1; j<2; j++){
        
        // skipping center cell
        if (i==0 && j==0) {
          continue;
        }
        // getting modulo of position + x/y to wrap around toroidal world
        int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
        int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

        // actully get neighbor patch
        auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

        // increment neighbor patch with amount diffused out from center patch
        neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getEndotoxin()*factor_for_neighbors*ENDOTOXIN_DIFFUSION_FACTOR); // evenly adding the amount diffused out from the center patch to neighbor patches
      }
    }
  }
  // After getting all the temp_vals from diffusing, set Endotoxin on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches){
    patch->setEndotoxin((patch->getEndotoxin() * (1-ENDOTOXIN_DIFFUSION_FACTOR)) + patch->getTempVar());  // new total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
    patch->setTempVar(0); // resetting
  }
  */
  
  //implement diffusion for other variables below
  
  
}

void World::evaporate(){
  // evaporate variables off of all patches
  for (auto& patch : all_patches) {
    // Endotoxin as an example, amount to set is current amount times the evaporation factor
    // patch->setEndotoxin(patch->getEndotoxin() * ENDOTOXIN_EVAPORATION_FACTOR);

  }
}

