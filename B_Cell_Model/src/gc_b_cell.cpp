#include "gc_b_cell.h"
#include "world.h"
#include "helper_functions.h"

GCBCell::GCBCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
    // std::cout<<"creating GCBcell with ID "<<id<<std::endl;

}

void World::gc_b_cell_function(std::shared_ptr<GCBCell> gc_b_cell) {
  if (!gc_b_cell->get_is_alive()) {return;}
  bool print_test = false;
  if (print_test){std::cout<<"doing GCB cell function for cell "<<gc_b_cell->getID()<<std::endl;}

    if(!gc_b_cell->getInBlood()) {
      // std::cout<<"doing GCB cell function for cell not in blood "<<gc_b_cell->getID()<<std::endl;
      // std::cout<<"location is "<<gc_b_cell->getX()<<", "<<gc_b_cell->getY()<<std::endl;
        Patch& current_patch = get_patch(gc_b_cell->getX(), gc_b_cell->getY());

        if(current_patch.getPatchType() == 2) {
            gc_b_cell->setInBlood(true);
            gc_b_cell->setVisible(false);
        }

        // Downregulates ebi2r and ccr7 so it can localize to follicle center again
        gc_b_cell->setEbi2rLevel(0);
        gc_b_cell->setCcr7Level(0);

        // The GC B-cell only moves if it has not yet reached the follicle center
        if(calculateDistance(WORLD_WIDTH/2, WORLD_HEIGHT/2, gc_b_cell->getX(), gc_b_cell->getY()) > 15) {
          if (print_test){std::cout<<"doing GCB cell function for large distance to center  "<<gc_b_cell->getID()<<std::endl;}
          if (print_test){std::cout<<"location is "<<gc_b_cell->getX()<<", "<<gc_b_cell->getY()<<std::endl;}
            chemotaxis(gc_b_cell);
            if (print_test){std::cout<<"Moving gcb"<<std::endl;}

            move_turtle(gc_b_cell, 0.5);
            if (print_test){std::cout<<"gcb moved"<<std::endl;}

            // gc_b_cell->gc_move();
        } else {
          if (print_test){std::cout<<"DISTANCE OF GC B CELL TO CENTER IS < 15 SO DOING ELSE PART OF GCB CELL FUNCTION"<<std::endl;}

            double proPC2 = current_patch.getIl21() + current_patch.getIl10() * 2 + current_patch.getIfA() + current_patch.getIfG();
            if (print_test){std::cout<<"got proPC2"<<std::endl;}
            double proMem2 = current_patch.getIl21() + current_patch.getIl4();
            if (print_test){std::cout<<"got proMem2"<<std::endl;}
            double proPC = fmod(RNG_Engine(), proPC2);
            if (print_test){std::cout<<"got proPC"<<std::endl;}
            double proMem = fmod(RNG_Engine(), proMem2);
            if (print_test){std::cout<<"got proMem"<<std::endl;}

            if(gc_b_cell->getTimeAlive() % 80 == 0) {
                if(proPC > proMem) {
                    auto plasma_cell = std::make_shared<LLPlasmaCell>(gc_b_cell->getX(), gc_b_cell->getY(), global_ID_counter++, gc_b_cell->getHeading());
                    plasma_cell->copy_other_turtle_attributes(gc_b_cell);
                    plasma_cell->setExposureNumber(step < 2800 ? 1 : 2);
                    plasma_cell->setTimeAlive(0);
                    plasma_cell->setColor("lime");
                    plasma_cell->setShape("circle");
                    plasma_cell->setSize(1);
                    plasma_cell->setS1pr1Level(40);
                    plasma_cell->setProBreg(0);
                    plasma_cell->setTnfaThreshold(plasma_cell->getTnfaThreshold() - 130);
                    std::weak_ptr<Turtle> plasma_cell_weak_ptr = plasma_cell;
                    all_turtles.push_back(plasma_cell_weak_ptr);
                    all_ll_plasma_cells.push_back(plasma_cell);
                    get_patch(plasma_cell->getX(), plasma_cell->getY()).add_turtle(plasma_cell);

                } else {
                    auto mem_b_cell = std::make_shared<MemBCell>(gc_b_cell->getX(), gc_b_cell->getY(), global_ID_counter++, gc_b_cell->getHeading());
                    mem_b_cell->copy_other_turtle_attributes(gc_b_cell);
                    mem_b_cell->setExposureNumber(step < 2800 ? 1 : 2);
                    mem_b_cell->setTimeAlive(0);
                    mem_b_cell->setColor("pink");
                    mem_b_cell->setShape("target");
                    mem_b_cell->setS1pr1Level(10);
                    mem_b_cell->setProBreg(0);
                    mem_b_cell->setCd21Level(0);
                    mem_b_cell->setCxcr5Level(10);

                    std::weak_ptr<Turtle> mem_b_cell_weak_ptr = mem_b_cell;
                    all_turtles.push_back(mem_b_cell_weak_ptr);
                    all_mem_b_cells.push_back(mem_b_cell);
                    get_patch(mem_b_cell->getX(), mem_b_cell->getY()).add_turtle(mem_b_cell);
                }
            }
        }

        //check_breg_status(gc_b_cell);
    }
    if (print_test){std::cout<<"gcb function part 2"<<std::endl;}

    bool die_by_tnf = checkTNFStatus(gc_b_cell);

    gc_b_cell->setTimeAlive(gc_b_cell->getTimeAlive() + 1);
    if (print_test){std::cout<<"gcb time alive"<<std::endl;}
    if((gc_b_cell->getTimeAlive() > 700) || die_by_tnf) {
      if (print_test){std::cout<<"gcb unbinding neighbor"<<std::endl;}
      if (print_test){std::cout<<"linkedTurtles: "<<gc_b_cell->getLinkedTurtles().size()<<std::endl;}

        for(auto neighbor : gc_b_cell->getLinkedTurtles()) {
          if (std::shared_ptr<TfhCell> cell_to_unbind = std::dynamic_pointer_cast<TfhCell>(neighbor.lock())){
            cell_to_unbind->setBcellBindingStatus(false);
            if (print_test){std::cout<<"gcb neighbor unbound"<<std::endl;}

          } else if (std::shared_ptr<Th2Cell> cell_to_unbind = std::dynamic_pointer_cast<Th2Cell>(neighbor.lock())){
            cell_to_unbind->setBcellBindingStatus(false);
            if (print_test){std::cout<<"gcb neighbor unbound"<<std::endl;}

          }
        }
        if (print_test){std::cout<<"killing gdb"<<std::endl;}
        kill(gc_b_cell);
        if (print_test){std::cout<<"killing dead"<<std::endl;}

    }
}
