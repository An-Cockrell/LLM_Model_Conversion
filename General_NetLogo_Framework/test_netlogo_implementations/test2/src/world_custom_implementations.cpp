#include "world.h"

std::shared_ptr<Turtle> World::create_deer_turtle(int x, int y, int id, int heading) {
   auto deer = std::make_shared<Turtle>(x, y, id, heading); // Creating a new deer turtle
   deer->setLabel("Deer");
   Patch& patch = get_patch(x, y); // Locating the patch to add the turtle
   std::weak_ptr<Turtle> deer_weak_ptr = deer; // Creating a weak pointer for the all_turtles vector
   all_turtles.push_back(deer_weak_ptr); // Adding a weak pointer for the all_turtles vector
   all_deers.push_back(deer); // Adding a shared pointer to the all_deers vector
   patch.add_turtle(deer); // Adding the turtle to the patch
   return deer; // Returning the created deer turtle
}

std::shared_ptr<Turtle> World::create_wolf_turtle(int x, int y, int id, int heading) {
   auto wolf = std::make_shared<Turtle>(x, y, id, heading); // Creating a new wolf turtle
   wolf->setLabel("Wolf");
   Patch& patch = get_patch(x, y); // Locating the patch to add the turtle
   std::weak_ptr<Turtle> wolf_weak_ptr = wolf; // Creating a weak pointer for the all_turtles vector
   all_turtles.push_back(wolf_weak_ptr); // Adding a weak pointer for the all_turtles vector
   all_wolves.push_back(wolf); // Adding a shared pointer to the all_wolves vector
   patch.add_turtle(wolf); // Adding the turtle to the patch
   return wolf; // Returning the created wolf turtle
}

void World::go() {
    // NetLogo "tick" equivalent in C++ is simply incrementing the time counter
    step++;

    // implement netlogo Go function here
   // Display deer
   std::cout << "\n--- Deers ---\n";
   for (int i = 0; i < all_deers.size(); ++i) {
       all_deers[i]->display();
   }

   // Display wolves
   std::cout << "\n--- Wolves ---\n";
   for (int i = 0; i < all_wolves.size(); ++i) {
       all_wolves[i]->display();
   }

    // end of go
    updateTurtleVectors(); // need to update turtle positions/delete dead turtles
}

// Sets up the initial patches in the world.
void World::setup_patches()
{
   for (int x = 0; x < WORLD_WIDTH; ++x) {
       for (int y = 0; y < WORLD_HEIGHT; ++y) {
           add_patch(x, y);
            Patch &new_patch = get_patch(x, y); 
            new_patch.set_grass(100); // Initialize grass for the new patch
       }
   }
}

void World::setup(){
  set_rng_seed(RNG_SEED);
  // This function initializes the global variables and setus up the world
  step = 0;
  time = 1;
  
  setup_patches(); // Initializing the patches
  
  global_ID_counter = 0;

   for (int i = 0; i < 100; i++) {
       int x, y;
       // Randomly generate deer position
       do {
           x = rand() % WORLD_WIDTH;
           y = rand() % WORLD_HEIGHT;
       } while (get_patch(x, y).is_at_capacity());
      auto deer = create_deer_turtle(x, y, global_ID_counter++, (rand() % 360)); // Randomly set the deer's heading
   }

   for (int i = 0; i < 15; i++) {
       int x, y;
       // Randomly generate wolf position
       do {
           x = rand() % WORLD_WIDTH;
           y = rand() % WORLD_HEIGHT;
       } while (get_patch(x, y).is_at_capacity());
      auto wolf = create_wolf_turtle(x, y, global_ID_counter++, (rand() % 360)); // Randomly set the wolf's heading
   }


}


void World::updateTurtleVectors(){
    // erasing the turtle weak pointers that are now a null pointer because they have been reset
    all_turtles.erase(std::remove_if(all_turtles.begin(), all_turtles.end(),
    [](const std::weak_ptr<Turtle>& wp) {     //lambda function to check if weak pointer is expired (aka killed and reset() has been called)
        return wp.expired();
      }),
    all_turtles.end());

    // now need to erase the shared pointers that were reset with kill()
    // for example:  all_pmns.erase(std::remove(all_pmns.begin(), all_pmns.end(), nullptr), all_pmns.end());
    // implement erasure for all turtle shared_ptr vectors here
}


void World::deer_function(Deer *deer) {
 deer_eat(deer);
}
// In the World.cpp file
void World::deer_eat(Deer *deer) {
 Patch *current_patch = get_patch(deer->get_x(), deer->get_y());
 int grass_amount = current_patch->get_grass();

 if (grass_amount > 30) {
   deer->eat_grass(grass_amount);
   current_patch->set_grass(0);
 }
}