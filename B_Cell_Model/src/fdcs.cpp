#include "fdcs.h"
#include "world.h"

FDCs::FDCs(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
    // std::cout<<"creating FDC with ID "<<id<<std::endl;

}
void World::fdcFunction(std::shared_ptr<FDCs> fdc) {
  // std::cout<<"Doing FDC Function!"<<std::endl;
    // Get the patch where the FDC is
    Patch& fdc_patch = get_patch(fdc->getX(), fdc->getY());

    // Secretes the cytokines
    fdc_patch.setCxcl13Level(fdc_patch.getCxcl13Level() + 2);
    fdc_patch.setIl4(fdc_patch.getIl4() + 1);
    fdc_patch.setIl6(fdc_patch.getIl6() + 2);
    fdc_patch.setIl15(fdc_patch.getIl15() + 2);
    fdc_patch.setIl12(fdc_patch.getIl12() + 2);

    // If FDC is presenting antigen, determines when it will stop presenting it
    if(fdc->getPresentedAntigen() != 0) {
        fdc->setTimePresenting(fdc->getTimePresenting() + 1);
    }
    if(fdc->getTimePresenting() == 300) {
        // Stops presenting the antigen after 300 steps
        fdc->setPresentedAntigen(0);
        fdc->setColor("brown");
        fdc->setPresentedAntigenType(0);
        fdc->setTimePresenting(0);
    }

    // Calculate FDC responsiveness
    fdc->calculate_responsiveness();
}


void FDCs::calculate_responsiveness() {
    if(responsiveness > 0) {
        responsiveness -= 0.01;
    }
}
