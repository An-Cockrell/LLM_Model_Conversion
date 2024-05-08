// rendering.cpp
#include "rendering.h"


RenderingEngine::RenderingEngine(){
  std::cout<<"calling default Constructor"<<std::endl;

  // SDL_Init(SDL_INIT_VIDEO);
  // window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  // SDL_RenderClear(renderer);
}

RenderingEngine::RenderingEngine(World *world){
  std::cout<<"calling world Constructor"<<std::endl;
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
    red = 85;
    green = 42;
    blue = 42;
  }
  else if (color == "orange"){
    red = 255;
    green = 95;
    blue = 31;
  }
  else if (color == "grey" || color =="gray"){
    red = 128;
    green = 128;
    blue = 128;
  }
  else if (color =="pink"){
    red = 255;
    green = 16;
    blue = 240;
  }
  else if (color =="lime"){
    red = 50;
    green = 205;
    blue = 50;
  }
  
  else if (color == "mauve"){
    red = 172;
    blue = 79;
    green = 106;
  }


  else {
    std::cout<<"using OTHER color"<<std::endl;
    std::cout<<"color is "<<color<<std::endl;
    
      red = 255;
      green = 16;
      blue = 240;
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
  centerX += GRID_SIZE/2;
  centerY += GRID_SIZE/2;
  std::tuple<int, int, int> RGB = getColor(color);
  SDL_SetRenderDrawColor(renderer, std::get<0>(RGB), std::get<1>(RGB), std::get<2>(RGB), alpha);

	for (int y = -radius; y <= radius; y++) {
	    for (int x = -radius; x <= radius; x++) {
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

void RenderingEngine::drawFilledEllipse(int centerX, int centerY, int radiusX, int radiusY, const std::string& color, int alpha, int angleDegrees) {
    std::tuple<int, int, int> RGB = getColor(color);
    SDL_SetRenderDrawColor(renderer, std::get<0>(RGB), std::get<1>(RGB), std::get<2>(RGB), alpha);
    double angleRadians = (90 + angleDegrees) * M_PI / 180.0;
    int major_axis = std::max(radiusX, radiusY);
    for (int x = centerX - major_axis; x <= centerX + major_axis; ++x) {
        for (int y = centerY - major_axis; y <= centerY + major_axis; ++y) {
            // Compute the position of (x, y) in the rotated ellipse's frame of reference
            double rotatedX = (x - centerX) * cos(-angleRadians) - (y - centerY) * sin(-angleRadians);
            double rotatedY = (x - centerX) * sin(-angleRadians) + (y - centerY) * cos(-angleRadians);

            // Check if this rotated point lies within the unrotated ellipse
            if ((rotatedX * rotatedX) / (radiusX * radiusX) + (rotatedY * rotatedY) / (radiusY * radiusY) <= 1) {
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
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

void RenderingEngine::renderPatchMode(){
  for (int x = 0; x < WORLD_WIDTH; x++){
    for (int y = 0; y < WORLD_HEIGHT; y++){
      auto& patch = myWorld->get_patch(x,y);
      int patch_x = patch.getX() * GRID_SIZE;
      int patch_y = patch.getY() * GRID_SIZE;
      std::string color = patch.getModeColor();
      int opacity = patch.getModeOpacity() * (255/3);
      if (opacity > 255){opacity=255;}
      // if (opacity>0){std::cout<<opacity<<std::endl;}
      // std::cout<<"opacity "<<opacity<<std::endl;
      // 
      // float opacity_scaling_value = (opacity - 0.001) / (3-0.001);
      // if (opacity_scaling_value>1){opacity_scaling_value=1;}
      // if (opacity_scaling_value<0){opacity_scaling_value=0;}
      // opacity = opacity / (3/2);
      // opacity = opacity_scaling_value * 255;
      drawSquare(patch_x,patch_y,GRID_SIZE, color, opacity);
    }
  }
}

void RenderingEngine::renderAllTurtles(){
  int x, y;
  for (auto& agent : myWorld->all_turtles){
    if (agent.lock()->getVisible() == false){continue;}
    x = agent.lock()->getX() * GRID_SIZE;
    y = agent.lock()->getY() * GRID_SIZE;
    std::string color = agent.lock()->getColor();
    int opacity = agent.lock()->getOpacity();
    std::string shape = agent.lock()->getShape();
    if (shape=="square"){
      drawSquare(x,y,(GRID_SIZE-1) * agent.lock()->getSize(), color, opacity);
    }else if(shape=="target"){
      drawSquare(x,y,(GRID_SIZE-1) * agent.lock()->getSize(), color, opacity);
      drawCircle(x-1,y-1,(((GRID_SIZE/2)*agent.lock()->getSize())-2), "black", opacity);
    }else if (shape=="bug"){
      // going to be an elipse with two little antennae on the front 
      int angle = agent.lock()->getHeading();
      
      drawFilledEllipse(x,y, (GRID_SIZE-1) * agent.lock()->getSize()/2, (GRID_SIZE-1) * agent.lock()->getSize()/4, color, opacity, angle);
      drawLine(x,y, x+cos(angle*M_PI/180)*GRID_SIZE* agent.lock()->getSize(), y - sin(angle*M_PI/180)*GRID_SIZE* agent.lock()->getSize(), color, opacity);
      // bounding rect
      // SDL_Rect ellipseRect = 
      
      // for (int x_draw=-1 * agent.lock()->getSize()*GRID_SIZE; x_draw <= agent.lock()->getSize()*GRID_SIZE; x_draw++){
      //   for (int y_draw=-1 * agent.lock()->getSize()*GRID_SIZE; y_draw <= agent.lock()->getSize()*GRID_SIZE; y_draw++){
      //     if (x_draw < ){
      // 
      //     };
      // 
      //   }
      // }
      
      
      // float x_offset = sin(angle) * (GRID_SIZE/2);
      // float y_offset = cos(angle) * (GRID_SIZE/2);
      // 
      // drawSquare(x,y,(GRID_SIZE-2) * agent.lock()->getSize(), color, opacity);
      // drawCircle(x + x_offset,y + y_offset,(((GRID_SIZE/2)*agent.lock()->getSize())-2), color, opacity);

    }else{
      drawCircle(x,y,((GRID_SIZE/2)), color, opacity);

    }
  }
}
