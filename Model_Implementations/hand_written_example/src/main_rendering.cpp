// main.cpp
#include "rendering.h"
#include "world.h"

#include <chrono>
// https://github.com/haotian-liu/LLaVA#llava-weights
int main(int argc, char const *argv[]) {
  std::cout<<"starting main"<<std::endl;
  World world = World();
  World* worldptr = &world; // pointer to give to rendering engine
  
  // setting up world
  world.setup();
  world.updateTurtleVectors();

  // setting up rendering engine
  RenderingEngine engine = RenderingEngine(worldptr);
  bool quit=false;
  SDL_Event event;


  auto startTime = std::chrono::high_resolution_clock::now();
  // main loop
  std::cout<<"STARTING GO"<<std::endl;
  for (size_t i = 0; i < NUM_STEPS; i++) {
    std::cout<<"beginning step number " <<i<<std::endl;
    world.go();
    
    // rendering loop
    if (RENDERING){
      engine.setBackgroundColor("black");
      engine.renderAllPatches();
      engine.renderAllTurtles();
      engine.render();
      SDL_PollEvent(&event);
      /* will pause every step until ctrl+c is pressed
      useful for debugging, but tgere is no way to break out of it.
      kind of annoying, only use it when NUM_STEPS is low*/
      
      // quit=false;
      // while (!quit) {
        //       while (SDL_PollEvent(&event)) {
          //           if (event.type == SDL_QUIT) {
            //               quit = true;
            //           }
            //       }
            //   }
      /* will loop through and display every turn until end of NUM_STEPS, can break out with ctrl+c */
      if (event.type == SDL_QUIT) {
        quit = true;
      }
      if(quit){break;}
    }
  }
  auto endTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
  std::cout << "Program duration: " << duration.count() << " milliseconds" << std::endl;

  return 0;
};