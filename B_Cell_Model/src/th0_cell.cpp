#include "th0_cell.h"
#include "world.h"

Th0Cell::Th0Cell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
    // std::cout<<"creating TH 0 cell with ID "<<id<<std::endl;

}

void World::th0CellFunction(std::shared_ptr<Th0Cell> th0_cell) {
  if (!th0_cell->get_is_alive()) {return;}

  std::cout<<"TH 0 function"<<std::endl;
    // Get the current patch of the Th0 cell
    Patch& current_patch = get_patch(th0_cell->getX(), th0_cell->getY());

    double pro_TH1 = (current_patch.getIl12() + current_patch.getIfG()) * 100;
    double pro_TH2 = (current_patch.getIl10() + current_patch.getIl4()) * 100;
    double pro_TFH = (current_patch.getIl21() + current_patch.getIl12()) * 100;

    double rTH1 = fmod((double)RNG_Engine(), pro_TH1);
    double rTH2 = fmod((double)RNG_Engine(), pro_TH2);
    double rTFH = fmod((double)RNG_Engine(), pro_TFH);

    th0_cell->setR1(rTH1);
    th0_cell->setR2(rTH2);
    th0_cell->setRf(rTFH);

    if (rTH1 > rTH2 && rTH1 > rTFH) {
        th0_cell->setTh1Activation(th0_cell->getTh1Activation() + 1);
    }
    if (rTH2 > rTH1 && rTH2 > rTFH) {
        th0_cell->setTh2Activation(th0_cell->getTh2Activation() + 1);
    }
    if (rTFH > rTH1 && rTFH > rTH2) {
        th0_cell->setTfhActivation(th0_cell->getTfhActivation() + 1);
    }

    if (th0_cell->getTh1Activation() >= 20) {
        auto th1_cell = std::make_shared<Th1Cell>(th0_cell->getX(), th0_cell->getY(), global_ID_counter++, th0_cell->getHeading());
        th1_cell->copy_other_turtle_attributes(th0_cell);
        th1_cell->setColor("blue");
        th1_cell->setTimeAlive(0);
        th1_cell->setSize(1);
        th1_cell->setShape("circle");

        std::weak_ptr<Turtle> th1_cell_weak_ptr = th1_cell;
        all_turtles.push_back(th1_cell_weak_ptr);
        all_th1_cells.push_back(th1_cell);
        current_patch.add_turtle(th1_cell);

        kill(th0_cell);
        std::shared_ptr<Th1Cell> th0_cell = th1_cell;
    } else if (th0_cell->getTh2Activation() >= 20) {
        auto th2_cell = std::make_shared<Th2Cell>(th0_cell->getX(), th0_cell->getY(), global_ID_counter++, th0_cell->getHeading());
        th2_cell->copy_other_turtle_attributes(th0_cell);
        th2_cell->setColor("blue");
        th2_cell->setTimeAlive(0);
        th2_cell->setSize(1);
        th2_cell->setShape("circle");
        th2_cell->setBcellBindingStatus(false);

        std::weak_ptr<Turtle> th2_cell_weak_ptr = th2_cell;
        all_turtles.push_back(th2_cell_weak_ptr);
        all_th2_cells.push_back(th2_cell);
        current_patch.add_turtle(th2_cell);

        kill(th0_cell);
        std::shared_ptr<Th2Cell> th0_cell = th2_cell;

    } else if (th0_cell->getTfhActivation() >= 20) {
        auto tfh_cell = std::make_shared<TfhCell>(th0_cell->getX(), th0_cell->getY(), global_ID_counter++, th0_cell->getHeading());
        global_ID_counter++;
        tfh_cell->copy_other_turtle_attributes(th0_cell);
        tfh_cell->setCxcr5Level(10);
        tfh_cell->setColor("cyan");
        tfh_cell->setShape("circle");
        tfh_cell->setSize(1);
        tfh_cell->setTimeAlive(0);
        tfh_cell->setBcellBindingStatus(false);

        std::weak_ptr<Turtle> tfh_cell_weak_ptr = tfh_cell;
        all_turtles.push_back(tfh_cell_weak_ptr);
        all_tfh_cells.push_back(tfh_cell);
        current_patch.add_turtle(tfh_cell);

        kill(th0_cell);
        std::shared_ptr<TfhCell> th0_cell = tfh_cell;

    }
    std::cout<<"chemotaxin from TH0 "<<std::endl;
    chemotaxis(th0_cell);
    // std::cout<<"Moving th0"<<std::endl;

    move_turtle(th0_cell);

    th0_cell->setTimeAlive(th0_cell->getTimeAlive() + 1);
    if (th0_cell->getTimeAlive() > 300) {
      std::cout <<"killing th0 cell "<<th0_cell->getID()<<std::endl;
        kill(th0_cell);
    }
}
