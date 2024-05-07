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

void World::change_breed(std::vector<std::shared_ptr<Turtle>>& source, std::vector<std::shared_ptr<Turtle>>& destination, const std::shared_ptr<Turtle>& obj) {
    auto it = std::find(source.begin(), source.end(), obj);
    if (it != source.end()) {
        destination.push_back(std::move(*it));
        source.erase(it);
    }
}

// removes turtle from its patch
void World::kill_turtle(std::shared_ptr<Turtle> turtle){
  Patch &turtle_current_patch = get_patch(turtle->getX(), turtle->getY());
  turtle_current_patch.remove_turtle(turtle);
}

//void kill(std::shared_ptr<T> &ptr) <-- templated function that will kill any turtle or subclass of turtle. implemented in header file



