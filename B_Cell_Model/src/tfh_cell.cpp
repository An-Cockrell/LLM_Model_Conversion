#include "tfh_cell.h"
#include "world.h"
#include "helper_functions.h"

TfhCell::TfhCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
    // std::cout<<"creating TFH cell ID "<<id<<std::endl;

}
void World::tfhCellFunction(std::shared_ptr<TfhCell> tfh_cell) {
  if (!tfh_cell->get_is_alive()) {return;}
  bool print_test = false;
  if (print_test){std::cout<<"TfH function for ID "<<tfh_cell->getID()<<std::endl;}

    Patch& current_patch = get_patch(tfh_cell->getX(), tfh_cell->getY());

    if (calculateDistance(WORLD_WIDTH / 2, WORLD_HEIGHT / 2, tfh_cell->getX(), tfh_cell->getY()) > 20 || !tfh_cell->getBcellBindingStatus()) {
      // std::cout<<"chemotaxin TfH with distance to center equal to "<<calculateDistance(WORLD_WIDTH / 2, WORLD_HEIGHT / 2, tfh_cell->getX(), tfh_cell->getY())<<std::endl;

        chemotaxis(tfh_cell);
        // std::cout<<"Moving tfh"<<std::endl;

        move_turtle(tfh_cell);
    }

    // Secretes the following cytokines
    current_patch.setIl21(current_patch.getIl21() + 1);
    current_patch.setIl4(current_patch.getIl4() + 1);
    current_patch.setIl2(current_patch.getIl2() + 1);
    current_patch.setIl10(current_patch.getIl10() + 1);

    tfh_cell->setTimeAlive(tfh_cell->getTimeAlive() + 1);

    // Uncomment the following lines if you want the Tfh cells to die after being alive for more than 500 time steps.
    //if (tfh_cell->getTimeAlive() > 500) {
    //    kill(tfh_cell);
    //}
}
