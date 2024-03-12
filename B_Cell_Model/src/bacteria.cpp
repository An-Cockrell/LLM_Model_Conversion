#include "bacteria.h"
#include "world.h"
Bacteria::Bacteria(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
    // std::cout<<"creating bacteria with ID "<<id<<std::endl;

}


void World::bacteriaFunction(std::shared_ptr<Bacteria> bacteria) {
  bool print_test = false;
  if (!bacteria->get_is_alive()) {return;}
    Patch& current_patch = get_patch(bacteria->getX(), bacteria->getY());

    // Check if the current patch type is 2
    if (current_patch.getPatchType() == 2) {
        int x = rand() % 2; // Randomly generate 0 or 1
        if (x == 0) {
            place_turtle(WORLD_WIDTH-2, std::ceil(WORLD_HEIGHT/2), bacteria);
            bacteria->setS1pr1Level(0);
        } else {
            std::vector<std::shared_ptr<FDCs>> available_fdcs = get_fdcs_with_no_presented_antigen();

            if (!available_fdcs.empty()) {
                auto fdc = available_fdcs[RNG_Engine() % available_fdcs.size()];
                fdc->setTimePresenting(0);
                fdc->setPresentedAntigen(bacteria->getEpitopeType());
                if (print_test){std::cout<<"BACTERIA SETTING FDC COLOR TO RED"<<std::endl;}
                fdc->setColor("red");
                int rTI = RNG_Engine() % bacteria->getNumTIep();
                int rTD = RNG_Engine() % bacteria->getNumTDep();
                if (rTI > rTD) {
                    fdc->setPresentedAntigenType(1);
                } else {
                    fdc->setPresentedAntigenType(2);
                }
                if (print_test){std::cout<<"killing bacteria at end of life. ID is "<<bacteria->getID()<<std::endl;}
                kill(bacteria);
                return;
            }
        }
    }

    bacteria->setS1pr1Level(bacteria->getS1pr1Level() + 0.004);
    chemotaxis(bacteria);
    if (print_test){std::cout<<"Moving bacteria"<<std::endl;}
    move_turtle(bacteria);
}
