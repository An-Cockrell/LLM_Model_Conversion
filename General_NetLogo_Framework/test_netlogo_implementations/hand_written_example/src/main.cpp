// // main.cpp
// #include "world.h"

// #include <chrono>
// // https://github.com/haotian-liu/LLaVA#llava-weights
// int main(int argc, char const *argv[]) {
//   std::cout<<"starting main"<<std::endl;
//   World world = World();
//   World* worldptr = &world; // pointer to give to rendering engine
  
//   // setting up world
//   world.setup();

//   world.updateTurtleVectors();

//   auto startTime = std::chrono::high_resolution_clock::now();
//   // main loop
//   std::cout<<"STARTING GO"<<std::endl;
//   for (size_t i = 0; i < NUM_STEPS; i++) {
//     std::cout<<"beginning step number " <<i<<std::endl;
//     world.go();
    
//   auto endTime = std::chrono::high_resolution_clock::now();
//   auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
//   std::cout << "Program duration: " << duration.count() << " milliseconds" << std::endl;

//   return 0;
//   }
// }