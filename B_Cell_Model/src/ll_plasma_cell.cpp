#include "ll_plasma_cell.h"
#include "world.h"

LLPlasmaCell::LLPlasmaCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
    // std::cout<<"creating LL Plasma cell"<<std::endl;

}


void World::ll_plasma_cell_function(std::shared_ptr<LLPlasmaCell> ll_plasma_cell) {
  if (!ll_plasma_cell->get_is_alive()) {return;}
  bool print_test = false;

  Patch& current_patch = get_patch(ll_plasma_cell->getX(), ll_plasma_cell->getY());
  bool turn_into_breg;
    if(!ll_plasma_cell->getInBlood()) {

        if(current_patch.getPatchType() == 2) {
            ll_plasma_cell->setInBlood(true);
            ll_plasma_cell->setVisible(false);
            kill(ll_plasma_cell);
            if(print_test){std::cout<<"killing ll_plasma cell at follicle exit"<<std::endl;}
            return;
        }

        // Assuming you have a check_breg_status function
        turn_into_breg = checkBregStatus(ll_plasma_cell);
        if (turn_into_breg){
          if (print_test){std::cout<<"ll_plasma ID "<<ll_plasma_cell->getID()<<"turning into Breg"<<std::endl;}
          std::shared_ptr<BregCell> ll_plasma_cell = turnIntoBreg(ll_plasma_cell);
        };

        // ll_plasma_cell->check_breg_status();

        chemotaxis(ll_plasma_cell);
        if (print_test){std::cout<<"moving ll_plasma_cell with use count "<<ll_plasma_cell.use_count()<<std::endl;}
        move_turtle(ll_plasma_cell);
      
    }
    else {
        if(ll_plasma_cell->getTimeAlive() % 200 == 0) {
            auto antibody = std::make_shared<Antibodies>(ll_plasma_cell->getX(), ll_plasma_cell->getY(), global_ID_counter++, ll_plasma_cell->getHeading());
            antibody->copy_other_turtle_attributes(ll_plasma_cell);
            antibody->setTimeAlive(0);
            antibody->setAntibodyType(ll_plasma_cell->getIsotype());
            // antibody->setHidden(true);
            antibody->setColor("mauve");
            
            std::weak_ptr<Turtle> antibody_weak_ptr = antibody;
            all_turtles.push_back(antibody_weak_ptr);
            all_antibodies.push_back(antibody);
            get_patch(antibody->getX(), antibody->getY()).add_turtle(antibody);
        }
    }


      // Assuming you have a check_tnf_status function
    bool die_by_tnf = checkTNFStatus(ll_plasma_cell);
    
    ll_plasma_cell->setTimeAlive(ll_plasma_cell->getTimeAlive() + 1);
    if ((ll_plasma_cell->getTimeAlive() > 8000 + (current_patch.getIl6() + current_patch.getIl21()) * 10) || die_by_tnf) {
      kill(ll_plasma_cell);
      
    }
}
