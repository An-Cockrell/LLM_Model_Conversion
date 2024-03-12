#include "activated_b_cell.h"
#include "world.h"

ActivatedBCell::ActivatedBCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
    // std::cout<<"Creating activated b cell with ID "<<id<<std::endl;
}


void World::activatedBCellFunction(std::shared_ptr<ActivatedBCell> abcell) {
  bool print_test = false;
  if (!abcell->get_is_alive()) {return;}
    // Only performs the following commands if currently inside the follicle, and NOT in the blood/lymph
    if (print_test){std::cout<<"ABC function for ID "<<abcell->getID()<<" use count is "<<abcell.use_count()<<std::endl;}

    if (!abcell->getInBlood()) {
      if (print_test){std::cout<<"doing ABC function for not in blood"<<std::endl;}
        // Get the patch where the B cell is
        Patch& bcell_patch = get_patch(abcell->getX(), abcell->getY());

        // If the cell reaches the follicle exit
        if(bcell_patch.getPatchType() == 2) {
            abcell->setInBlood(true);
            abcell->setVisible(false);
            if (print_test){std::cout<<"killing abc cell at follicle exit "<<abcell->getID()<<std::endl;}
            if (print_test){std::cout<<"abcell use count is "<<abcell.use_count()<<std::endl;}
            kill(abcell);
            return;
        }

        isotypeSwitch(abcell);   // Determines which isotype to switch to
        if (print_test){std::cout<<"abcell use count 1is "<<abcell.use_count()<<std::endl;}

        if (abcell->getResponseType() == 2) {
            tdResponse(abcell);
            if (print_test){std::cout<<"abcell use count 2ais "<<abcell.use_count()<<std::endl;}
            if (!abcell->get_is_alive()){return;}
        } else if (abcell->getResponseType() == 1) {
            tiResponse(abcell);
            // return;
            if (print_test){std::cout<<"abcell use count 2bis "<<abcell.use_count()<<std::endl;}
        }

        //abcell->checkBregStatus();
        if (print_test){std::cout<<"abcell use count 3is "<<abcell.use_count()<<std::endl;}

        chemotaxis(abcell);
        if (print_test){std::cout<<"abcell use count 4is "<<abcell.use_count()<<std::endl;}

        move_turtle(abcell);
        if (print_test){std::cout<<"abcell use count 5is "<<abcell.use_count()<<std::endl;}

    }

    bool die_by_tnf = checkTNFStatus(abcell);
    if (print_test){std::cout<<"abcell use count 6is "<<abcell.use_count()<<std::endl;}

    abcell->setTimeAlive(abcell->getTimeAlive() + 1);
    if ((abcell->getTimeAlive() > 300) || die_by_tnf) {
        kill(abcell);
        if (print_test){std::cout<<"killing abc cell at time alive or TNF"<<std::endl;}

    }
}

void World::isotypeSwitch(std::shared_ptr<ActivatedBCell> activated_b_cell) {
    // Get the patch where the activated b cell is
    Patch& activated_b_cell_patch = get_patch(activated_b_cell->getX(), activated_b_cell->getY());

    if (!activated_b_cell->getCsrBool()) {
        double igM_bucket = activated_b_cell_patch.getIl12() + activated_b_cell_patch.getIl15() + activated_b_cell_patch.getIl6();
        double igA_bucket = activated_b_cell_patch.getIl10() + activated_b_cell_patch.getIl15() + activated_b_cell_patch.getIl21() + activated_b_cell_patch.getTgfB();
        double igG_bucket = activated_b_cell_patch.getIl4() + activated_b_cell_patch.getIl10() + activated_b_cell_patch.getIl15() + activated_b_cell_patch.getIl21();
        double igE_bucket = activated_b_cell_patch.getIl4() - activated_b_cell_patch.getIl12() - activated_b_cell_patch.getIfA() - activated_b_cell_patch.getIfG() - activated_b_cell_patch.getTgfB() + activated_b_cell_patch.getIl21();

        std::vector<double> mylist = {igM_bucket, igA_bucket, igG_bucket, igE_bucket};
        int max_index = std::max_element(mylist.begin(), mylist.end()) - mylist.begin();

        if (max_index == 0) {
            activated_b_cell->setCsrBool(true);
            activated_b_cell->setIsotype("m");
        } else if (max_index == 1) {
            activated_b_cell->setCsrBool(true);
            activated_b_cell->setIsotype("a");
        } else if (max_index == 2) {
            activated_b_cell->setCsrBool(true);
            activated_b_cell->setIsotype("g");
        } else if (max_index == 3) {
            activated_b_cell->setCsrBool(true);
            activated_b_cell->setIsotype("e");
        }
    }
}




void World::tdResponse(std::shared_ptr<ActivatedBCell> activated_b_cell) {
    // Try to get Tfh cell in the current location of the activated B cell
    auto tfh = getOneTfhHere(activated_b_cell->getX(), activated_b_cell->getY());

    // If a Tfh cell is found, create a new GcB cell and link it with the Tfh cell
    if (tfh) {
      if (tfh->getBcellBindingStatus()){std::cout<<"TFH CELL IS ALREADY BOUND TO; skipping"<<std::endl; return;}
        auto new_gc_b_cell = std::make_shared<GCBCell>(activated_b_cell->getX(), activated_b_cell->getY(), global_ID_counter++, activated_b_cell->getHeading());
        // std::cout<< "activated b cell doing tdResponse at "<< activated_b_cell->getX()<<", "<<activated_b_cell->getY()<< "for ID "<<activated_b_cell->getID()<<std::endl;
        new_gc_b_cell->copy_other_turtle_attributes(activated_b_cell);
        new_gc_b_cell->setProBreg(0);
        new_gc_b_cell->setColor("orange");
        new_gc_b_cell->setShape("circle");
        new_gc_b_cell->setSize(1);
        new_gc_b_cell->setTimeAlive(0);

        std::weak_ptr<Turtle> new_gc_b_cell_weak_ptr = new_gc_b_cell;
        all_turtles.push_back(new_gc_b_cell_weak_ptr);
        all_gcb_cells.push_back(new_gc_b_cell);
        get_patch(new_gc_b_cell->getX(), new_gc_b_cell->getY()).add_turtle(new_gc_b_cell);

        // createLinkWith(new_gc_b_cell, tfh);
        new_gc_b_cell->addLinkedTurtle(tfh);
        tfh->setEbi2rLevel(0);
        tfh->setCcr7Level(0);
        tfh->setBcellBindingStatus(true);

        kill(activated_b_cell);
        return;
    } else {
        // If no Tfh cell is found, try to get a Th2 cell in the current location of the activated B cell
        auto th2 = getOneTh2Here(activated_b_cell->getX(), activated_b_cell->getY());

        // If a Th2 cell is found, create a new GcB cell and link it with the Th2 cell
        if (th2) {
          if (th2->getBcellBindingStatus()){std::cout<<"th2 CELL IS ALREADY BOUND TO; skipping"<<std::endl; return;}
          // std::cout<< "activated b cell doing tdResponse at "<< activated_b_cell->getX()<<", "<<activated_b_cell->getY()<< "for ID "<<activated_b_cell->getID()<<std::endl;

            auto new_gc_b_cell = std::make_shared<GCBCell>(activated_b_cell->getX(), activated_b_cell->getY(), global_ID_counter++, activated_b_cell->getHeading());
            new_gc_b_cell->copy_other_turtle_attributes(activated_b_cell);
            new_gc_b_cell->setProBreg(0);
            new_gc_b_cell->setColor("orange");
            new_gc_b_cell->setShape("circle");
            new_gc_b_cell->setSize(1);
            new_gc_b_cell->setTimeAlive(0);

            std::weak_ptr<Turtle> new_gc_b_cell_weak_ptr = new_gc_b_cell;
            all_turtles.push_back(new_gc_b_cell_weak_ptr);
            all_gcb_cells.push_back(new_gc_b_cell);
            get_patch(new_gc_b_cell->getX(), new_gc_b_cell->getY()).add_turtle(new_gc_b_cell);

            // createLinkWith(new_gc_b_cell, th2);
            new_gc_b_cell->addLinkedTurtle(th2);
            th2->setEbi2rLevel(0);
            th2->setCcr7Level(0);
            th2->setBcellBindingStatus(true);

            kill(activated_b_cell);
            return;
        }
    }
}


void World::tiResponse(std::shared_ptr<ActivatedBCell> activated_b_cell) {
    bool print_test = false;
    // Activated B-cells undergoing TI response secrete TNF-a
    auto current_patch = get_patch(activated_b_cell->getX(), activated_b_cell->getY());
    current_patch.setTnfA(current_patch.getTnfA() + 1);

    if (activated_b_cell->getTimeAlive() % 70 == 0) {
        double proPC = (current_patch.getIl21() + current_patch.getIl10() + current_patch.getIfA() + current_patch.getIfG()) * 10;
        double proMem = 0;
        if (proPC > proMem) {
            // Create a new SL Plasma Cell and add it to the corresponding containers
            auto new_sl_plasma_cell = std::make_shared<SLPlasmaCell>(activated_b_cell->getX(), activated_b_cell->getY(), global_ID_counter++);
            new_sl_plasma_cell->copy_other_turtle_attributes(activated_b_cell);
            new_sl_plasma_cell->setTimeAlive(0);
            // Set color, shape, size, etc. as required
            new_sl_plasma_cell->setColor("lime");
            new_sl_plasma_cell->setShape("circle");
            new_sl_plasma_cell->setSize(1);


            std::weak_ptr<Turtle> new_sl_plasma_cell_weak_ptr = new_sl_plasma_cell;
            all_turtles.push_back(new_sl_plasma_cell_weak_ptr);
            all_sl_plasma_cells.push_back(new_sl_plasma_cell);
            get_patch(new_sl_plasma_cell->getX(), new_sl_plasma_cell->getY()).add_turtle(new_sl_plasma_cell);
            if(print_test){std::cout<<"final SL plasma cell use count "<<new_sl_plasma_cell.use_count()<<std::endl;}
        } else {
            if (step < 2800) {
                // Create a new Memory B Cell and add it to the corresponding containers
                auto new_mem_b_cell = std::make_shared<MemBCell>(activated_b_cell->getX(), activated_b_cell->getY(), global_ID_counter++);
                new_mem_b_cell->copy_other_turtle_attributes(activated_b_cell);

                new_mem_b_cell->setExposureNumber(1);
                new_mem_b_cell->setTimeAlive(0);
                // Set color, shape, size, etc. as required
                new_mem_b_cell->setColor("pink");
                new_mem_b_cell->setShape("target");
                new_mem_b_cell->setSize(1);
                new_mem_b_cell->setS1pr1Level(10);
                new_mem_b_cell->setProBreg(0);
                new_mem_b_cell->setCd21Level(0);
                new_mem_b_cell->setCxcr5Level(10);

                std::weak_ptr<Turtle> new_mem_b_cell_weak_ptr = new_mem_b_cell;
                all_turtles.push_back(new_mem_b_cell_weak_ptr);
                all_mem_b_cells.push_back(new_mem_b_cell);
                get_patch(new_mem_b_cell->getX(), new_mem_b_cell->getY()).add_turtle(new_mem_b_cell);

            } else {
                // Create a new Memory B Cell and add it to the corresponding containers
                auto new_mem_b_cell = std::make_shared<MemBCell>(activated_b_cell->getX(), activated_b_cell->getY(), global_ID_counter++);
                new_mem_b_cell->copy_other_turtle_attributes(activated_b_cell);

                new_mem_b_cell->setExposureNumber(2);
                new_mem_b_cell->setTimeAlive(0);
                // Set color, shape, size, etc. as required
                new_mem_b_cell->setColor("pink");
                new_mem_b_cell->setShape("target");
                new_mem_b_cell->setSize(1);
                new_mem_b_cell->setS1pr1Level(10);
                new_mem_b_cell->setProBreg(0);
                new_mem_b_cell->setCd21Level(0);
                new_mem_b_cell->setCxcr5Level(10);

                std::weak_ptr<Turtle> new_mem_b_cell_weak_ptr = new_mem_b_cell;
                all_turtles.push_back(new_mem_b_cell_weak_ptr);
                all_mem_b_cells.push_back(new_mem_b_cell);
                get_patch(new_mem_b_cell->getX(), new_mem_b_cell->getY()).add_turtle(new_mem_b_cell);

            }
        }
    }
}
