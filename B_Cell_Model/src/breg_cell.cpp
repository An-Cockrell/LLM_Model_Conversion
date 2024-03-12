#include "breg_cell.h"
#include "world.h"

BregCell::BregCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
    // std::cout<<"Creating a breg cell with ID "<<id<<std::endl;
}


void World::bregFunction(std::shared_ptr<BregCell> breg_cell) {
  if (!breg_cell->get_is_alive()) {return;}

    // std::cout<<"breg_function for ID number "<<breg_cell->getID()<<std::endl;

    // Get the patch where the BReg cell is
    Patch& breg_patch = get_patch(breg_cell->getX(), breg_cell->getY());

    // Secretes the cytokines
    breg_patch.setIl10(breg_patch.getIl10() + 5);
    breg_patch.setTgfB(breg_patch.getTgfB() + 1);

    // Chemotaxis and move
    chemotaxis(breg_cell);
    // std::cout<<"Moving breg"<<std::endl;
    
    move_turtle(breg_cell);

    // Check TNF status
    bool die_by_tnf = checkTNFStatus(breg_cell);

    // Increase the time alive
    breg_cell->setTimeAlive(breg_cell->getTimeAlive() + 1);

    // Kill if the time alive exceeds 300
    if((breg_cell->getTimeAlive() > 300)|| die_by_tnf) {
      // std::cout<<"killing breg_cell at end of life. ID is "<<breg_cell->getID()<<std::endl;
      kill(breg_cell);
    }
}
