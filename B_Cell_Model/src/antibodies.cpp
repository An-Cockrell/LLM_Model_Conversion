#include "antibodies.h"
#include "world.h"

Antibodies::Antibodies(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
    // std::cout<<"creating antibdy at "<<x<<", "<<y<<" with ID "<<id<<std::endl;
}


void World::antibodiesFunction(std::shared_ptr<Antibodies> antibody) {
  if (!antibody->get_is_alive()) {return;}

    // Increase the time alive of the antibody
    antibody->setTimeAlive(antibody->getTimeAlive() + 1);

    // Check if the antibody has exceeded its lifespan
    if (antibody->getTimeAlive() > 900) {
        // If it has, kill the antibody
        // std::cout<<"killing antibody at end of life. ID is "<<antibody->getID()<<std::endl;
        kill(antibody);
    }
}
