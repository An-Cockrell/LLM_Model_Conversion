// patch.cpp
#include <iostream>
#include <string>
#include <vector>
#include "patch.h"
#include <algorithm>
#include <fstream>

void Patch::add_turtle(std::shared_ptr<Turtle> turtle_to_add){
  turtles_here.emplace_back(turtle_to_add);
}

void Patch::remove_turtle(std::shared_ptr<Turtle> turtle_to_remove){
  turtles_here.erase(std::remove(turtles_here.begin(), turtles_here.end(), turtle_to_remove), turtles_here.end());
  turtle_to_remove.reset();
}

void Patch::display() {
    std::cout << "Patch position: (" << this->getX() << ", " << getY() << ")" << std::endl;
    // std::cout << "Patch color: " << color << std::endl;
    if (turtles_here.size() > 0){
      for (auto &turtle: turtles_here){
        turtle->display();
      }
    }
}
bool Patch::is_at_capacity(){
  return (turtles_here.size() >= PATCH_MAX_CAPACITY);
}


// Add more patch specific functions below for the particular implementation of a net logo model being ported
