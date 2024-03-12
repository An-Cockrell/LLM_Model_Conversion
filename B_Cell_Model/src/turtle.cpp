// turtle.cpp
#include <iostream>
#include <string>
#include "agent.h"
#include <cmath>
#include "turtle.h"
#include "parameters.h"

void Turtle::addLinkedTurtle(std::weak_ptr<Turtle> linkedTurtle) {
    linkedTurtles.push_back(linkedTurtle);
    std::shared_ptr<Turtle> temp_shared_ptr = std::dynamic_pointer_cast<Turtle>(shared_from_this());
    std::weak_ptr<Turtle> weak_ptr = temp_shared_ptr;
    linkedTurtle.lock()->getLinkedTurtles().push_back(weak_ptr);
    temp_shared_ptr.reset();
}

std::vector<std::weak_ptr<Turtle>>& Turtle::getLinkedTurtles() {
    return linkedTurtles;
}

void Turtle::removeLinkedTurtle(){
  for(auto neighbor : this->getLinkedTurtles()){
    auto iter = std::find_if(neighbor.lock()->getLinkedTurtles().begin(), neighbor.lock()->getLinkedTurtles().end(), WeakPtrComparator<Turtle>(ID_num));  // finding where current ID_num of this turtle is in the nieghbors list of links
    if (iter != neighbor.lock()->getLinkedTurtles().end()) {
         neighbor.lock()->getLinkedTurtles().erase(iter);
     } else {
         std::cout << "Linked turtle not found in the neighbors vector. 1 ERRORRRR" << std::endl;
     }

     iter = std::find_if(this->getLinkedTurtles().begin(), this->getLinkedTurtles().end(), WeakPtrComparator<Turtle>(neighbor.lock()->getID()));
     if (iter != this->getLinkedTurtles().end()) {
          this->getLinkedTurtles().erase(iter);
      } else {
          std::cout << "Neighbor Linked turtle not found in the vector. 2 ERRORRRR" << std::endl;
      }
  }
  linkedTurtles.clear();
}


std::pair<double,double> Turtle::move(float distance) {
  // this function moves the turtle internally and is called inside World::move_turtle(), so should only really be accessed from there.

  // default val for distance=1
  // calculates the movement for the turtle based on its heading and distance to move and returns the coordinates of the destination patch
    temp_x = x_dec;   //placeholders in case we are not able to actually execute the move. In that case, these will reset x/y decimal values
    temp_y = y_dec;
    double d_x = cos(heading * M_PI/180) * distance;   // converting degrees to radians and getting delta x and delta y
    double d_y = -1 * sin(heading * M_PI/180) * distance;
    if (!TURTLE_CONTINOUS_MOVEMENT) {   //ie discrete movement, turtles always land on the middle of patches
      d_x = round(d_x);
      d_y = round(d_y);
    }
    x_dec += d_x;
    y_dec += d_y;
    if (TOROIDAL_WORLD){
      x_dec = fmod(fmod(x_dec, WORLD_WIDTH) +WORLD_WIDTH, WORLD_WIDTH);              // wrapping around world with modulo
      y_dec = fmod(fmod(y_dec, WORLD_HEIGHT) +WORLD_HEIGHT, WORLD_HEIGHT);           // taking double mod to make sure that a positive number for grid coordinates
    } else{
      if (x_dec >= WORLD_WIDTH){x_dec = WORLD_WIDTH-1;}
      if (y_dec >= WORLD_HEIGHT){y_dec = WORLD_HEIGHT-1;}
      if (x_dec < 0){x_dec = 0;}
      if (y_dec < 0){y_dec = 0;}
    }
    return std::make_pair(x_dec, y_dec);
}
void Turtle::execute_move(bool didMove){
  // if we execute the move, then the world has moved the turtle to a new patch, and we need to update internal variables
  if(didMove){
    setX(trunc(x_dec));                             // truncating decimal coords to get int coords
    setY(trunc(y_dec));
    temp_x = x_dec;
    temp_y = y_dec;
  } else{
    x_dec = temp_x;
    y_dec = temp_y;
  }
}




std::pair<int,int> Turtle::jumpRandom(std::mt19937 &RNG_Engine) {
  // get a random direction, then a random distance, then return the coordinates that we jumped to
    int random_heading = RNG_Engine()%360;
    int random_distance = RNG_Engine()%MAX_RANDOM_DISTANCE;

    this->setHeading(random_heading);
    return this->move(random_distance);
}


void Turtle::wiggle(std::mt19937 &RNG_Engine){
  // randomly wiggle heading up to 45 degrees left up to 45 degrees right
  int random_left = RNG_Engine()%45;
  int random_right = RNG_Engine()%45;

  // std::cout<<"WIGGLING TURTLE NOW. new heading is "<<this->getHeading() + random_left - random_right<<std::endl;
  this->setHeading(this->getHeading() + (random_left - random_right));
}

void Turtle::display() {
  std::cout << "Turtle position: (" << this->getX() << ", " << this->getY() << ")" << std::endl;
  std::cout << "Turtle exact position: (" << x_dec << ", " << y_dec << ")" << std::endl;
  std::cout << "Turtle Heading: (" << heading << ")" << std::endl;
  std::cout << "Turtle ID: (" << ID_num << ")" << std::endl;
}

// add more turtle specific functions below. These are functions that will apply to all turtle types, not a single breed

void Turtle::copy_other_turtle_attributes(std::shared_ptr<Turtle> otherTurtle){
  this->setHeading(otherTurtle->getHeading());
  this->setTimeAlive(otherTurtle->getTimeAlive());
  // Copying the additional attributes
  // this->x_dec = otherTurtle->get_x_dec();
  // this->y_dec = otherTurtle->get_y_dec();
  // this->temp_x = otherTurtle->get_temp_x();
  // this->temp_y = otherTurtle->get_temp_y();
  this->setInBlood(otherTurtle->getInBlood());
  this->setBcr(otherTurtle->getBcr());
  this->setIsotype(otherTurtle->getIsotype());
  this->setCsrBool(otherTurtle->getCsrBool());
  this->setTnfaThreshold(otherTurtle->getTnfaThreshold());
  this->setBregThreshold(otherTurtle->getBregThreshold());
  this->setCd21Level(otherTurtle->getCd21Level());
  this->setS1pr1Level(otherTurtle->getS1pr1Level());
  this->setS1pr2Level(otherTurtle->getS1pr2Level());
  this->setCxcr5Level(otherTurtle->getCxcr5Level());
  this->setCcr7Level(otherTurtle->getCcr7Level());
  this->setEbi2rLevel(otherTurtle->getEbi2rLevel());
  this->setProBreg(otherTurtle->getProBreg());
  this->setLevelOfActivation(otherTurtle->getLevelOfActivation());
  this->setTnfaStimulation(otherTurtle->getTnfaStimulation());
  this->setExposureNumber(otherTurtle->getExposureNumber());
}
