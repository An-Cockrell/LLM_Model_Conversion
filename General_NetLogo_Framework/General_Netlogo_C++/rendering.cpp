// rendering.cpp
#include "rendering.h"


RenderingEngine::RenderingEngine(){
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderClear(renderer);
}

RenderingEngine::RenderingEngine(World *world){
  myWorld = world;
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_RenderClear(renderer);
}

RenderingEngine::~RenderingEngine(){
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

std::tuple<int, int, int> RenderingEngine::getColor(const std::string& color){
  int red, green, blue;
  if (color == "red") {
      red = 255;
      green = 0;
      blue = 0;
  }
  else if (color == "green") {
      red = 0;
      green = 255;
      blue = 0;
  }
  else if (color == "cyan") {
    red = 0;
    green = 255;
    blue = 255;
  }
  else if (color == "blue") {
      red = 0;
      green = 0;
      blue = 255;
  }
  else if (color =="black"){
    red = 0;
    green = 0;
    blue = 0;
  }
  else if (color == "white"){
    red = 255;
    green = 255;
    blue = 255;
  }
  else if (color == "yellow"){
    red = 255;
    green = 255;
    blue = 0;
  }
  else if (color == "violet"){
    red = 127;
    green = 0;
    blue = 255;
  }
  else if (color =="brown"){
    red = 165;
    green = 42;
    blue = 42;
  }
  else if (color == "orange"){
    red = 255;
    green = 165;
    blue = 0;
  }
  else if (color == "grey"){
    red = 128;
    green = 128;
    blue = 128;
  }
  
  else {
      red = 100;
      green = 100;
      blue = 100;
  }
  return std::make_tuple(red, green, blue);
}

void RenderingEngine::render(){
  SDL_RenderPresent(renderer);
}

void RenderingEngine::setBackgroundColor(const std::string& color){
  std::tuple<int, int, int> RGB = getColor(color);
    SDL_SetRenderDrawColor(renderer, std::get<0>(RGB), std::get<1>(RGB), std::get<2>(RGB), SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    // SDL_RenderPresent(renderer);
}

void RenderingEngine::drawLine(double startX, double startY, double endX, double endY, const std::string& color, int alpha){
  std::tuple<int, int, int> RGB = getColor(color);
  SDL_SetRenderDrawColor(renderer, std::get<0>(RGB), std::get<1>(RGB), std::get<2>(RGB), alpha);
  SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
  // SDL_RenderPresent(renderer);
}

void RenderingEngine::drawCircle(double centerX, double centerY, double radius, const std::string& color, int alpha){
  centerX -= GRID_SIZE/2;
  centerY -= GRID_SIZE/2;
  std::tuple<int, int, int> RGB = getColor(color);
  SDL_SetRenderDrawColor(renderer, std::get<0>(RGB), std::get<1>(RGB), std::get<2>(RGB), alpha);

	for (int y = -radius; y <= radius; ++y) {
	    for (int x = -radius; x <= radius; ++x) {
	        if (x*x + y*y <= radius*radius) {
	            SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
	        }
	    }
	}
	// SDL_RenderPresent(renderer);
}

void RenderingEngine::drawSquare(int x, int y, int size, const std::string& color, int alpha) {
  std::tuple<int, int, int> RGB = getColor(color);
  SDL_SetRenderDrawColor(renderer, std::get<0>(RGB), std::get<1>(RGB), std::get<2>(RGB), alpha);
  SDL_Rect rect = { x, y, size, size };
  SDL_RenderFillRect(renderer, &rect);
  // SDL_RenderPresent(renderer);
}

void RenderingEngine::renderAllPatches(){
  for (int x = 0; x < WORLD_WIDTH; x++){
    for (int y = 0; y < WORLD_HEIGHT; y++){
      auto& patch = myWorld->get_patch(x,y);
      int patch_x = patch.getX() * GRID_SIZE;
      int patch_y = patch.getY() * GRID_SIZE;
      std::string color = patch.getColor();
      int opacity = patch.getOpacity();
      drawSquare(patch_x,patch_y,GRID_SIZE, color, opacity);
    }
  }
}

void RenderingEngine::renderAllTurtles(){
  int x, y;
  for (auto& agent : myWorld->all_turtles){
    x = agent.lock()->getX() * GRID_SIZE;
    y = agent.lock()->getY() * GRID_SIZE;
    std::string color = agent.lock()->getColor();
    int opacity = agent.lock()->getOpacity();
    drawCircle(x,y,(GRID_SIZE/2)-2, color, opacity);
  }
}