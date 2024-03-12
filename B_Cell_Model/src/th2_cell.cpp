#include "th2_cell.h"
#include "world.h"
#include "helper_functions.h"

Th2Cell::Th2Cell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
    // std::cout<<"creating TH 2 cell ID "<<id<<std::endl;

}


void World::th2CellFunction(std::shared_ptr<Th2Cell> th2_cell) {
  if (!th2_cell->get_is_alive()) {return;}
  bool print_test = false;
  if (print_test){std::cout<<"TH2 function for "<<th2_cell->getID()<<std::endl;}

    Patch& current_patch = get_patch(th2_cell->getX(), th2_cell->getY());

    // Calculate the Euclidean distance between the cell's location and the center of the world (WORLD_WIDTH/2, WORLD_HEIGHT/2)
    if (calculateDistance(WORLD_WIDTH/2, WORLD_HEIGHT/2, th2_cell->getX(), th2_cell->getY()) > 20 || !th2_cell->getBcellBindingStatus()) {
        chemotaxis(th2_cell);
        // std::cout<<"Moving th2"<<std::endl;

        move_turtle(th2_cell);
    }

    // Secretes the following cytokines
    current_patch.setIl4(current_patch.getIl4() + 1);
    current_patch.setIl10(current_patch.getIl10() + 1);

    th2_cell->setTimeAlive(th2_cell->getTimeAlive() + 1);

    // Uncomment the following lines if you want the Th2 cells to die after being alive for more than 500 time steps.
    //if (th2_cell->getTimeAlive() > 500) {
    //    kill(th2_cell);
    //}
}
