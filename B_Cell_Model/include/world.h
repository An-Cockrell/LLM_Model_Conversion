// world.h
#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <memory>
#include <algorithm>
#include <random>


#include "parameters.h"
#include "agent.h"
#include "patch.h"
#include "turtle.h"
#include "bacteria.h"
#include "antibodies.h"
#include "fdcs.h"
#include "naive_b_cell.h"
#include "activated_b_cell.h"
#include "gc_b_cell.h"
#include "sl_plasma_cell.h"
#include "ll_plasma_cell.h"
#include "mem_b_cell.h"
#include "breg_cell.h"
#include "tfh_cell.h"
#include "th0_cell.h"
#include "th1_cell.h"
#include "th2_cell.h"

class World {
private:
  std::mt19937 RNG_Engine;
  int global_ID_counter;  //ID counter for creating new turtles to make sure each has a unique ID
  int step;
  int time;
  int loop_run;

  // NetLogo global variables
  int days_passed;
  double average_cd21_expression;
  int total_num_of_apoptosed_cells;

  Patch all_patches[WORLD_WIDTH][WORLD_HEIGHT];

  // Vectors of shared pointers to different turtle subclasses
  std::vector<std::shared_ptr<Bacteria>> all_bacterias;
  std::vector<std::shared_ptr<Antibodies>> all_antibodies;
  std::vector<std::shared_ptr<FDCs>> all_fdcs;
  std::vector<std::shared_ptr<NaiveBCell>> all_naive_b_cells;
  std::vector<std::shared_ptr<ActivatedBCell>> all_activated_b_cells;
  std::vector<std::shared_ptr<GCBCell>> all_gcb_cells;
  std::vector<std::shared_ptr<SLPlasmaCell>> all_sl_plasma_cells;
  std::vector<std::shared_ptr<LLPlasmaCell>> all_ll_plasma_cells;
  std::vector<std::shared_ptr<MemBCell>> all_mem_b_cells;
  std::vector<std::shared_ptr<BregCell>> all_breg_cells;
  std::vector<std::shared_ptr<TfhCell>> all_tfh_cells;
  std::vector<std::shared_ptr<Th0Cell>> all_th0_cells;
  std::vector<std::shared_ptr<Th1Cell>> all_th1_cells;
  std::vector<std::shared_ptr<Th2Cell>> all_th2_cells;
  std::vector<std::shared_ptr<Turtle>> all_turtles_to_kill;

  void set_rng_seed(int newSeed){RNG_Engine.seed(newSeed);}
  void add_patch(int x, int y);

public:
  std::vector<std::weak_ptr<Turtle>> all_turtles;

  World(){};

  Patch& get_patch(double x_arg, double y_arg);
  Patch& get_patch_ahead(std::shared_ptr<Turtle> turtle, float distance=1);
  Patch& get_patch_ahead_right(std::shared_ptr<Turtle> turtle, float distance=1);
  Patch& get_patch_ahead_left(std::shared_ptr<Turtle> turtle, float distance=1);

  void setup_patches();
  void display_patches();

  void add_turtle(int x, int y, int id, int heading=0);
  void place_turtle(double x, double y, std::shared_ptr<Turtle> turtle);
  void move_turtle(std::shared_ptr<Turtle> turtle, float distance=1);
  void move_turtle_random_jump(std::shared_ptr<Turtle> turtle);
  void turtle_wiggle(std::shared_ptr<Turtle> turtle);

  void kill_turtle(std::shared_ptr<Turtle> turtle);

  // templated function to kill any turtle or agent
  template <typename T>
  void kill(std::shared_ptr<T> &ptr){
    // std::cout<<"Killing turtle with ID "<<ptr->getID()<<std::endl;
    ptr->removeLinkedTurtle();
    all_turtles_to_kill.push_back(ptr);
    kill_turtle(ptr);
  }

  void updateTurtleVectors();

  void setup();
  void go();

  void check_overall_cd21_expression();
  void calculateIncomingTNFaIL6Level();
  void simulateBackgroundInflammation();
  void update_chemokine_gradient();
  void auto_inoculate(int numBac);

  void diffuse();
  void evaporate();

  void spawnTh0Cell();
  void spawnBCell();

  void fdcFunction(std::shared_ptr<FDCs> fdc);
  void antibodiesFunction(std::shared_ptr<Antibodies> antibody);
  void naiveBCellFunction(std::shared_ptr<NaiveBCell> naive_b_cell);
  void bregFunction(std::shared_ptr<BregCell> breg_cell);
  void activatedBCellFunction(std::shared_ptr<ActivatedBCell> activated_b_cell);
  void tdResponse(std::shared_ptr<ActivatedBCell> activated_b_cell);
  void tiResponse(std::shared_ptr<ActivatedBCell> activated_b_cell);
  void gc_b_cell_function(std::shared_ptr<GCBCell> gc_b_cell);
  void ll_plasma_cell_function(std::shared_ptr<LLPlasmaCell> ll_plasma_cell);
  void sl_plasma_cell_function(std::shared_ptr<SLPlasmaCell> sl_plasma_cell);
  void memBCellFunction(std::shared_ptr<MemBCell> naive_b_cell);
  void th0CellFunction(std::shared_ptr<Th0Cell> th0_cell);
  void tfhCellFunction(std::shared_ptr<TfhCell> tfh_cell);
  void th1CellFunction(std::shared_ptr<Th1Cell> th1_cell);
  void th2CellFunction(std::shared_ptr<Th2Cell> th2_cell);
  void bacteriaFunction(std::shared_ptr<Bacteria> bacteria);

  std::shared_ptr<Antibodies> getOneAntibodyHere(int xPos, int yPos);
  std::shared_ptr<FDCs> getOneFDCHere(int patchX, int patchY);
  std::shared_ptr<Bacteria> getOneBacteriaHere(int patchX, int patchY);
  std::shared_ptr<Th2Cell> getOneTh2Here(int patchX, int patchY);
  std::shared_ptr<TfhCell> getOneTfhHere(int patchX, int patchY);
  std::shared_ptr<ActivatedBCell> getOneActivatedBCellHere(int patchX, int patchY);
  std::vector<std::shared_ptr<FDCs>> get_fdcs_with_no_presented_antigen();

  // void checkTNFStatus(std::shared_ptr<T> cell) lives at bottom because of template declaration restrictions
  void isotypeSwitch(std::shared_ptr<ActivatedBCell> activated_b_cell);
  // Getters and setters for global variables
  int getDaysPassed() const { return days_passed; }
  void setDaysPassed(int days) { days_passed = days; }

  double getAverageCD21Expression() const { return average_cd21_expression; }
  void setAverageCD21Expression(double expr) { average_cd21_expression = expr; }

  int getTotalNumOfApoptosedCells() const { return total_num_of_apoptosed_cells; }
  void setTotalNumOfApoptosedCells(int num) { total_num_of_apoptosed_cells = num; }













  template <typename CellType>
  int countCells(int patchX, int patchY){
    int total_count = 0;
    Patch& current_patch = get_patch(patchX, patchY);
    std::vector<std::shared_ptr<Turtle>> turtles_on_patch = current_patch.getTurtlesHere();

    for (const auto& turtle : turtles_on_patch){
      if (std::dynamic_pointer_cast<CellType>(turtle)) {
        // The turtle is of type we are looking for, increment count
        total_count++;
      }
    }
    return total_count;
  }



  template <typename CellType>
  bool checkTNFStatus(CellType& cell){
// if returns true, that means this cell needs to die
    Patch& current_patch = get_patch(cell->getX(), cell->getY());
    current_patch.setTnfA(current_patch.getTnfA() - .01);
    cell->setTnfaStimulation(current_patch.getTnfA() * 100);

    return (cell->getTnfaStimulation() > cell->getTnfaThreshold());
  }

  template <typename CellType>
  bool checkBregStatus(CellType& cell){
    Patch& current_patch = get_patch(cell->getX(), cell->getY());
    if (cell->getProBreg() > cell->getBregThreshold()){
      return true;
    } else{
      float amount_to_set = (current_patch.getIl6() + current_patch.getIl21()) * 45;
      cell->setProBreg(amount_to_set);
      return false;
    }
  }

  template <typename CellType>
  std::shared_ptr<BregCell> turnIntoBreg(CellType& cell){
    // std::cout<<std::endl<<std::endl<<std::endl<<std::endl;
    // std::cout<<"turning into breg"<<std::endl;
    // std::cout<<"original cell ID "<<cell->getID()<< " and location "<<cell<<std::endl;
    CellType placeholder = cell;
    // std::cout<<"placeholder cell ID "<<placeholder->getID()<<" and location "<<placeholder<<std::endl;

    std::shared_ptr<BregCell> breg = std::make_shared<BregCell>(cell->getX(), cell->getY(), global_ID_counter++, cell->getHeading());
    // std::shared_ptr<BregCell> breg = std::make_shared<BregCell>(0, 0, 99,0);
    // std::cout<<"breg ID "<<breg->getID()<<" and location "<<breg<<std::endl;
    // std::cout<<"original cell ID "<<cell->getID()<< " and location "<<cell<<std::endl;
    // std::cout<<"placeholder cell ID "<<placeholder->getID()<<" and location "<<placeholder<<std::endl;
    // global_ID_counter++;
    // std::cout<<"breg ID "<<breg->getID()<<" and location "<<breg<<std::endl;
    // std::cout<<"original cell ID "<<cell->getID()<< " and location "<<cell<<std::endl;
    // std::cout<<"placeholder cell ID "<<placeholder->getID()<<" and location "<<placeholder<<std::endl;



    breg->copy_other_turtle_attributes(cell);
    breg->setSize(1);
    breg->setShape("circle");
    breg->setColor("violet");
    breg->setS1pr1Level(0);
    breg->setCxcr5Level(10);
    breg->setTimeAlive(0);

    std::weak_ptr<Turtle> breg_weak_ptr = breg;
    all_turtles.push_back(breg_weak_ptr);
    all_breg_cells.push_back(breg);
    get_patch(cell->getX(), cell->getY()).add_turtle(breg);
    // std::cout<<"breg ID "<<breg->getID()<<std::endl;
    // std::cout<<"original cell ID "<<cell->getID()<<std::endl;
    kill(cell);
    return breg;
  }

  template <typename CellType>
  void chemotaxis(CellType& cell){
    // std::cout<<"chemotaxing"<<std::endl;
    // std::cout<<"my location" <<cell->getX()<<", "<< cell->getY()<<std::endl;
    Patch& current_patch = get_patch(cell->getX(), cell->getY());
    double final_heading = cell->getHeading();
    std::shared_ptr<Bacteria> test_cell = std::dynamic_pointer_cast<Bacteria>(cell);
    bool print_test = false;
    // print_test = (test_cell != NULL);
    // if (print_test){print_test = bool(test_cell->getID() == 120);}
    // if (print_test){test_cell->setColor("orange"); test_cell->setShape("bug");}
    // if (print_test){
    //   print_test = (cell->getX() == WORLD_WIDTH);
    // }
    if(print_test){std::cout<<"1st time: ID "<<cell->getID()<<", HEADING "<<cell->getHeading()<<std::endl;};

    std::vector<Patch> neighbors;
    int count = 0;
    for (int x = -1; x < 2; x++) {
      for (int y = -1; y < 2; y++) {
        if (x==0 && y==0){continue;}

        if (!TOROIDAL_WORLD){
          if ((current_patch.getX() + x) >= WORLD_WIDTH){continue;}
          if ((current_patch.getX() + x) < 0){continue;}
          if ((current_patch.getY() + y) >= WORLD_HEIGHT){continue;}
          if ((current_patch.getY() + x) < 0){continue;}
        }
        // std::cout<<"neighbor "<<count<<" location"<<cell->getX() + x<<", "<< cell->getY()+ y <<std::endl;
        count ++;
        neighbors.push_back(get_patch(current_patch.getX() + x, current_patch.getY() + y));
      }
    }
    if (print_test){std::cout<<"chemotaxing2:    "<<count<<std::endl;}

    int random_max_patch_ind = RNG_Engine() % neighbors.size();
    Patch max_patch = neighbors[random_max_patch_ind];

    double s1pr1_weight = cell->getS1pr1Level() / 100.0;
    double max_s1p = 0;
    for (auto neighbor_patch: neighbors){
      if (neighbor_patch.getS1pLevel() > max_s1p) {
        // std::cout<<"Changing max patch"<<std::endl;
        max_s1p = neighbor_patch.getS1pLevel();
        max_patch = get_patch(neighbor_patch.getX(), neighbor_patch.getY());
      }
    }
    double angle_absolute_to_s1p = cell->angle_to(get_patch(max_patch.getX(), max_patch.getY()));
    double angle_relative_to_s1p = angle_absolute_to_s1p - cell->getHeading();
    if (angle_relative_to_s1p<-180){angle_relative_to_s1p+=360;}
    if (angle_relative_to_s1p>180){angle_relative_to_s1p-=360;}
    angle_relative_to_s1p = fmod(angle_relative_to_s1p,360);

    if (print_test){std::cout<<"my current position is "<<test_cell->getX()<<", "<<test_cell->getY()<<std::endl;
      std::cout<<"Max Patch position is "<<max_patch.getX()<<", "<<max_patch.getY()<<std::endl;}


    if(print_test){
      std::cout<<"2nd  time: ID "<<cell->getID()<<", HEADING "<< final_heading;
      std::cout<<" s1pr1_weight "<<s1pr1_weight<<std::endl;
      std::cout<<"angle_absolute_to_s1p  "<<angle_absolute_to_s1p<<std::endl;
      std::cout<<"angle_relative_to_s1p  "<<angle_relative_to_s1p<<std::endl;
    };
    final_heading += angle_relative_to_s1p*s1pr1_weight;


    // random_max_patch_ind = RNG_Engine() % neighbors.size();
    // max_patch = neighbors[random_max_patch_ind];
    double s1pr2_weight = cell->getS1pr2Level() / 100.0;
    double max_s1pr2 = 0;
    for (auto neighbor_patch: neighbors){
        if (neighbor_patch.getS1pLevel() > max_s1pr2) {
            max_s1pr2 = neighbor_patch.getS1pLevel();
            max_patch = get_patch(neighbor_patch.getX(), neighbor_patch.getY());
        }
    }
    double angle_absolute_to_s1pr2 = cell->angle_to(get_patch(max_patch.getX(), max_patch.getY()));
    double angle_relative_to_s1pr2 = angle_absolute_to_s1pr2 - cell->getHeading();
    if (angle_relative_to_s1pr2<-180){angle_relative_to_s1pr2+=360;}
    if (angle_relative_to_s1pr2>180){angle_relative_to_s1pr2-=360;}
    angle_relative_to_s1pr2 = fmod(angle_relative_to_s1pr2,360);

    if (print_test){std::cout<<"my current position is "<<test_cell->getX()<<", "<<test_cell->getY()<<std::endl;
      std::cout<<"Max Patch position is "<<max_patch.getX()<<", "<<max_patch.getY()<<std::endl;}


    final_heading += angle_relative_to_s1pr2*s1pr2_weight;
    if(print_test){std::cout<<"3rd time: ID "<<cell->getID()<<", HEADING "<<final_heading<<" s1pr2_weight "<<s1pr2_weight<<std::endl;
    std::cout<<"angle_absolute_to_s1pr2  "<<angle_absolute_to_s1pr2<<std::endl;
    std::cout<<"angle_relative_to_s1pr2  "<<angle_relative_to_s1pr2<<std::endl;
    };


    //for CxCR5/cxcl13
    // random_max_patch_ind = RNG_Engine() % neighbors.size();
    // max_patch = neighbors[random_max_patch_ind];
    double cxcr5_weight = cell->getCxcr5Level() / 100.0;
    double max_cxcl13 = 0;
    for (auto neighbor_patch: neighbors){
        if (neighbor_patch.getCxcl13Level() > max_cxcl13) {
            max_cxcl13 = neighbor_patch.getCxcl13Level();
            max_patch = get_patch(neighbor_patch.getX(), neighbor_patch.getY());
        }
    }
    double angle_absolute_to_cxcl13 = cell->angle_to(get_patch(max_patch.getX(), max_patch.getY()));
    double angle_relative_to_cxcl13 = angle_absolute_to_cxcl13 - cell->getHeading();
    if (angle_relative_to_cxcl13<-180){angle_relative_to_cxcl13+=360;}
    if (angle_relative_to_cxcl13>180){angle_relative_to_cxcl13-=360;}
    angle_relative_to_cxcl13 = fmod(angle_relative_to_cxcl13,360);

    if (print_test){std::cout<<"my current position is "<<test_cell->getX()<<", "<<test_cell->getY()<<std::endl;
      std::cout<<"Max Patch position is "<<max_patch.getX()<<", "<<max_patch.getY()<<std::endl;}

    final_heading += angle_relative_to_cxcl13*cxcr5_weight;
    if(print_test){std::cout<<"4th time: ID "<<cell->getID()<<", HEADING "<< final_heading<<" CXCR5 weight "<<cxcr5_weight<<std::endl;
    std::cout<<"angle_absolute_to_cxcl13  "<<angle_absolute_to_cxcl13<<std::endl;
    std::cout<<"angle_relative_to_cxcl13  "<<angle_relative_to_cxcl13<<std::endl;
    };

    // for ccr7/ccl19
    // random_max_patch_ind = RNG_Engine() % neighbors.size();
    // max_patch = neighbors[random_max_patch_ind];
    double cccl7_weight = cell->getCcr7Level() / 100.0;
    double max_ccl19 = 0;
    for (auto neighbor_patch: neighbors){
        if (neighbor_patch.getCcl19Level() > max_ccl19) {
            max_ccl19 = neighbor_patch.getCcl19Level();
            max_patch = get_patch(neighbor_patch.getX(), neighbor_patch.getY());
        }
    }
    double angle_absolute_to_ccl19 = cell->angle_to(get_patch(max_patch.getX(), max_patch.getY()));
    double angle_relative_to_ccl19 = angle_absolute_to_ccl19 - cell->getHeading();
    if (angle_relative_to_ccl19<-180){angle_relative_to_ccl19+=360;}
    if (angle_relative_to_ccl19>180){angle_relative_to_ccl19-=360;}
    angle_relative_to_ccl19 = fmod(angle_relative_to_ccl19,360);

    if (print_test){std::cout<<"my current position is "<<test_cell->getX()<<", "<<test_cell->getY()<<std::endl;
      std::cout<<"Max Patch position is "<<max_patch.getX()<<", "<<max_patch.getY()<<std::endl;}

    final_heading += angle_relative_to_ccl19*cccl7_weight;
    if(print_test){std::cout<<"5th time: ID "<<cell->getID()<<", HEADING "<< final_heading<<" CCcL7 weight "<<cccl7_weight<<std::endl;
    std::cout<<"angle_absolute_to_ccl19  "<<angle_absolute_to_ccl19<<std::endl;
    std::cout<<"angle_relative_to_ccl19  "<<angle_relative_to_ccl19<<std::endl;
    };

    // For ebi2r_level
    // random_max_patch_ind = RNG_Engine() % neighbors.size();
    // max_patch = neighbors[random_max_patch_ind];
    double ebi2r_weight = cell->getEbi2rLevel() / 100.0;
    double max_ebi2 = 0;
    for (auto neighbor_patch: neighbors){
        if (neighbor_patch.getEbi2Level() > max_ebi2) {

            max_ebi2 = neighbor_patch.getEbi2Level();
            max_patch = get_patch(neighbor_patch.getX(), neighbor_patch.getY());
        }
    }
    double angle_absolute_to_ebi2 = cell->angle_to(get_patch(max_patch.getX(), max_patch.getY()));
    double angle_relative_to_ebi2 = angle_absolute_to_ebi2 - cell->getHeading();
    if (angle_relative_to_ebi2<-180){angle_relative_to_ebi2+=360;}
    if (angle_relative_to_ebi2>180){angle_relative_to_ebi2-=360;}
    angle_relative_to_ebi2 = fmod(angle_relative_to_ebi2,360);

    if (print_test){std::cout<<"my current position is "<<test_cell->getX()<<", "<<test_cell->getY()<<std::endl;
      std::cout<<"Max Patch position is "<<max_patch.getX()<<", "<<max_patch.getY()<<std::endl;}



    final_heading += angle_relative_to_ebi2 * ebi2r_weight;
    if(print_test){std::cout<<"6th time: ID "<<cell->getID()<<", HEADING "<< final_heading<<" ebi2r_weight"<<ebi2r_weight<<std::endl;
    std::cout<<"angle_absolute_to_ebi2  "<<angle_absolute_to_ebi2<<std::endl;
    std::cout<<"angle_relative_to_ebi2  "<<angle_relative_to_ebi2<<std::endl;
    };


    if (final_heading > 180){final_heading-=360;}
    if (final_heading < -180){final_heading+=360;}
  cell->setHeading((int)final_heading%(360));
  // cell->setHeading(180);
  if(print_test){std::cout<<"FINAL TIME: ID "<<cell->getID()<<", HEADING "<<cell->getHeading()<<std::endl<<std::endl<<std::endl<<std::endl;};

  // std::cout<<"END CHEMOTAXING"<<std::endl;
  }


};


#endif
