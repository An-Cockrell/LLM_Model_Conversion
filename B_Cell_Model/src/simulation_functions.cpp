#include "world.h"

// functions here are ones that are used in the simulation, but are not go or setup. Functions defined in world.cpp are more general or dont have to do much with the specific simulation, besudes setup and go


void World::auto_inoculate(int numBac) {

    int numPhagocytosedBac = std::min(static_cast<int>(round(numBac * 0.8)), 100);
    int numFreeBac = numBac - numPhagocytosedBac;

    // Simulating the "ask up-to-n-of num-phagocytosed-bac fdcs"
    // Let's say all_fdcs is your list of FDCs
    for (int i = 0; i < std::min(numPhagocytosedBac, static_cast<int>(all_fdcs.size())); i++) {
        int random_FDC_index = RNG_Engine() % all_fdcs.size();
        all_fdcs[random_FDC_index]->setResponsiveness(std::min(all_fdcs[random_FDC_index]->getResponsiveness() + 50, 100));
        all_fdcs[random_FDC_index]->setTimePresenting(0);
        all_fdcs[random_FDC_index]->setPresentedAntigen(BACTERIA_EPITOPE_TYPE);
        all_fdcs[random_FDC_index]->setColor("red");  // Assuming you have some way to represent color in your FDC class

        int rTI = RNG_Engine() % NUMBER_OF_TI_EPITOPES;
        int rTD = RNG_Engine() % NUMBER_OF_TD_EPITOPES;

        if (rTI > rTD) {
            all_fdcs[random_FDC_index]->setPresentedAntigenType(1); // 1 is TI
        } else {
            all_fdcs[random_FDC_index]->setPresentedAntigenType(2); // 2 is TD
        }
    }

    // Now simulating the "create-bacteria num-free-bac"
    std::cout<<"IN AUTOINOCULATE, ADDING NEW BACTERIA. TOTAL ADDED IS "<<numFreeBac<<std::endl;
    for (int i = 0; i < numFreeBac; i++) {
        std::shared_ptr<Bacteria> new_bacteria = std::make_shared<Bacteria>(WORLD_WIDTH-1, std::ceil(WORLD_HEIGHT/2), global_ID_counter++);

        new_bacteria->setColor("red");
        new_bacteria->setShape("bug");
        new_bacteria->setSize(BACTERIA_SIZE);
        new_bacteria->setTimeAlive(0);
        new_bacteria->setInBlood(false);
        new_bacteria->setEpitopeType(BACTERIA_EPITOPE_TYPE);
        new_bacteria->setNumTIep(NUMBER_OF_TI_EPITOPES);
        new_bacteria->setNumTDep(NUMBER_OF_TD_EPITOPES);

        new_bacteria->setS1pr1Level(0);
        new_bacteria->setS1pr2Level(0);
        new_bacteria->setCxcr5Level(0);
        new_bacteria->setCcr7Level(0);
        new_bacteria->setEbi2rLevel(0);

        std::weak_ptr<Turtle> new_bacteria_weak_ptr = new_bacteria;
        all_bacterias.push_back(new_bacteria);  // Assuming you have a list named all_bacteria
        all_turtles.push_back(new_bacteria_weak_ptr);
        get_patch(new_bacteria->getX(), new_bacteria->getY()).add_turtle(new_bacteria);
    }
}

void World::check_overall_cd21_expression() {
    if (step % 20 == 0) {   // Only calculating avg CD21 expression every 20 ticks to increase run speed
        double total_cd21_expression = 0;

        for (const auto& cell : all_naive_b_cells) {
            total_cd21_expression += cell->getCd21Level();
        }

        for (const auto& cell : all_mem_b_cells) {
            total_cd21_expression += cell->getCd21Level();
        }

        int total_cell_count = all_naive_b_cells.size() + all_mem_b_cells.size();

        if (total_cell_count != 0) {
            average_cd21_expression = total_cd21_expression / total_cell_count;
        } else {
            average_cd21_expression = 0;
        }
    }
}
void World::calculateIncomingTNFaIL6Level() {
    for (auto& patchRow : all_patches) {
      for (auto& patch : patchRow){
        patch.setTnfA(patch.getTnfA() + (countCells<Bacteria>(patch.getX(), patch.getY()) / 500.0));

        patch.setIl6(patch.getIl6() + (countCells<Bacteria>(patch.getX(), patch.getY()) / 500.0));
      }
    }
}

void World::simulateBackgroundInflammation() {
    for (auto& patchRow : all_patches) {
      for (auto& patch : patchRow){
        patch.setTnfA(patch.getTnfA() + BACKGROUND_TNFA);
        patch.setIl6(patch.getIl6() + BACKGROUND_IL6);
      }
    }
}

void World::spawnTh0Cell() {
    if(step % 20 == 0) {
      std::cout<<"SPAWNING TH0 CELL"<<std::endl;

        // Position of the new Th0-cell
        int cell_x, cell_y;

        // Keep generating random coordinates until we find a patch of type 1
        do {
            cell_x = RNG_Engine() % WORLD_WIDTH;
            cell_y = RNG_Engine() % WORLD_HEIGHT;
        } while(get_patch(cell_x, cell_y).getPatchType() != 1);

        // Increase global ID counter
        global_ID_counter++;
        // Create a new Th0-cell
        auto th0_cell = std::make_shared<Th0Cell>(cell_x, cell_y, global_ID_counter);
        std::weak_ptr<Turtle> th0_cell_weak_ptr = th0_cell;

        // Set the properties of the new Th0-cell
        th0_cell->setShape("square");
        th0_cell->setColor("yellow");
        th0_cell->setTimeAlive(0);
        th0_cell->setS1pr1Level(0);
        th0_cell->setS1pr2Level(0);
        th0_cell->setCxcr5Level(0);
        th0_cell->setCcr7Level(6);
        th0_cell->setEbi2rLevel(6);
        th0_cell->setInBlood(false);

        // Add the new Th0-cell to the list of all Th0-cells
        all_th0_cells.push_back(th0_cell);

        // Also add it to the list of all Turtles
        all_turtles.push_back(th0_cell_weak_ptr);

        get_patch(th0_cell->getX(), th0_cell->getY()).add_turtle(th0_cell);

    }
}

// Function to randomly return an antibody on a specific patch

std::shared_ptr<Antibodies> World::getOneAntibodyHere(int patchX, int patchY) {
    // Create a vector to store antibodies on the specified patch
    std::vector<std::shared_ptr<Antibodies>> antibodiesOnPatch;
    Patch& current_patch = get_patch(patchX, patchY);
    std::vector<std::shared_ptr<Turtle>> turtles_on_patch = current_patch.getTurtlesHere();
    // Find antibodies on the patch
    for (const auto& turtle : turtles_on_patch) {
      if (std::shared_ptr<Antibodies> antibody = std::dynamic_pointer_cast<Antibodies>(turtle)) {
        // The turtle is a NaiveBCell, add it to the corresponding vector
        antibodiesOnPatch.push_back(antibody);
      }
    }
    // Check if any antibodies are found on the patch
    if (antibodiesOnPatch.empty()) {
        return nullptr; // Return nullptr if no antibodies found on the patch
    }
    // Generate a random index to pick a random antibody from the antibodiesOnPatch vector
    int randomIndex = RNG_Engine() % antibodiesOnPatch.size();

    // Return the randomly selected antibody
    return antibodiesOnPatch[randomIndex];
}

// Function to randomly return an antibody on a specific patch
std::shared_ptr<FDCs> World::getOneFDCHere(int patchX, int patchY){
    // Create a vector to store antibodies on the specified patch
    std::vector<std::shared_ptr<FDCs>> FDCsOnPatch;
    Patch& current_patch = get_patch(patchX, patchY);
    std::vector<std::shared_ptr<Turtle>> turtles_on_patch = current_patch.getTurtlesHere();
    // Find antibodies on the patch
    for (const auto& turtle : turtles_on_patch) {
      if (std::shared_ptr<FDCs> fdc = std::dynamic_pointer_cast<FDCs>(turtle)) {
        // The turtle is a NaiveBCell, add it to the corresponding vector
        FDCsOnPatch.push_back(fdc);
      }
    }
    // Check if any antibodies are found on the patch
    if (FDCsOnPatch.empty()) {
        return nullptr; // Return nullptr if no antibodies found on the patch
    }
    // Generate a random index to pick a random antibody from the antibodiesOnPatch vector
    int randomIndex = RNG_Engine() % FDCsOnPatch.size();

    // Return the randomly selected antibody
    return FDCsOnPatch[randomIndex];
}

std::shared_ptr<Bacteria> World::getOneBacteriaHere(int patchX, int patchY){
    // Create a vector to store antibodies on the specified patch
    std::vector<std::shared_ptr<Bacteria>> bacteriaOnPatch;
    for (float x =-BACTERIA_SIZE/2; x <= BACTERIA_SIZE/2; x++){
      for (float y =-BACTERIA_SIZE/2; y <= BACTERIA_SIZE/2; y++){
        float x_to_get, y_to_get;
        if (!TOROIDAL_WORLD) {
          x_to_get = std::min(patchX+x,(float)WORLD_WIDTH-1);
          x_to_get = std::max(x_to_get,(float)0);
          y_to_get = std::min(patchY+y,(float)WORLD_HEIGHT-1);
          y_to_get = std::min(y_to_get,(float)0);
        } else {
          x_to_get = fmod(patchX+x,WORLD_WIDTH);
          y_to_get = fmod(patchY+y,WORLD_HEIGHT);
        }

        Patch& current_patch = get_patch(x_to_get, y_to_get);
        std::vector<std::shared_ptr<Turtle>> turtles_on_patch = current_patch.getTurtlesHere();
        // Find antibodies on the patch
        for (const auto& turtle : turtles_on_patch) {
          if (std::shared_ptr<Bacteria> bacteria = std::dynamic_pointer_cast<Bacteria>(turtle)) {
            // The turtle is a bacteria, add it to the corresponding vector
            // std::cout<<"FOUND BACTERIA ON PATCH"<<std::endl;
            bacteriaOnPatch.push_back(bacteria);
          }
        }
      }
    }

    // Check if any antibodies are found on the patch
    if (bacteriaOnPatch.empty()) {
        return nullptr; // Return nullptr if no antibodies found on the patch
    }
    // Generate a random index to pick a random Bacteria from the antibodiesOnPatch vector
    int randomIndex = RNG_Engine() % bacteriaOnPatch.size();

    // Return the randomly selected Bacteria
    return bacteriaOnPatch[randomIndex];
}
// Function to randomly return a Th2 cell on a specific patch
std::shared_ptr<Th2Cell> World::getOneTh2Here(int patchX, int patchY){
    std::vector<std::shared_ptr<Th2Cell>> Th2CellsOnPatch;
    Patch& current_patch = get_patch(patchX, patchY);
    std::vector<std::shared_ptr<Turtle>> turtles_on_patch = current_patch.getTurtlesHere();
    for (const auto& turtle : turtles_on_patch) {
      if (std::shared_ptr<Th2Cell> th2 = std::dynamic_pointer_cast<Th2Cell>(turtle)) {
        Th2CellsOnPatch.push_back(th2);
      }
    }
    if (Th2CellsOnPatch.empty()) {
        return nullptr;
    }
    int randomIndex = RNG_Engine() % Th2CellsOnPatch.size();
    return Th2CellsOnPatch[randomIndex];
}

// Function to randomly return a Tfh cell on a specific patch
std::shared_ptr<TfhCell> World::getOneTfhHere(int patchX, int patchY){
    std::vector<std::shared_ptr<TfhCell>> TfhCellsOnPatch;
    Patch& current_patch = get_patch(patchX, patchY);
    std::vector<std::shared_ptr<Turtle>> turtles_on_patch = current_patch.getTurtlesHere();
    for (const auto& turtle : turtles_on_patch) {
      if (std::shared_ptr<TfhCell> tfh = std::dynamic_pointer_cast<TfhCell>(turtle)) {
        TfhCellsOnPatch.push_back(tfh);
      }
    }
    if (TfhCellsOnPatch.empty()) {
        return nullptr;
    }
    int randomIndex = RNG_Engine() % TfhCellsOnPatch.size();
    return TfhCellsOnPatch[randomIndex];
}

// Function to randomly return an Activated B cell on a specific patch
std::shared_ptr<ActivatedBCell> World::getOneActivatedBCellHere(int patchX, int patchY){
    std::vector<std::shared_ptr<ActivatedBCell>> ActivatedBCellsOnPatch;
    Patch& current_patch = get_patch(patchX, patchY);
    std::vector<std::shared_ptr<Turtle>> turtles_on_patch = current_patch.getTurtlesHere();
    for (const auto& turtle : turtles_on_patch) {
      if (std::shared_ptr<ActivatedBCell> activated_b_cell = std::dynamic_pointer_cast<ActivatedBCell>(turtle)) {
        ActivatedBCellsOnPatch.push_back(activated_b_cell);
      }
    }
    if (ActivatedBCellsOnPatch.empty()) {
        return nullptr;
    }
    int randomIndex = RNG_Engine() % ActivatedBCellsOnPatch.size();
    return ActivatedBCellsOnPatch[randomIndex];
}


std::vector<std::shared_ptr<FDCs>> World::get_fdcs_with_no_presented_antigen(){
  std::vector<std::shared_ptr<FDCs>> FDCs_with_no_anitigen;
  for (auto fdc : all_fdcs){
    if (fdc->getPresentedAntigen() ==0) {
      FDCs_with_no_anitigen.push_back(fdc);
    }
  }
  return FDCs_with_no_anitigen;
}
