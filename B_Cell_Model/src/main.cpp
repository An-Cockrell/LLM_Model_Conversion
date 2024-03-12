// main.cpp
#include "rendering.h"
#include "world.h"

#include <chrono>
#include <thread>

// https://github.com/haotian-liu/LLaVA#llava-weights
int main(int argc, char const *argv[]) {
  std::cout<<"starting main"<<std::endl;
  World world = World();
  World* worldptr = &world; // pointer to give to rendering engine
  RenderingEngine engine;
  // setting up world
  world.setup();
  // world.injure_sterile(INJURY_NUMBER);
  world.updateTurtleVectors();

  // setting up rendering engine
  if (RENDERING){
    engine.~RenderingEngine();
    new (&engine) RenderingEngine(worldptr);
  }
  bool quit=false;
  bool paused=false;
  SDL_Event event;


  auto startTime = std::chrono::high_resolution_clock::now();
  // main loop
  std::cout<<"STARTING GO"<<std::endl;
  for (size_t i = 0; i < NUM_STEPS; i++) {
    if (i%100==0){
      std::cout<<"#######################################################\n Beginning Step Number " <<i<<"\n#######################################################"<<std::endl;
    }
    if (!paused){
      world.go();
    }

    // rendering loop
    if (RENDERING){
      engine.setBackgroundColor("black");
      engine.renderAllPatches();
      engine.renderPatchMode();
      engine.renderAllTurtles();
      engine.render();
      while (SDL_PollEvent(&event)) {
           if (event.type == SDL_QUIT) {
               quit = true;
           }
           if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
               paused = !paused; // Toggle pause state
           }
       }

       if (quit) {
           break;
       }

       // While paused, keep checking for events and only proceed if space is pressed again
       while (paused) {
           while (SDL_PollEvent(&event)) {
               if (event.type == SDL_QUIT) {
                   quit = true;
                   paused = false;
               }
               if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
                   paused = !paused; // Toggle pause state
               }
           }
           std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Sleep for a short duration to avoid busy-waiting
       }
       std::this_thread::sleep_for(std::chrono::milliseconds(50));
   }


  }
  auto endTime = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
  std::cout << "Program duration: " << duration.count() << " milliseconds" << std::endl;

  return 0;
}
