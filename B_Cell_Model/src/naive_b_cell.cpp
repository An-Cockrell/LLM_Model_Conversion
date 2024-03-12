#include "naive_b_cell.h"
#include "world.h"
#include "parameters.h"

NaiveBCell::NaiveBCell(int x, int y, int id, int heading) : Turtle(x, y, id, heading) {
    // Constructor
    // std::cout<<"creating naive B cell with ID "<<id<<std::endl;

}
// NaiveBCell use count after creation  1
// NaiveBCell use count after weap tr  1
// NaiveBCell use count after all cells pushback  2
// NaiveBCell use count after all turtles pushback  2
// NaiveBCell use count after all patch add  3

void World::spawnBCell() {
  bool print_test = false;
  if(step % 10 == 0) {
    // std::cout<<"SPAWNING B CELL"<<std::endl;
      // Position of the new Naive B-cell
      int cell_x = WORLD_WIDTH -2; //I added std::floor()
      int cell_y = std::floor(WORLD_HEIGHT / 2);

      // Create a new Naive B-cell
      auto b_cell = std::make_shared<NaiveBCell>(cell_x, cell_y, global_ID_counter++, 180);
      if (print_test){std::cout<< "NaiveBCell use count after creation  "<<b_cell.use_count()<<std::endl;}

      std::weak_ptr<Turtle> b_cell_weak_ptr = b_cell;
      if (print_test){std::cout<< "NaiveBCell use count after weap tr  "<<b_cell.use_count()<<std::endl;}

      // Set the properties of the new Naive B-cell
      b_cell->setShape("circle");
      b_cell->setColor("white");
      b_cell->setSize(1);
      b_cell->setTimeAlive(0);
      b_cell->setBcr(RNG_Engine() % 30);
      b_cell->setIsotype("md");
      b_cell->setS1pr1Level(0);
      b_cell->setS1pr2Level(0);
      b_cell->setCxcr5Level(16);
      b_cell->setCcr7Level(0);
      b_cell->setEbi2rLevel(0);
      b_cell->setCd21Level(0);
      b_cell->setInBlood(false);

      // Set thresholds
      b_cell->setTnfaThreshold(BCELL_TNFA_APOP_THRESHOLD + (RNG_Engine() % 50));
      b_cell->setBregThreshold(BCELL_BREG_DIFF_THRESHOLD + (RNG_Engine() % 50));

      // Add the new Naive B-cell to the list
      all_naive_b_cells.push_back(b_cell);
      if (print_test){std::cout<< "NaiveBCell use count after all cells pushback  "<<b_cell.use_count()<<std::endl;}

      all_turtles.push_back(b_cell_weak_ptr);
      if (print_test){std::cout<< "NaiveBCell use count after all turtles pushback  "<<b_cell.use_count()<<std::endl;}

      get_patch(b_cell->getX(), b_cell->getY()).add_turtle(b_cell);
      if (print_test){std::cout<< "NaiveBCell use count after all patch add  "<<b_cell.use_count()<<std::endl;}

  }
}

void World::naiveBCellFunction(std::shared_ptr<NaiveBCell> naive_b_cell) {
  if (!naive_b_cell->get_is_alive()) {return;}

  bool print_test = false;
  // std::cout<<"naive_b_cellfunction for ID number "<<naive_b_cell->getID()<<"Ebi2rLevel "<<naive_b_cell->getEbi2rLevel()<<std::endl;
  // Get the current patch of the naive B cell
  Patch& current_patch = get_patch(naive_b_cell->getX(), naive_b_cell->getY());

  // Calculate CD21 expression based on il6 and il10 levels in the current patch
  int cd21_level = 300 - (current_patch.getIl6() * 50 + current_patch.getIl10() * 0);
  // std::cout<<"setting cd21 level to be "<<cd21_level<<" for cell with ID "<<naive_b_cell->getID()<<std::endl;
  naive_b_cell->setCd21Level(cd21_level);

  // If naive B cell is in the follicle, it dies
  if (current_patch.getPatchType() == 2) {
    // std::cout<<"killing naive Bcell on follicle exit "<<naive_b_cell->getID()<<std::endl;
    // std::cout<<"naive_b_cell use count is "<<naive_b_cell.use_count()<<std::endl;
    kill(naive_b_cell);
    return;
  }
  auto apc = getOneFDCHere(naive_b_cell->getX(), naive_b_cell->getY());
  int random_encounter = RNG_Engine() % 100;
  auto antigen = getOneBacteriaHere(naive_b_cell->getX(), naive_b_cell->getY());

  if ((apc != nullptr && apc->getPresentedAntigen() != 0 && apc->getResponsiveness() > random_encounter) || antigen != nullptr){
    if (print_test){std::cout<<"STEPPING INTO NAIVE_B_CELL FUNCTION. ID is "<<naive_b_cell->getID()<<std::endl;}
    if(naive_b_cell->getCd21Level() > BCELL_CD21_ACTIVATION_THRESHOLD){
      if(print_test){std::cout<<"becoming activated B_cell"<<std::endl;}
      // creating a new activated_b_cell
      // std::cout<<"Before turning into activated b cell, original naive_b_cell use count is "<<naive_b_cell.use_count()<<std::endl;

      auto new_activated_b_cell = std::make_shared<ActivatedBCell>(naive_b_cell->getX(), naive_b_cell->getY(), global_ID_counter++, naive_b_cell->getHeading());
      if (print_test){std::cout<< "new_activated_b_cell use count upon creation "<<new_activated_b_cell.use_count()<<std::endl;}
      new_activated_b_cell->copy_other_turtle_attributes(naive_b_cell);
      if (print_test){std::cout<< "new_activated_b_cell use count after copy  "<<new_activated_b_cell.use_count()<<std::endl;}

      // std::cout<<"After creeating but before turning into activated b cell, original naive_b_cell use count is "<<naive_b_cell.use_count()<<std::endl;
      
      current_patch.setIl6(current_patch.getIl6() + PHAG_IL6_BURST);
      new_activated_b_cell->setProBreg(0);
      new_activated_b_cell->setShape("target");
      new_activated_b_cell->setSize(1);
      new_activated_b_cell->setColor("red");
      new_activated_b_cell->setCsrBool(false);
      new_activated_b_cell->setTimeAlive(0);

      if (print_test){std::cout<<"location of mew activated_b_Cell is "<<new_activated_b_cell->getX()<<", "<<new_activated_b_cell->getY()<<std::endl;}

      int rTI = RNG_Engine() % 100;
      int rTD = RNG_Engine() % 100;

      if(antigen != nullptr){
        if(rTI > rTD){
          new_activated_b_cell->setResponseType(1); // TI response
        } else{
          new_activated_b_cell->setResponseType(2); // TD response
          new_activated_b_cell->setCcr7Level(12);
          new_activated_b_cell->setEbi2rLevel(12);
        }
        if (print_test){std::cout<<"Killing antigen in naive_b_cell function "<<antigen->getID()<<". use count:"<<antigen.use_count()<<std::endl;}

        kill(antigen);
      }
      else if (apc != nullptr){
        if(rTI > rTD){
            new_activated_b_cell->setResponseType(1); // TI response
        }
        else{
            new_activated_b_cell->setResponseType(2); // TD response
            new_activated_b_cell->setCcr7Level(12);
            new_activated_b_cell->setEbi2rLevel(12);
        }
      }


      std::weak_ptr<Turtle> new_activated_b_cell_weak_ptr = new_activated_b_cell;
      if (print_test){std::cout<< "new_activated_b_cell use count after weak ptr  "<<new_activated_b_cell.use_count()<<std::endl;}

      all_activated_b_cells.push_back(new_activated_b_cell);
      if (print_test){std::cout<< "new_activated_b_cell use count after all_activated_b_cells pushback "<<new_activated_b_cell.use_count()<<std::endl;}
      all_turtles.push_back(new_activated_b_cell_weak_ptr);
      if (print_test){std::cout<< "new_activated_b_cell use count after turtles pushback  "<<new_activated_b_cell.use_count()<<std::endl;}
      
      get_patch(new_activated_b_cell->getX(), new_activated_b_cell->getY()).add_turtle(new_activated_b_cell);
      // current_patch.add_turtle(new_activated_b_cell);
      if (print_test){std::cout<< "new_activated_b_cell use count after patch add  "<<new_activated_b_cell.use_count()<<std::endl;}
      // std::cout<<"After turning into activated b cell, original naive_b_cell use count is "<<naive_b_cell.use_count()<<std::endl;
      // std::cout<<"After turning into activated b cell, original naive_b_cell ID is "<<naive_b_cell->getID()<<std::endl;
      kill(naive_b_cell);

      // std::shared_ptr<ActivatedBCell> naive_b_cell = new_activated_b_cell; // renaming naive_b_Cell so the rest of the function still works as expected
      // std::cout<< "new_activated_b_cell use count after rename  "<<new_activated_b_cell.use_count()<<std::endl;

      return;
    }
  }


  // if(print_test){std::cout<<"chomtaxin naive_b_cell"<<std::endl;}
  // std::cout<< "naive_b_cell use count before chemotaxis "<<naive_b_cell.use_count()<<std::endl;

  chemotaxis(naive_b_cell);
  // std::cout<< "naive_b_cell use count after chemotaxis "<<naive_b_cell.use_count()<<std::endl;
  // std::cout<<"Moving b_cell"<<std::endl;

  move_turtle(naive_b_cell);
  // std::cout<< "naive_b_cell use count after move turtle "<<naive_b_cell.use_count()<<std::endl;

  // Checks level of stimulation of b-reg differentiation
  bool turn_into_breg = checkBregStatus(naive_b_cell);
  if (turn_into_breg){
    if (print_test){std::cout<<"naive_b cell ID "<<naive_b_cell->getID()<<"turning into Breg"<<std::endl;}
    // cant do it like below, leads to weird error where orignal cell is overwritten in templated turnIntoBreg function.
    // std::shared_ptr<BregCell> naive_b_cell = turnIntoBreg(naive_b_cell);
    // need to do it like this instead
    std::shared_ptr<BregCell> output_b_cell = turnIntoBreg(naive_b_cell);
    std::shared_ptr<BregCell> naive_b_Cell;
    naive_b_Cell = output_b_cell;
  };

  // Checks level of TNF-a stimulation for apoptosis
  bool die_by_tnf = checkTNFStatus(naive_b_cell);

  // this slowly increases the # of s1p receptors (s1pr) in the naive b cell when the b-cell is old enough
  if(naive_b_cell->getTimeAlive() > 300)
  {
      naive_b_cell->setS1pr1Level(naive_b_cell->getS1pr1Level() + 0.5);
  }

  naive_b_cell->setTimeAlive(naive_b_cell->getTimeAlive() + 1);

  // Checks if the cell has lived beyond its maximum lifespan, if so, it will be removed
  if((naive_b_cell->getTimeAlive() > 1000) || die_by_tnf ) {
      kill(naive_b_cell);
  }


}
