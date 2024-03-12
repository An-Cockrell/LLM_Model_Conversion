// world.cpp
#include "world.h"
#include "helper_functions.h"

// this is where general functions

void World::add_patch(int x, int y){
  all_patches[x][y] = Patch(x,y);
}

Patch& World::get_patch(double x_arg, double y_arg){
  int x = trunc(x_arg);
  int y = trunc(y_arg);
  x = (x+WORLD_WIDTH)%WORLD_WIDTH;
  y = (y+WORLD_HEIGHT)%WORLD_HEIGHT;
  return all_patches[x][y];
}

Patch& World::get_patch_ahead(std::shared_ptr<Turtle> turtle, float distance){
  // default value for distance=1
  // get the patch in front of the turtle, based on its current heading
  int heading = turtle->getHeading();
  int current_x = turtle->getX();
  int current_y = turtle->getY();

  float d_x = cos(heading * M_PI/180) * distance;   // converting degrees to radians and getting delta x and delta y
  float d_y = sin(heading * M_PI/180) * distance;

  int ahead_x = round(current_x + d_x);
  int ahead_y = round(current_y + d_y);

  return get_patch(ahead_x, ahead_y);
}

Patch& World::get_patch_ahead_right(std::shared_ptr<Turtle> turtle, float distance){
  // default value for distance=1
  // get the patch in front and to the right of the turtle, based on its current heading
  int heading = turtle->getHeading() - 45; // to look right 45 degrees
  int current_x = turtle->getX();
  int current_y = turtle->getY();

  float d_x = cos(heading * M_PI/180) * distance;   // converting degrees to radians and getting delta x and delta y
  float d_y = sin(heading * M_PI/180) * distance;

  int ahead_x = round(current_x + d_x);
  int ahead_y = round(current_y + d_y);

  return get_patch(ahead_x, ahead_y);
}

Patch& World::get_patch_ahead_left(std::shared_ptr<Turtle> turtle, float distance){
  // default value for distance=1
  // get the patch in front and to the right of the turtle, based on its current heading
  int heading = turtle->getHeading() + 45; // to look left 45 degrees
  int current_x = turtle->getX();
  int current_y = turtle->getY();

  float d_x = cos(heading * M_PI/180) * distance;   // converting degrees to radians and getting delta x and delta y
  float d_y = sin(heading * M_PI/180) * distance;

  int ahead_x = round(current_x + d_x);
  int ahead_y = round(current_y + d_y);

  return get_patch(ahead_x, ahead_y);
}

void World::setup_patches(){
  for(int x=0; x<WORLD_WIDTH; x++){
    for(int y=0; y<WORLD_HEIGHT; y++){
      add_patch(x,y);
      // set initial values on patches here; eg patch.setOxy(100) to set patch oxy to initial value
    }
  }
}

void World::display_patches(){
  // prints details about all patches as well as any turtles associated with them to the console
  for(auto &patchArr: all_patches){
    for(auto &patch: patchArr){
      patch.display();
    }
  }
}

void World::add_turtle(int x, int y, int id, int heading){
  // creates a new generic turtle and adds it to the patch, as well as the list of all turtles
  std::shared_ptr<Turtle> new_turtle = std::make_shared<Turtle>(x, y, id, heading);
  get_patch(x,y).add_turtle(new_turtle);
  all_turtles.emplace_back(new_turtle);
}

void World::place_turtle(double x, double y, std::shared_ptr<Turtle> turtle){
  // moves turtle from its original patch to a new patch
  Patch& original_patch = get_patch(turtle->getX(), turtle->getY());
  Patch& new_patch = get_patch(x, y);
  turtle->setX(trunc(x));
  turtle->setY(trunc(y));
  turtle->set_x_dec(x);
  turtle->set_y_dec(y);
  turtle->set_temp_x(x);
  turtle->set_temp_y(y);

  original_patch.remove_turtle(turtle);
  new_patch.add_turtle(turtle);
}

void World::move_turtle(std::shared_ptr<Turtle> turtle, float distance){
  // moves the turtle but asking the turtle to calculate its movement, and if there is space on the target_patch, the world executes the move
  // This is the way that turtls should really be moved in the simulation, because this is the highest level control and will move them on world, their patch, and internally

  turtle->wiggle(RNG_Engine);


  Patch &turtle_current_patch = get_patch(turtle->getX(), turtle->getY());
  std::pair<double,double> new_coords = turtle->move(distance); //moving along turtle.heading for the default value of 1 unit unless a parameter is given
  Patch& target_patch = get_patch(new_coords.first, new_coords.second);
  if (target_patch.is_at_capacity()){
    turtle->execute_move(false);
  }
  else{
    // moving linked turtles here
    float main_turtle_dx = new_coords.first - turtle->get_temp_x();
    float main_turtle_dy = new_coords.second - turtle->get_temp_y();
    if (turtle->getLinkedTurtles().size()>0){
      for (auto link : turtle->getLinkedTurtles()) {
        float current_x = link.lock()->get_x_dec();
        float current_y = link.lock()->get_y_dec();
        place_turtle(current_x+main_turtle_dx, current_y+main_turtle_dy, link.lock());
      }
    }
    turtle->execute_move(true);
    int temp = turtle_current_patch.getTurtlesHere().size();
    turtle_current_patch.remove_turtle(turtle);
    // if we do not actually remove the turtle from the patch, something is wrong
    if ((temp - turtle_current_patch.getTurtlesHere().size()) != 1){std::cout<<"We did not remove the turtle from a patch when moving it. current size is "<< turtle_current_patch.getTurtlesHere().size()<<". This is a problem and we will exit now"; exit(10);}
    // adding the turtle to the new patch (can be the same as the old patch)
    target_patch.add_turtle(turtle);
  }
}

void World::move_turtle_random_jump(std::shared_ptr<Turtle> turtle){
  // same as move turtle but instead of turtle-> move() we are using turtle->jumpRandom(RNG_Engine)
  Patch &turtle_current_patch = get_patch(turtle->getX(), turtle->getY());
  std::pair<int,int> new_coords = turtle->jumpRandom(RNG_Engine);

  Patch& target_patch = get_patch(new_coords.first, new_coords.second);
    if (turtle_current_patch == target_patch){
    return;
  } else if (target_patch.is_at_capacity()){
    turtle->execute_move(false);
  }
  else{
    turtle->execute_move(true);
    turtle_current_patch.remove_turtle(turtle);
    target_patch.add_turtle(turtle);
  }
}

void World::turtle_wiggle(std::shared_ptr<Turtle> turtle){
  // wiggles the turtles heading, then moves forward along the new heading for 1 unit
  turtle->wiggle(RNG_Engine);
  move_turtle(turtle);
}

void World::kill_turtle(std::shared_ptr<Turtle> turtle){
  // removed turtle from its patch
  turtle->set_is_alive(false);
  Patch &turtle_current_patch = get_patch(turtle->getX(), turtle->getY());
  turtle_current_patch.remove_turtle(turtle);
  turtle.reset();
}

//void kill(std::shared_ptr<T> &ptr) <-- templated function that will kill any turtle or subclass of turtle. implemented in header file

void World::updateTurtleVectors(){
    for(auto& turtle : all_turtles_to_kill){
      if (std::shared_ptr<Bacteria> bacteria = std::dynamic_pointer_cast<Bacteria>(turtle)) {
        all_bacterias.erase(std::remove(begin(all_bacterias), end(all_bacterias), bacteria), end(all_bacterias));
        bacteria.reset();
      } else if (std::shared_ptr<Antibodies> antibody = std::dynamic_pointer_cast<Antibodies>(turtle)) {
        all_antibodies.erase(std::remove(begin(all_antibodies), end(all_antibodies), antibody), end(all_antibodies));
      } else if (std::shared_ptr<FDCs> fdc = std::dynamic_pointer_cast<FDCs>(turtle)) {
        all_fdcs.erase(std::remove(begin(all_fdcs), end(all_fdcs), fdc), end(all_fdcs));
        fdc.reset();
      } else if (std::shared_ptr<NaiveBCell> naive_b_cell = std::dynamic_pointer_cast<NaiveBCell>(turtle)) {
        all_naive_b_cells.erase(std::remove(begin(all_naive_b_cells), end(all_naive_b_cells), naive_b_cell), end(all_naive_b_cells));
        naive_b_cell.reset();
      } else if (std::shared_ptr<ActivatedBCell> activated_b_cell = std::dynamic_pointer_cast<ActivatedBCell>(turtle)) {
        all_activated_b_cells.erase(std::remove(begin(all_activated_b_cells), end(all_activated_b_cells), activated_b_cell),end(all_activated_b_cells));
        activated_b_cell.reset();
      } else if (std::shared_ptr<GCBCell> gcb_cell = std::dynamic_pointer_cast<GCBCell>(turtle)) {
        all_gcb_cells.erase(std::remove(begin(all_gcb_cells), end(all_gcb_cells), gcb_cell), end(all_gcb_cells));
        gcb_cell.reset();
      } else if (std::shared_ptr<SLPlasmaCell> sl_plasma_cell = std::dynamic_pointer_cast<SLPlasmaCell>(turtle)) {
        all_sl_plasma_cells.erase(std::remove(begin(all_sl_plasma_cells), end(all_sl_plasma_cells), sl_plasma_cell), end(all_sl_plasma_cells));
        sl_plasma_cell.reset();
      } else if (std::shared_ptr<LLPlasmaCell> ll_plasma_cell = std::dynamic_pointer_cast<LLPlasmaCell>(turtle)) {
        all_ll_plasma_cells.erase(std::remove(begin(all_ll_plasma_cells), end(all_ll_plasma_cells), ll_plasma_cell), end(all_ll_plasma_cells));
        ll_plasma_cell.reset();
      } else if (std::shared_ptr<MemBCell> mem_b_cell = std::dynamic_pointer_cast<MemBCell>(turtle)) {
        all_mem_b_cells.erase(std::remove(begin(all_mem_b_cells), end(all_mem_b_cells), mem_b_cell), end(all_mem_b_cells));
        mem_b_cell.reset();
      } else if (std::shared_ptr<BregCell> breg_cell = std::dynamic_pointer_cast<BregCell>(turtle)) {
        all_breg_cells.erase(std::remove(begin(all_breg_cells), end(all_breg_cells), breg_cell), end(all_breg_cells));
        breg_cell.reset();
      } else if (std::shared_ptr<TfhCell> tfh_cell = std::dynamic_pointer_cast<TfhCell>(turtle)) {
        all_tfh_cells.erase(std::remove(begin(all_tfh_cells), end(all_tfh_cells), tfh_cell), end(all_tfh_cells));
        tfh_cell.reset();
      } else if (std::shared_ptr<Th0Cell> th0_cell = std::dynamic_pointer_cast<Th0Cell>(turtle)) {
        all_th0_cells.erase(std::remove(begin(all_th0_cells), end(all_th0_cells), th0_cell), end(all_th0_cells));
        th0_cell.reset();
      } else if (std::shared_ptr<Th1Cell> th1_cell = std::dynamic_pointer_cast<Th1Cell>(turtle)){
        all_th1_cells.erase(std::remove(begin(all_th1_cells), end(all_th1_cells), th1_cell), end(all_th1_cells));
        th1_cell.reset();
      } else if (std::shared_ptr<Th2Cell> th2_cell = std::dynamic_pointer_cast<Th2Cell>(turtle)) {
        all_th2_cells.erase(std::remove(begin(all_th2_cells), end(all_th2_cells), th2_cell), end(all_th2_cells));
        th2_cell.reset();
      }
      // std::cout<<"turtle ID "<<turtle->getID()<<" use count "<<turtle.use_count()<<std::endl;
      turtle.reset();

    }
    all_turtles_to_kill.clear();

    // erasing the turtle weak pointers that are now a null pointer because they have been reset
    all_turtles.erase(std::remove_if(all_turtles.begin(), all_turtles.end(),
    [](const std::weak_ptr<Turtle>& wp) {     //lambda function to check if weak pointer is expired (aka killed and reset() has been called)
        // if (wp.expired()){std::cout<<"Deleting expired Turtle"<<std::endl;}
        return wp.expired();
      }),
    all_turtles.end());
}

void World::setup(){
  // This function initializes the global variables and setus up the world
  global_ID_counter = 0;
  step = 0;
  time = 1;
  setup_patches(); // Initializing the patches

  // Clear all existing entities
  all_turtles.clear();
  all_bacterias.clear();
  all_antibodies.clear();
  all_fdcs.clear();
  all_naive_b_cells.clear();
  all_activated_b_cells.clear();
  all_gcb_cells.clear();
  all_sl_plasma_cells.clear();
  all_ll_plasma_cells.clear();
  all_mem_b_cells.clear();
  all_breg_cells.clear();
  all_tfh_cells.clear();
  all_th0_cells.clear();
  all_th1_cells.clear();
  all_th2_cells.clear();

  // Check if random runs is false, then set seed
  if (!RandomRuns) {
      set_rng_seed(RNG_SEED);
      std::cout<<"printing test RNG"<<std::endl;
      for (size_t i = 0; i < 10; i++) {
        std::cout<<RNG_Engine()<<std::endl;
      }
  }

  // Sets up the world structure
  // In NetLogo, the center of the world is 0,0
  // In C++, the bottom left of the world is 0,0
  // Therefore, we need to shift the patches
  int center_x = std::ceil(WORLD_WIDTH / 2);   //added the std::floor() because only in odd world sizes
  int center_y = std::ceil(WORLD_HEIGHT / 2);  //will it matter, but 101 is odd

  // Paracortex zone
  for (int x = center_x - 200; x <= center_x + 200; x++) {
      for (int y = center_y - 200; y <= center_y + 200; y++) {
          if (x >= 0 && x < WORLD_WIDTH && y >= 0 && y < WORLD_HEIGHT) {
              Patch& p = get_patch(x, y);
              p.setPatchType(1);
              p.setColor("gray");
          }
      }
  }

  // Follicle zone
  for (int x = center_x - 49; x <= center_x + 49; x++) {
      for (int y = center_y - 49; y <= center_y + 49; y++) {
          if (x >= 0 && x < WORLD_WIDTH && y >= 0 && y < WORLD_HEIGHT) {
            // Calculate the distance from the center
            int dist_x = center_x - x;
            int dist_y = center_y - y;
            if (dist_x * dist_x + dist_y * dist_y <= 49 * 49) {  // Radius of 49 units
                Patch& p = get_patch(x, y);
                p.setPatchType(0);
                p.setColor("black");
            }
          }
      }
  }

// Rendering orientation squares
  // get_patch(0,0).setColor("pink");
  // get_patch(0,WORLD_HEIGHT-1).setColor("green");
  // get_patch(WORLD_WIDTH-1,0).setColor("yellow");
  // get_patch(WORLD_WIDTH-1,WORLD_HEIGHT-1).setColor("orange");
  //
  // std::cout<<"Angle To neighbor 0-TL(degrees)"<<atan2(1,-1) * 180/M_PI<<std::endl;
  // std::cout<<"Angle To neighbor 1-T(degrees)"<<atan2(1,0) * 180/M_PI<<std::endl;
  // std::cout<<"Angle To neighbor 2-TR(degrees)"<<atan2(1,1) * 180/M_PI<<std::endl;
  // std::cout<<"Angle To neighbor 3-L(degrees)"<<atan2(0,-1) * 180/M_PI<<std::endl;
  // std::cout<<"Angle To neighbor 4-R(degrees)"<<atan2(0,1) * 180/M_PI<<std::endl;
  // std::cout<<"Angle To neighbor 5-BL(degrees)"<<atan2(-1,-1) * 180/M_PI<<std::endl;
  // std::cout<<"Angle To neighbor 6-B(degrees)"<<atan2(-1,0) * 180/M_PI<<std::endl;
  // std::cout<<"Angle To neighbor 7-BR(degrees)"<<atan2(-1,1) * 180/M_PI<<std::endl;
  //
  // get_patch(0,0).setColor("pink");
  // get_patch(1,0).setColor("blue");
  // get_patch(2,0).setColor("green");
  // get_patch(0,1).setColor("black");
  // get_patch(1,1).setColor("white");
  // get_patch(2,1).setColor("brown");
  // get_patch(0,2).setColor("yellow");
  // get_patch(1,2).setColor("orange");
  // get_patch(2,2).setColor("cyan");

  // Exit from follicle
  for (int y = center_y - 5; y <= center_y + 5; y++) {
      if (y >= 0 && y < WORLD_HEIGHT) {
          Patch& p1 = get_patch(center_x - 50, y);
          Patch& p2 = get_patch(center_x - 49, y);
          p1.setPatchType(2);
          p1.setColor("red");
          p2.setPatchType(2);
          p2.setColor("red");
      }
  }

  //I added this part. In order to initialize 100 (or however many) FDCs that are apropriate distances from the center and also from eachother, we need to generate their positions intentionally, and not randomly. To do this we should generate all of them ahead of time, then create the cells.
  std::vector<std::pair<int, int>> FDCcoordinates = generateCoordinates(100, center_x, center_y, 30, 3);
  // creates 100 coordinates, centered on our origin (center_x/y) within 30 units of the center, and each at least 3 units apart from eachother

  // using the above command, I told gpt to recreate the pmn example from the general world.h example, but doing a different initialization command
  for (size_t i = 0; i < FDCcoordinates.size(); i++) {
      int x = FDCcoordinates[i].first;  // x-coordinate of the FDC
      int y = FDCcoordinates[i].second; // y-coordinate of the FDC
      auto fdc = std::make_shared<FDCs>(x, y, global_ID_counter++); // creating the actual FDC turtle
      Patch& patch = get_patch(x, y); // get patch to add the turtle to

      // Set shape and color for the FDC
      fdc->setShape("square");
      fdc->setColor("brown");

      std::weak_ptr<Turtle> fdc_weak_ptr = fdc;   // creating a weak pointer to give to all turtles vector so that deleting fdc object kills it in the turtle list

      // adding new turtle to all turtles vector, and specific type vector
      all_turtles.push_back(fdc_weak_ptr);
      all_fdcs.push_back(fdc);
      patch.add_turtle(fdc); // add turtle to patch
  }

  int NUM_TFH_CELLS_TO_ADD = 50;
  for (int i = 0; i < NUM_TFH_CELLS_TO_ADD; i++) {
      // generate a new coordinate and verify if the patch at the coordinate is of type 1
      std::pair<int, int> coord;
      // do while loop. executes the codeblock once, then checks. while condition, it executes the block again then checks. Same thing as a normal while loop, but it will always do it at least once
      do {
          int random_x = RNG_Engine() % WORLD_WIDTH; //random X and Y position
          int random_y = RNG_Engine() % WORLD_HEIGHT;
          coord = std::make_pair(random_x, random_y);
      } while (get_patch(coord.first, coord.second).getPatchType() != 1);
      auto tfhCell = std::make_shared<TfhCell>(coord.first, coord.second, global_ID_counter++, RNG_Engine()%360); // create the TfhCell
      Patch& patch = get_patch(coord.first, coord.second); // get patch to add the turtle to

      // Set cell variables
      tfhCell->setTimeAlive(-1000);
      tfhCell->setShape("square");
      tfhCell->setColor("cyan");
      tfhCell->setCxcr5Level(11);
      tfhCell->setCcr7Level(6);
      tfhCell->setEbi2rLevel(5);
      tfhCell->setBcellBindingStatus(false);

      std::weak_ptr<Turtle> tfhCell_weak_ptr = tfhCell; // creating a weak pointer to give to all turtles vector

      // adding new turtle to all turtles vector, and specific type vector
      all_turtles.push_back(tfhCell_weak_ptr);
      all_tfh_cells.push_back(tfhCell);
      patch.add_turtle(tfhCell); // add turtle to patch
  }

  int NUM_TH1_CELLS_TO_ADD = 10;
  for (int i = 0; i < NUM_TH1_CELLS_TO_ADD; i++) {
      std::pair<int, int> coord;
      do {
          int random_x = RNG_Engine() % WORLD_WIDTH; //random X and Y position
          int random_y = RNG_Engine() % WORLD_HEIGHT;
          coord = std::make_pair(random_x, random_y);
      } while (get_patch(coord.first, coord.second).getPatchType() != 1);

      auto th1Cell = std::make_shared<Th1Cell>(coord.first, coord.second, global_ID_counter++, RNG_Engine()%360); // create the Th1Cell
      Patch& patch = get_patch(coord.first, coord.second); // get patch to add the turtle to

      // Set cell variables
      th1Cell->setTimeAlive(-1000);
      th1Cell->setShape("square");
      th1Cell->setColor("blue");
      th1Cell->setCxcr5Level(0);
      th1Cell->setCcr7Level(2);
      th1Cell->setEbi2rLevel(2);
      th1Cell->setBcellBindingStatus(false);

      std::weak_ptr<Turtle> th1Cell_weak_ptr = th1Cell; // creating a weak pointer to give to all turtles vector

      // adding new turtle to all turtles vector, and specific type vector
      all_turtles.push_back(th1Cell_weak_ptr);
      all_th1_cells.push_back(th1Cell);
      patch.add_turtle(th1Cell); // add turtle to patch
  }

  int NUM_TH2_CELLS_TO_ADD = 50;
  for (int i = 0; i < NUM_TH2_CELLS_TO_ADD; i++) {
      std::pair<int, int> coord;
      do {
          int random_x = RNG_Engine() % WORLD_WIDTH; //random X and Y position
          int random_y = RNG_Engine() % WORLD_HEIGHT;
          coord = std::make_pair(random_x, random_y);
      } while (get_patch(coord.first, coord.second).getPatchType() != 1);

      auto th2Cell = std::make_shared<Th2Cell>(coord.first, coord.second, global_ID_counter++, RNG_Engine()%360); // create the Th2Cell
      Patch& patch = get_patch(coord.first, coord.second); // get patch to add the turtle to

      // Set cell variables
      th2Cell->setTimeAlive(-1000);
      th2Cell->setShape("square");
      th2Cell->setColor("blue");
      th2Cell->setCxcr5Level(0);
      th2Cell->setCcr7Level(2);
      th2Cell->setEbi2rLevel(2);
      th2Cell->setBcellBindingStatus(false);

      std::weak_ptr<Turtle> th2Cell_weak_ptr = th2Cell; // creating a weak pointer to give to all turtles vector

      // adding new turtle to all turtles vector, and specific type vector
      all_turtles.push_back(th2Cell_weak_ptr);
      all_th2_cells.push_back(th2Cell);
      patch.add_turtle(th2Cell); // add turtle to patch
  }



  // Initialize global variables and counters
  days_passed = 0;

  // Reset ticks or equivalent time step counter
  step = 0;
}


void World::go() {
  bool print_test = false;
  calculateIncomingTNFaIL6Level();
  simulateBackgroundInflammation();

  // Update days_passed based on the step value.
  setDaysPassed(step / 48);

  spawnBCell();
  // spawnTh0Cell();

  // Cytokine release from paracortex
  for (int x = 0; x < WORLD_WIDTH; x++) {
      for (int y = 0; y < WORLD_HEIGHT; y++) {
          Patch& p = get_patch(x, y);
          if (p.getPatchType() == 1) {
              p.setCcl19Level(p.getCcl19Level() + 2);
              p.setEbi2Level(p.getEbi2Level() + 2);
          }
      }
  }

  // Cytokine release from follicle exit
  for (int x = 0; x < WORLD_WIDTH; x++) {
      for (int y = 0; y < WORLD_HEIGHT; y++) {
          Patch& p = get_patch(x, y);
          if (p.getPatchType() == 2) {
              p.setS1pLevel(p.getS1pLevel() + 2);
          }
      }
  }
  if (print_test){std::cout<<"go 1"<<std::endl;}
  // Ask all agents to perform their functions
  for (auto& fdc : all_fdcs){fdcFunction(fdc);}
  if (print_test){std::cout<<"go 2"<<std::endl;}

  for (auto& naiveBCell : all_naive_b_cells){naiveBCellFunction(naiveBCell);}
  if (print_test){std::cout<<"go 3"<<std::endl;}

  for (auto& activatedBCell : all_activated_b_cells){activatedBCellFunction(activatedBCell);}
  if (print_test){std::cout<<"go 4"<<std::endl;}

  for (auto& gcbCell : all_gcb_cells){gc_b_cell_function(gcbCell);}
  if (print_test){std::cout<<"go 5"<<std::endl;}

  for (auto& llPlasmaCell : all_ll_plasma_cells){ll_plasma_cell_function(llPlasmaCell);}
  if (print_test){std::cout<<"go 6"<<std::endl;}

  for (auto& slPlasmaCell : all_sl_plasma_cells){sl_plasma_cell_function(slPlasmaCell);}
  if (print_test){std::cout<<"go 7"<<std::endl;}

  for (auto& memBCell : all_mem_b_cells){memBCellFunction(memBCell);}
  if (print_test){std::cout<<"go 8"<<std::endl;}

  for (auto& antibody : all_antibodies){antibodiesFunction(antibody);}
  if (print_test){std::cout<<"go 9"<<std::endl;}

  for (auto& bregCell : all_breg_cells){bregFunction(bregCell);}
  if (print_test){std::cout<<"go 10"<<std::endl;}

  for (auto& tfhCell : all_tfh_cells){tfhCellFunction(tfhCell);}
  if (print_test){std::cout<<"go 11"<<std::endl;}

  for (auto& th0Cell : all_th0_cells){th0CellFunction(th0Cell);}
  if (print_test){std::cout<<"go 12"<<std::endl;}

  for (auto& th1Cell : all_th1_cells){th1CellFunction(th1Cell);}
  if (print_test){std::cout<<"go 13"<<std::endl;}

  for (auto& th2Cell : all_th2_cells){th2CellFunction(th2Cell);}
  if (print_test){std::cout<<"go 14"<<std::endl;}

  for (auto& bacteria : all_bacterias){bacteriaFunction(bacteria);}
  if (print_test){std::cout<<"go 15"<<std::endl;}

  update_chemokine_gradient();
  if (print_test){std::cout<<"go 16"<<std::endl;}
  check_overall_cd21_expression();
  if (print_test){std::cout<<"go 17"<<std::endl;}

  // Check if autoinoculate is active
  if(AUTOINOCULATE) {
      if(step == AUTOINOCULATE_FIRST_TIME) {
        std::cout<<"INNOCULATING FOR THE FIRST TIME NOWWWWWW"<<std::endl;
          auto_inoculate(FIRST_EXPOSURE_AMOUNT);
      }
      else if(step == AUTOINOCULATE_SECOND_TIME) {
        std::cout<<"INNOCULATING FOR THE SECONDDDDDDDDDD TIME NOWWWWWW"<<std::endl;
          auto_inoculate(SECOND_EXPOSURE_AMOUNT);
      }
      else if(step == STOP_EXPOSURE_TIME) {
          // stop();  // You might want to stop the simulation
      }
  }

  // Increase the step (equivalent to NetLogo's tick)
  step++;
  // end of go
  updateTurtleVectors(); // need to update turtle positions/delete dead turtles
  if (print_test){std::cout<<"go 18"<<std::endl;}

}


void World::update_chemokine_gradient(){
  diffuse();
  evaporate();
}


void World::diffuse(){
  // need to diffuse values from patches out to neighboring patches
  double factor_for_neighbors = 1.0/8; //normally eight neighbors in a 2D grid

  // follow this example format for each diffusing variable
  for (auto& patch_row : all_patches) {  // reset temp_var
    for (auto& patch : patch_row){
      patch.setTempS1pLevel(0);
      patch.setTempCxcl13Level(0);
      patch.setTempCcl19Level(0);
      patch.setTempEbi2Level(0);
      patch.setTempIl2(0);
      patch.setTempIl4(0);
      patch.setTempIl6(0);
      patch.setTempIl10(0);
      patch.setTempIl12(0);
      patch.setTempIl15(0);
      patch.setTempIl21(0);
      patch.setTempIfG(0);
      patch.setTempIfA(0);
      patch.setTempTnfA(0);
      patch.setTempTgfB(0);
    }
  }
  // start with Endotoxin
  // get totals for neighbors
  for (auto& patch_row : all_patches){
    for (auto& center_patch : patch_row){
      int center_patch_x_pos = center_patch.getX();
      int center_patch_y_pos = center_patch.getY();
      if (!TOROIDAL_WORLD){
        bool x_edge = (center_patch_x_pos==0 || center_patch_x_pos==(WORLD_WIDTH-1));
        bool y_edge = (center_patch_y_pos==0|| center_patch_y_pos==(WORLD_HEIGHT-1));
        if(x_edge && y_edge){factor_for_neighbors = (1.0/3);}
        else if (x_edge || y_edge){factor_for_neighbors=(1.0/5);}
        else{factor_for_neighbors=(1.0/8);}
      }
      for (int i=-1; i<2; i++){
        for (int j=-1; j<2; j++){
          if (i==0 && j==0) { // skipping center cell
            continue;
          }
          int neighbor_x, neighbor_y;
          if (TOROIDAL_WORLD){
            neighbor_x = fmod((center_patch_x_pos + i), WORLD_WIDTH); // getting modulo of position + x/y to wrap around toroidal world
            neighbor_y = fmod((center_patch_y_pos + j), WORLD_HEIGHT);
          } else{
            neighbor_x = center_patch_x_pos + i;
            neighbor_y = center_patch_y_pos + j;
            // if not toroidal world and neighbor is out of bounds (ie we are on the edge) then continue
            if ((neighbor_y >= WORLD_HEIGHT) || (neighbor_y < 0)){continue;}
            if ((neighbor_x >= WORLD_WIDTH) || (neighbor_x < 0)){continue;}
          }
          auto& neighbor_patch = get_patch(neighbor_x, neighbor_y);

          // evenly adding the amount diffused out from the center patch to neighbor patches
          neighbor_patch.setTempCxcl13Level(neighbor_patch.getTempCxcl13Level() + center_patch.getCxcl13Level() * factor_for_neighbors * CXCL13_LEVEL_DIFFUSION_FACTOR);
          neighbor_patch.setTempCcl19Level(neighbor_patch.getTempCcl19Level() + center_patch.getCcl19Level() * factor_for_neighbors * CCL19_LEVEL_DIFFUSION_FACTOR);
          neighbor_patch.setTempEbi2Level(neighbor_patch.getTempEbi2Level() + center_patch.getEbi2Level() * factor_for_neighbors * EBI2_LEVEL_DIFFUSION_FACTOR);
          neighbor_patch.setTempS1pLevel(neighbor_patch.getTempS1pLevel() + center_patch.getS1pLevel() * factor_for_neighbors * S1P_LEVEL_DIFFUSION_FACTOR);
          neighbor_patch.setTempIl2(neighbor_patch.getTempIl2() + center_patch.getIl2() * factor_for_neighbors * IL2_DIFFUSION_FACTOR);
          neighbor_patch.setTempIl4(neighbor_patch.getTempIl4() + center_patch.getIl4() * factor_for_neighbors * IL4_DIFFUSION_FACTOR);
          neighbor_patch.setTempIl6(neighbor_patch.getTempIl6() + center_patch.getIl6() * factor_for_neighbors * IL6_DIFFUSION_FACTOR);
          neighbor_patch.setTempIl10(neighbor_patch.getTempIl10() + center_patch.getIl10() * factor_for_neighbors * IL10_DIFFUSION_FACTOR);
          neighbor_patch.setTempIl12(neighbor_patch.getTempIl12() + center_patch.getIl12() * factor_for_neighbors * IL12_DIFFUSION_FACTOR);
          neighbor_patch.setTempIl15(neighbor_patch.getTempIl15() + center_patch.getIl15() * factor_for_neighbors * IL15_DIFFUSION_FACTOR);
          neighbor_patch.setTempIl21(neighbor_patch.getTempIl21() + center_patch.getIl21() * factor_for_neighbors * IL21_DIFFUSION_FACTOR);
          neighbor_patch.setTempIfG(neighbor_patch.getTempIfG() + center_patch.getIfG() * factor_for_neighbors * IF_G_DIFFUSION_FACTOR);
          neighbor_patch.setTempIfA(neighbor_patch.getTempIfA() + center_patch.getIfA() * factor_for_neighbors * IF_A_DIFFUSION_FACTOR);
          neighbor_patch.setTempTnfA(neighbor_patch.getTempTnfA() + center_patch.getTnfA() * factor_for_neighbors * TNF_A_DIFFUSION_FACTOR);
          neighbor_patch.setTempTgfB(neighbor_patch.getTempTgfB() + center_patch.getTgfB() * factor_for_neighbors * TGF_B_DIFFUSION_FACTOR);
        }
      }
    }
  }
  // set Endotoxin on each patch from the temp var, then reset tempvar
  for (auto& patch_row : all_patches){
    for (auto& patch : patch_row){
      // new total is starting amount minus the amount that diffused out, plus the amount that diffused in from neighbors
      patch.setS1pLevel((patch.getS1pLevel() * (1 - S1P_LEVEL_DIFFUSION_FACTOR)) + patch.getTempS1pLevel());
      // std::cout<<"S1p level in diffusion: "<<patch.getTempS1pLevel()<<std::endl;
      patch.setTempS1pLevel(0); // resetting

      patch.setCxcl13Level((patch.getCxcl13Level() * (1 - CXCL13_LEVEL_DIFFUSION_FACTOR)) + patch.getTempCxcl13Level());
      patch.setTempCxcl13Level(0); // resetting

      patch.setCcl19Level((patch.getCcl19Level() * (1 - CCL19_LEVEL_DIFFUSION_FACTOR)) + patch.getTempCcl19Level());
      patch.setTempCcl19Level(0); // resetting

      patch.setEbi2Level((patch.getEbi2Level() * (1 - EBI2_LEVEL_DIFFUSION_FACTOR)) + patch.getTempEbi2Level());
      patch.setTempEbi2Level(0); // resetting

      patch.setIl2((patch.getIl2() * (1 - IL2_DIFFUSION_FACTOR)) + patch.getTempIl2());
      patch.setTempIl2(0); // resetting

      patch.setIl4((patch.getIl4() * (1 - IL4_DIFFUSION_FACTOR)) + patch.getTempIl4());
      patch.setTempIl4(0); // resetting

      patch.setIl6((patch.getIl6() * (1 - IL6_DIFFUSION_FACTOR)) + patch.getTempIl6());
      patch.setTempIl6(0); // resetting

      patch.setIl10((patch.getIl10() * (1 - IL10_DIFFUSION_FACTOR)) + patch.getTempIl10());
      patch.setTempIl10(0); // resetting

      patch.setIl12((patch.getIl12() * (1 - IL12_DIFFUSION_FACTOR)) + patch.getTempIl12());
      patch.setTempIl12(0); // resetting

      patch.setIl15((patch.getIl15() * (1 - IL15_DIFFUSION_FACTOR)) + patch.getTempIl15());
      patch.setTempIl15(0); // resetting

      patch.setIl21((patch.getIl21() * (1 - IL21_DIFFUSION_FACTOR)) + patch.getTempIl21());
      patch.setTempIl21(0); // resetting

      patch.setIfG((patch.getIfG() * (1 - IF_G_DIFFUSION_FACTOR)) + patch.getTempIfG());
      patch.setTempIfG(0); // resetting

      patch.setIfA((patch.getIfA() * (1 - IF_A_DIFFUSION_FACTOR)) + patch.getTempIfA());
      patch.setTempIfA(0); // resetting

      patch.setTnfA((patch.getTnfA() * (1 - TNF_A_DIFFUSION_FACTOR)) + patch.getTempTnfA());
      patch.setTempTnfA(0); // resetting

      patch.setTgfB((patch.getTgfB() * (1 - TGF_B_DIFFUSION_FACTOR)) + patch.getTempTgfB());
      patch.setTempTgfB(0); // resetting
    }
  }
}

void World::evaporate(){
  // evaporate variables off of all patches
  for (auto& patch_row : all_patches) {
    for (auto& patch : patch_row){
      // Endotoxin as an example, amount to set is current amount times the evaporation factor
      patch.setCxcl13Level(patch.getCxcl13Level() * CXCL13_EVAPORATION_FACTOR);
      patch.setCcl19Level(patch.getCcl19Level() * CCL19_EVAPORATION_FACTOR);
      patch.setEbi2Level(patch.getEbi2Level() * EBI2_EVAPORATION_FACTOR);
      patch.setS1pLevel(patch.getS1pLevel() * S1P_EVAPORATION_FACTOR);
      patch.setIl2(patch.getIl2() * IL2_EVAPORATION_FACTOR);
      patch.setIl4(patch.getIl4() * IL4_EVAPORATION_FACTOR);
      patch.setIl6(patch.getIl6() * IL6_EVAPORATION_FACTOR);
      patch.setIl10(patch.getIl10() * IL10_EVAPORATION_FACTOR);
      patch.setIl12(patch.getIl12() * IL12_EVAPORATION_FACTOR);
      patch.setIl15(patch.getIl15() * IL15_EVAPORATION_FACTOR);
      patch.setIl21(patch.getIl21() * IL21_EVAPORATION_FACTOR);
      patch.setIfG(patch.getIfG() * IF_G_EVAPORATION_FACTOR);
      patch.setIfA(patch.getIfA() * IF_A_EVAPORATION_FACTOR);
      patch.setTnfA(patch.getTnfA() * TNF_A_EVAPORATION_FACTOR);
      patch.setTgfB(patch.getTgfB() * TGF_B_EVAPORATION_FACTOR);
    }
  }
}
