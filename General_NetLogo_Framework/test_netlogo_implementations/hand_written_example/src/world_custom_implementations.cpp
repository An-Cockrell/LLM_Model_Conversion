// world_custom_implementations.cpp
#include "world.h"

// THIS IS A CUSTOM IMPLEMETATION FUNCTION AS AN EXAMPLE TO CREATE NEW EXAMPLE TURTLES
std::shared_ptr<Turtle> World::create_test_turtle(int x, int y, int id, int heading) {
   auto example = std::make_shared<Turtle>(x, y, id, heading);
   example->setLabel("example");
   Patch& patch = get_patch(x, y);
   std::weak_ptr<Turtle> example_weak_ptr = example;

   all_examples.push_back(example);
   all_turtles.push_back(example_weak_ptr);
   patch.add_turtle(example);

   return example;
}

// THIS IS A CUSTOM IMPLEMETATION FUNCTION AS AN EXAMPLE TO CREATE TURTLE SPECIFIC FUNCTIONS
void World::example_turtle_function(std::shared_ptr<Turtle> example_turtle){
  // turtle is just going to wiggle (change heading randomly 45 degrees either direction, then move forward)
  turtle_wiggle(example_turtle);
}



// ###############################################
// ##########END USER MIGHT WANT TO EDIT##########
// #####IN THIS SECTION TO CHANGE PATCH SETUP#####
// ###############################################
// Sets up the initial patches in the world.
void World::setup_patches() {
   for (int x = 0; x < WORLD_WIDTH; ++x) {
       for (int y = 0; y < WORLD_HEIGHT; ++y) {
          add_patch(x, y);
          // ######################################
          // #END USER SHOULD EDIT IN THIS SECTION#
          // #TO ADD INITIAL VARIABLES TO PATCHES##
          // ######################################
       }
   }
}



// ###############################################
// ############ END USER SHOULD EDIT #############
// ########### UPDATE TURTLES FUNCTION ###########
// ###############################################
// updates tracking vectors and gets rid of turtles that have died
void World::updateTurtleVectors(){
    // erasing the turtle weak pointers that are now a null pointer because they have been reset
    all_turtles.erase(std::remove_if(all_turtles.begin(), all_turtles.end(),
    [](const std::weak_ptr<Turtle>& wp) {     //lambda function to check if weak pointer is expired (aka killed and reset() has been called)
        return wp.expired();
      }),
    all_turtles.end());

    // now need to erase the shared pointers that were reset with kill()
    // ##################################################
    // #######END USER SHOULD EDIT IN THIS SECTION#######
    // ####ADD CODE TO RESET FOR EACH TRACKING VECTOR####
    // #COPY AND REPLACE'all_examples' WITH CUSTOM NAMES#
    // ##################################################
   all_examples.erase(std::remove_if(
       all_examples.begin(), all_examples.end(),
       [](const std::shared_ptr<Turtle>& sp) {
           return !sp;  // Check if the shared_ptr is empty
       }
   ), all_examples.end());

    // implement erasure for all other turtle shared_ptr vectors here


}


// ###############################################
// ##### END USER SHOULD EDIT SETUP FUNCTION #####
// ###############################################
void World::setup(){
  set_rng_seed(RNG_SEED);
  // This function initializes the global variables and setus up the world
  step = 0;
  time = 1;
  
  setup_patches(); // Initializing the patches
  
  global_ID_counter = 0;

  // ##################################################
  // #######END USER SHOULD EDIT IN THIS SECTION#######
  // ### ADD CODE TO SETUP THE WORLD AND ADD TURTLES###
  // ##################################################
  // ADD TURTLES LIKE THIS EXAMPLE
  // adding 100 example turtles
  for (int i=0; i< 100; i++){
    // get random params to add new cell
    //random X position
    int random_x = RNG_Engine() % WORLD_WIDTH; 
    //random Y position
    int random_y = RNG_Engine() % WORLD_HEIGHT; 
    // random direction for the new turtle to face
    int random_heading = RNG_Engine() % 360;
    // function that creates the actual turtle, adds it to the patch, and tracking vectors, and returns the turtle itself. This is implemented by the user for each new turtle type
    auto example_turtle = create_test_turtle(random_x, random_y, global_ID_counter++, random_heading);

    // set turtle init vars now. Just setting color for example
    example_turtle->setColor("blue");
  }
  
  // set world global variables at the end
  // eg, system_oxy = 10201.0;
}



// ###############################################
// ####### END USER SHOULD EDIT GO FUNCTION ######
// ###############################################
void World::go() {
    // NetLogo "tick" equivalent in C++ is simply incrementing the time counter
    step++;

  // ##################################################
  // #######END USER SHOULD EDIT IN THIS SECTION#######
  // ###CALL EACH TURTLE TYPE AND HAVE IT DO FUNCTION##
  // ##################################################
    // for each turtle in all_examples
    for (auto& example_turtle : all_examples){
      // do custom defined user function
      example_turtle_function(example_turtle);
    }


    // end of go
    // user should add any end of step functions, or variable updates.




    updateTurtleVectors(); // need to delete dead turtles
}


// ##################################################
// ######END USER SHOULD EDIT DIFFUSE FUNCTION#######
// ##################################################
// function to diffuse cytokine from center patch to neighboring patch for whole grid
// this function can become very long with many vars,
// but it is mostly boiler plate
void World::diffuse(){
  // need to diffuse values from patches out to neighboring patches
  float factor_for_neighbors = 1/8; //eight neighbors in a 2D grid

  // ###############################################
  // #FOLLOW THIS EXAMPLE CODE TO DIFFUSE VARIABLES#
  // ############ ENDOTOXIN AS EXAMPLE #############
  // ###############################################
  /*
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
  
  // ###############################################
  // ######## END USER IMPLEMENTS DIFFUSION ########
  // ############# FOR OTHER VARS BELOW ############
  // ###############################################
  
  
}


// ##################################################
// #####END USER SHOULD EDIT EVAPORATE FUNCTION######
// ##################################################
void World::evaporate(){
  // evaporate variables off of all patches
  for (auto& patch : all_patches) {
  // ###############################################
  // ##FOLLOW THIS EXAMPLE CODE TO EVAPORATE VARS###
  // ############ ENDOTOXIN AS EXAMPLE #############
  // ###############################################
    // set amount to current amount times the evaporation factor
    // patch->setEndotoxin(patch->getEndotoxin() * ENDOTOXIN_EVAPORATION_FACTOR);
  
  }
}