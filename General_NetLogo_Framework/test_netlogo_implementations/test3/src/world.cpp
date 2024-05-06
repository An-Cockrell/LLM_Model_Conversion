// world.cpp
#include "world.h"
#include "math.h"

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
// Retrieves the patch at given (x, y) coordinates, adjusting for world wrapping.
Patch& World::get_patch(float x, float y) {
    int x_int = round(x);
    int y_int = round(y);
    x_int = (x_int + WORLD_WIDTH) % WORLD_WIDTH;
    y_int = (y_int + WORLD_HEIGHT) % WORLD_HEIGHT;
    return all_patches[x_int][y_int];
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



// function to diffuse cytokien from center patch to neighboring patch for whole grid
void World::diffuse(){
  // need to diffuse values from patches out to neighboring patches
  float factor_for_neighbors = 1/8; //eight neighbors in a 2D grid

  // Start with Endotoxin
  for (auto& patch : all_patches) { 
    patch->setTempVar(0); 
    } // Reset temp_var on patch
  for (auto& center_patch : all_patches) {
    int center_patch_x_pos = center_patch->getX();
    int center_patch_y_pos = center_patch->getY();
    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        // Skip center cell
        if (i == 0 && j == 0) {
          continue;
        }
        // Getting modulo of position + x/y to wrap around toroidal world
        int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
        int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

        // Actual get neighbor patch
        auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

        // Increment neighbor patch with the amount diffused out from the center patch
        neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getEndotoxin() * factor_for_neighbors * ENDOTOXIN_DIFFUSION_FACTOR); // Evenly add the amount diffused out from the center patch to neighbor patches
      }
    }
  }
  // After getting all the temp_vals from diffusing, set Endotoxin on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches) {
    patch->setEndotoxin((patch->getEndotoxin() * (1 - ENDOTOXIN_DIFFUSION_FACTOR)) + patch->getTempVar()); // New total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
    patch->setTempVar(0); // Resetting
  }
  
  //implement diffusion for other variables below

  // Start with PAF
  for (auto& patch : all_patches) {
    patch->setTempVar(0); 
  } // Reset temp_PAF on patch
  for (auto& center_patch : all_patches) {
  int center_patch_x_pos = center_patch->getX();
  int center_patch_y_pos = center_patch->getY();
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      // Skip center cell
      if (i == 0 && j == 0) {
        continue;
      }
      // Getting modulo of position + x/y to wrap around toroidal world
      int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
      int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

      // Actual get neighbor patch
      auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

      // Increment neighbor patch with the amount diffused out from the center patch
      neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getPAF() * factor_for_neighbors * PAF_DIFFUSION_FACTOR); // Evenly add the amount diffused out from the center patch to neighbor patches
    }
  }
  }
  // After getting all the temp_vals from diffusing, set PAF on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches) {
  patch->setPAF((patch->getPAF() * (1 - PAF_DIFFUSION_FACTOR)) + patch->getTempVar()); // New total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  patch->setTempVar(0); // Resetting
  }
  
  // Start with Cytotox
  for (auto& patch : all_patches) {
  patch->setTempVar(0); // Reset temp_var on patch
  }
  for (auto& center_patch : all_patches) {
  int center_patch_x_pos = center_patch->getX();
  int center_patch_y_pos = center_patch->getY();
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      // Skip center cell
      if (i == 0 && j == 0) {
        continue;
      }
      // Getting modulo of position + x/y to wrap around toroidal world
      int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
      int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

      // Actual get neighbor patch
      auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

      // Increment neighbor patch with the amount diffused out from the center patch
      neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getCytotox() * factor_for_neighbors * CYTOTOX_DIFFUSION_FACTOR); // Evenly add the amount diffused out from the center patch to neighbor patches
    }
  }
  }
  // After getting all the temp_vals from diffusing, set Cytotox on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches) {
  patch->setCytotox((patch->getCytotox() * (1 - CYTOTOX_DIFFUSION_FACTOR)) + patch->getTempVar()); // New total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  patch->setTempVar(0); // Resetting
  }

  // Start with TNF
  for (auto& patch : all_patches) {
  patch->setTempVar(0); // Reset temp_var on patch
  }
  for (auto& center_patch : all_patches) {
  int center_patch_x_pos = center_patch->getX();
  int center_patch_y_pos = center_patch->getY();
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      // Skip center cell
      if (i == 0 && j == 0) {
        continue;
      }
      // Getting modulo of position + x/y to wrap around toroidal world
      int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
      int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

      // Actual get neighbor patch
      auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

      // Increment neighbor patch with the amount diffused out from the center patch
      neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getTNF() * factor_for_neighbors * TNF_DIFFUSION_FACTOR); // Evenly add the amount diffused out from the center patch to neighbor patches
    }
  }
  }
  // After getting all the temp_vals from diffusing, set TNF on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches) {
  patch->setTNF((patch->getTNF() * (1 - TNF_DIFFUSION_FACTOR)) + patch->getTempVar()); // New total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  patch->setTempVar(0); // Resetting
  }

  // Start with sTNFr
  for (auto& patch : all_patches) {
  patch->setTempVar(0); // Reset temp_var on patch
  }
  for (auto& center_patch : all_patches) {
  int center_patch_x_pos = center_patch->getX();
  int center_patch_y_pos = center_patch->getY();
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      // Skip center cell
      if (i == 0 && j == 0) {
        continue;
      }
      // Getting modulo of position + x/y to wrap around toroidal world
      int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
      int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

      // Actual get neighbor patch
      auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

      // Increment neighbor patch with the amount diffused out from the center patch
      neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getSTNFr() * factor_for_neighbors * STNFR_DIFFUSION_FACTOR); // Evenly add the amount diffused out from the center patch to neighbor patches
    }
  }
  }
  // After getting all the temp_vals from diffusing, set sTNFr on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches) {
  patch->setSTNFr((patch->getSTNFr() * (1 - STNFR_DIFFUSION_FACTOR)) + patch->getTempVar()); // New total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  patch->setTempVar(0); // Resetting
  }

  // Start with IL_1
  for (auto& patch : all_patches) {
  patch->setTempVar(0); // Reset temp_var on patch
  }
  for (auto& center_patch : all_patches) {
  int center_patch_x_pos = center_patch->getX();
  int center_patch_y_pos = center_patch->getY();
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      // Skip center cell
      if (i == 0 && j == 0) {
        continue;
      }
      // Getting modulo of position + x/y to wrap around toroidal world
      int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
      int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

      // Actual get neighbor patch
      auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

      // Increment neighbor patch with the amount diffused out from the center patch
      neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getIL_1() * factor_for_neighbors * IL_1_DIFFUSION_FACTOR); // Evenly add the amount diffused out from the center patch to neighbor patches
    }
  }
  }
  // After getting all the temp_vals from diffusing, set IL_1 on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches) {
  patch->setIL_1((patch->getIL_1() * (1 - IL_1_DIFFUSION_FACTOR)) + patch->getTempVar()); // New total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  patch->setTempVar(0); // Resetting
  }

  // Start with INF_G
  for (auto& patch : all_patches) {
  patch->setTempVar(0); // Reset temp_var on patch
  }
  for (auto& center_patch : all_patches) {
  int center_patch_x_pos = center_patch->getX();
  int center_patch_y_pos = center_patch->getY();
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      // Skip center cell
      if (i == 0 && j == 0) {
        continue;
      }
      // Getting modulo of position + x/y to wrap around toroidal world
      int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
      int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

      // Actual get neighbor patch
      auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

      // Increment neighbor patch with the amount diffused out from the center patch
      neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getINF_g() * factor_for_neighbors * INF_G_DIFFUSION_FACTOR); // Evenly add the amount diffused out from the center patch to neighbor patches
    }
  }
  }
  // After getting all the temp_vals from diffusing, set INF_G on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches) {
  patch->setINF_g((patch->getINF_g() * (1 - INF_G_DIFFUSION_FACTOR)) + patch->getTempVar()); // New total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  patch->setTempVar(0); // Resetting
  }

  // Start with IL_8
  for (auto& patch : all_patches) {
  patch->setTempVar(0); // Reset temp_var on patch
  }
  for (auto& center_patch : all_patches) {
  int center_patch_x_pos = center_patch->getX();
  int center_patch_y_pos = center_patch->getY();
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      // Skip center cell
      if (i == 0 && j == 0) {
        continue;
      }
      // Getting modulo of position + x/y to wrap around toroidal world
      int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
      int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

      // Actual get neighbor patch
      auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

      // Increment neighbor patch with the amount diffused out from the center patch
      neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getIL_8() * factor_for_neighbors * IL_8_DIFFUSION_FACTOR); // Evenly add the amount diffused out from the center patch to neighbor patches
    }
  }
  }
  // After getting all the temp_vals from diffusing, set IL_8 on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches) {
  patch->setIL_8((patch->getIL_8() * (1 - IL_8_DIFFUSION_FACTOR)) + patch->getTempVar()); // New total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  patch->setTempVar(0); // Resetting
  }

  // Start with IL-10
  for (auto& patch : all_patches) {
  patch->setTempVar(0); // Reset temp_var on patch
  }
  for (auto& center_patch : all_patches) {
  int center_patch_x_pos = center_patch->getX();
  int center_patch_y_pos = center_patch->getY();
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      // Skip center cell
      if (i == 0 && j == 0) {
        continue;
      }
      // Getting modulo of position + x/y to wrap around toroidal world
      int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
      int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

      // Actual get neighbor patch
      auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

      // Increment neighbor patch with the amount diffused out from the center patch
      neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getIL_10() * factor_for_neighbors * IL_10_DIFFUSION_FACTOR); // Evenly add the amount diffused out from the center patch to neighbor patches
    }
  }
  }
  // After getting all the temp_vals from diffusing, set IL-10 on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches) {
  patch->setIL_10((patch->getIL_10() * (1 - IL_10_DIFFUSION_FACTOR)) + patch->getTempVar()); // New total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  patch->setTempVar(0); // Resetting
  }

  // Start with IL-1ra
  for (auto& patch : all_patches) {
  patch->setTempVar(0); // Reset temp_var on patch
  }
  for (auto& center_patch : all_patches) {
  int center_patch_x_pos = center_patch->getX();
  int center_patch_y_pos = center_patch->getY();
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      // Skip center cell
      if (i == 0 && j == 0) {
        continue;
      }
      // Getting modulo of position + x/y to wrap around toroidal world
      int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
      int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

      // Actual get neighbor patch
      auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

      // Increment neighbor patch with the amount diffused out from the center patch
      neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getIL_1ra() * factor_for_neighbors * IL_1RA_DIFFUSION_FACTOR); // Evenly add the amount diffused out from the center patch to neighbor patches
    }
  }
  }
  // After getting all the temp_vals from diffusing, set IL-1ra on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches) {
  patch->setIL_1ra((patch->getIL_1ra() * (1 - IL_1RA_DIFFUSION_FACTOR)) + patch->getTempVar()); // New total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  patch->setTempVar(0); // Resetting
  }

  // Start with sIL_1r
  for (auto& patch : all_patches) {
  patch->setTempVar(0); // Reset temp_var on patch
  }
  for (auto& center_patch : all_patches) {
  int center_patch_x_pos = center_patch->getX();
  int center_patch_y_pos = center_patch->getY();
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      // Skip center cell
      if (i == 0 && j == 0) {
        continue;
      }
      // Getting modulo of position + x/y to wrap around toroidal world
      int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
      int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

      // Actual get neighbor patch
      auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

      // Increment neighbor patch with the amount diffused out from the center patch
      neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getSIL_1r() * factor_for_neighbors * SIL_1R_DIFFUSION_FACTOR); // Evenly add the amount diffused out from the center patch to neighbor patches
    }
  }
  }
  // After getting all the temp_vals from diffusing, set sIL_1r on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches) {
  patch->setSIL_1r((patch->getSIL_1r() * (1 - SIL_1R_DIFFUSION_FACTOR)) + patch->getTempVar()); // New total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  patch->setTempVar(0); // Resetting
  }

  // Start with IL-12
  for (auto& patch : all_patches) {
  patch->setTempVar(0); // Reset temp_var on patch
  }
  for (auto& center_patch : all_patches) {
  int center_patch_x_pos = center_patch->getX();
  int center_patch_y_pos = center_patch->getY();
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      // Skip center cell
      if (i == 0 && j == 0) {
        continue;
      }
      // Getting modulo of position + x/y to wrap around toroidal world
      int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
      int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

      // Actual get neighbor patch
      auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

      // Increment neighbor patch with the amount diffused out from the center patch
      neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getIL_12() * factor_for_neighbors * IL_12_DIFFUSION_FACTOR); // Evenly add the amount diffused out from the center patch to neighbor patches
    }
  }
  }
  // After getting all the temp_vals from diffusing, set IL-12 on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches) {
  patch->setIL_12((patch->getIL_12() * (1 - IL_12_DIFFUSION_FACTOR)) + patch->getTempVar()); // New total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  patch->setTempVar(0); // Resetting
  }

  // Start with IL_4
  for (auto& patch : all_patches) {
  patch->setTempVar(0); // Reset temp_var on patch
  }
  for (auto& center_patch : all_patches) {
  int center_patch_x_pos = center_patch->getX();
  int center_patch_y_pos = center_patch->getY();
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      // Skip center cell
      if (i == 0 && j == 0) {
        continue;
      }
      // Getting modulo of position + x/y to wrap around toroidal world
      int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
      int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

      // Actual get neighbor patch
      auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

      // Increment neighbor patch with the amount diffused out from the center patch
      neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getIL_4() * factor_for_neighbors * IL_4_DIFFUSION_FACTOR); // Evenly add the amount diffused out from the center patch to neighbor patches
    }
  }
  }
  // After getting all the temp_vals from diffusing, set IL_4 on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches) {
  patch->setIL_4((patch->getIL_4() * (1 - IL_4_DIFFUSION_FACTOR)) + patch->getTempVar()); // New total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  patch->setTempVar(0); // Resetting
  }

  // Start with GCSF
  for (auto& patch : all_patches) {
  patch->setTempVar(0); // Reset temp_var on patch
  }
  for (auto& center_patch : all_patches) {
  int center_patch_x_pos = center_patch->getX();
  int center_patch_y_pos = center_patch->getY();
  for (int i = -1; i < 2; i++) {
    for (int j = -1; j < 2; j++) {
      // Skip center cell
      if (i == 0 && j == 0) {
        continue;
      }
      // Getting modulo of position + x/y to wrap around toroidal world
      int neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH);
      int neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);

      // Actual get neighbor patch
      auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

      // Increment neighbor patch with the amount diffused out from the center patch
      neighbor_patch.setTempVar(neighbor_patch.getTempVar() + center_patch->getGCSF() * factor_for_neighbors * GCSF_DIFFUSION_FACTOR); // Evenly add the amount diffused out from the center patch to neighbor patches
    }
  }
  }
  // After getting all the temp_vals from diffusing, set GCSF on each patch from the temp var, then reset tempvar
  for (auto& patch : all_patches) {
  patch->setGCSF((patch->getGCSF() * (1 - GCSF_DIFFUSION_FACTOR)) + patch->getTempVar()); // New total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
  patch->setTempVar(0); // Resetting
  }

}

void World::evaporate() {
   for (auto& patch : all_patches) {
       patch->setEndotoxin(patch->getEndotoxin() * ENDOTOXIN_EVAPORATION_FACTOR);
       patch->setPAF(patch->getPAF() * PAF_EVAPORATION_FACTOR);
       patch->setCytotox(patch->getCytotox() * CYTOTOX_EVAPORATION_FACTOR);
       patch->setTNF(patch->getTNF() * TNF_EVAPORATION_FACTOR);
       patch->setIL_1(patch->getIL_1() * IL_1_EVAPORATION_FACTOR);
       patch->setSTNFr(patch->getSTNFr() * STNFR_EVAPORATION_FACTOR);
       patch->setIL_1ra(patch->getIL_1ra() * IL_1RA_EVAPORATION_FACTOR);
       patch->setSIL_1r(patch->getSIL_1r() * SIL_1R_EVAPORATION_FACTOR);
       patch->setINF_g(patch->getINF_g() * INF_G_EVAPORATION_FACTOR);
       patch->setIL_8(patch->getIL_8() * IL_8_EVAPORATION_FACTOR);
       patch->setIL_10(patch->getIL_10() * IL_10_EVAPORATION_FACTOR);
       patch->setIL_12(patch->getIL_12() * IL_12_EVAPORATION_FACTOR);
       patch->setIL_4(patch->getIL_4() * IL_4_EVAPORATION_FACTOR);
       patch->setGCSF(patch->getGCSF() * GCSF_EVAPORATION_FACTOR);
   }
}

