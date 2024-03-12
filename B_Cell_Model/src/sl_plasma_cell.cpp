#include "sl_plasma_cell.h"
#include "world.h"

SLPlasmaCell::SLPlasmaCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
    // std::cout<<"creating SL plasma cell with ID "<<id<<std::endl;

}

void World::sl_plasma_cell_function(std::shared_ptr<SLPlasmaCell> sl_plasma_cell) {
  if (!sl_plasma_cell->get_is_alive()) {return;}
  bool print_test = false;
  Patch& current_patch = get_patch(sl_plasma_cell->getX(), sl_plasma_cell->getY());
  if (print_test){std::cout<<"doing SL plasma function for "<<sl_plasma_cell->getID()<<std::endl;}
    if(!sl_plasma_cell->getInBlood()) {
      if (print_test){std::cout<<"doing SL plasma cause not in blood at "<<sl_plasma_cell->getX()<<", "<<sl_plasma_cell->getY()<<std::endl;}

        if(current_patch.getPatchType() == 2) {
            sl_plasma_cell->setInBlood(true);
            sl_plasma_cell->setVisible(false);
            if(print_test){std::cout<<"killing Sl_plasma cell at follicle exit"<<std::endl;}
            return;
        }

        // Assuming you have a check_breg_status function
        bool turn_into_breg = checkBregStatus(sl_plasma_cell);
        if (turn_into_breg){
          if (print_test){std::cout<<"sl_plasma ID "<<sl_plasma_cell->getID()<<"turning into Breg"<<std::endl;}
          std::shared_ptr<BregCell> sl_plasma_cell = turnIntoBreg(sl_plasma_cell);
        };

        chemotaxis(sl_plasma_cell);
        if (print_test){std::cout<<"moving SL_plasma_cell with use count "<<sl_plasma_cell.use_count()<<std::endl;}

        move_turtle(sl_plasma_cell);
        //std::cout<<"moved SL_plasma_cell with use count "<<sl_plasma_cell.use_count()<<std::endl;
    }

    if(sl_plasma_cell->getTimeAlive() % 50 == 0) {
        auto antibody = std::make_shared<Antibodies>(sl_plasma_cell->getX(), sl_plasma_cell->getY(), global_ID_counter++, sl_plasma_cell->getHeading());
        antibody->copy_other_turtle_attributes(sl_plasma_cell);
        antibody->setTimeAlive(0);
        antibody->setAntibodyType(sl_plasma_cell->getIsotype());
        antibody->setVisible(false);
        antibody->setColor("mauve");
        
        std::weak_ptr<Turtle> antibody_weak_ptr = antibody;
        all_turtles.push_back(antibody_weak_ptr);
        all_antibodies.push_back(antibody);
        get_patch(antibody->getX(), antibody->getY()).add_turtle(antibody);
    }

    // Checks level of TNF-a stimulation for apoptosis
    bool die_by_tnf = checkTNFStatus(sl_plasma_cell);

    sl_plasma_cell->setTimeAlive(sl_plasma_cell->getTimeAlive() + 1);
    if((sl_plasma_cell->getTimeAlive() > 240 + (current_patch.getIl6() + current_patch.getIl21()) * 10) || die_by_tnf) {
        kill(sl_plasma_cell);
    }
}
