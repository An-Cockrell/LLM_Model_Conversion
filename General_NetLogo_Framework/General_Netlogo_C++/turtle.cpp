// turtle.cpp
#include <iostream>
#include <string>
#include "agent.h"
#include <cmath>
#include "turtle.h"
#include "parameters.h"

std::pair<int,int> Turtle::move(float distance) {
  // default val for distance=1
  // calculates the movement for the turtle based on its heading and distance to move and returns the coordinates of the destination patch
    temp_x = x_dec;   //placeholders in case we are not able to actually execute the move. In that case, these will reset x/y decimal values
    temp_y = y_dec;
    float d_x = cos(heading * M_PI/180) * distance;   // converting degrees to radians and getting delta x and delta y
    float d_y = sin(heading * M_PI/180) * distance;
    if (!TURTLE_CONTINOUS_MOVEMENT) {   //ie discrete movement, turtles always land on the middle of patches
      d_x = round(d_x);
      d_y = round(d_y);
    }
    x_dec += d_x;
    y_dec += d_y;
    
    x_dec = fmod(fmod(x_dec, WORLD_WIDTH) +WORLD_WIDTH, WORLD_WIDTH);              // wrapping around world with modulo
    y_dec = fmod(fmod(y_dec, WORLD_HEIGHT) +WORLD_HEIGHT, WORLD_HEIGHT);           // taking double mod to make sure that a positive number for grid coordinates
    int newx, newy;
    newx = trunc(x_dec);    // truncating decimal coords to get int coords
    newy = trunc(y_dec);

    return std::make_pair(newx, newy);
}
std::pair<int,int> Turtle::jumpRandom(std::mt19937 &RNG_Engine) {
  // get a random direction, then a random distance, then return the coordinates that we jumped to
    int random_heading = RNG_Engine()%360;
    int random_distance = RNG_Engine()%MAX_RANDOM_DISTANCE;
    
    this->setHeading(random_heading);
    return this->move(random_distance);
}

void Turtle::execute_move(bool didMove){
  // if we execute the move, then the world has moved the turtle to a new patch, and we need to update internal variables
  if(didMove){
    setX(trunc(x_dec));                             // truncating decimal coords to get int coords
    setY(trunc(y_dec));
  } else{
    x_dec = temp_x;
    y_dec = temp_y;
  }
}

void Turtle::wiggle(std::mt19937 &RNG_Engine){
  // randomly wiggle heading up to 45 degrees left up to 45 degrees right
  int random_left = RNG_Engine()%45;
  int random_right = RNG_Engine()%45;
  this->setHeading(this->getHeading() + random_left - random_right);
}

void Turtle::display() {
  std::cout << "Turtle position: (" << this->getX() << ", " << this->getY() << ")" << std::endl;
  std::cout << "Turtle exact position: (" << x_dec << ", " << y_dec << ")" << std::endl;
  std::cout << "Turtle Heading: (" << heading << ")" << std::endl;
  std::cout << "Turtle ID: (" << ID_num << ")" << std::endl;
}

// add more turtle specific functions below. These are functions that will apply to all turtle types, not a single breed
