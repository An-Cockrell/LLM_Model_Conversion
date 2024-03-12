// rendering.h
#ifndef RENDERING_H
#define RENDERING_H

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <tuple>
#include "world.h"

#include "parameters.h"

class RenderingEngine {
private:
  /* data */
  SDL_Window* window;
  SDL_Renderer* renderer;
  World* myWorld;


public:
  const static int GRID_SIZE = 10;
  const static int SCREEN_WIDTH = WORLD_WIDTH * GRID_SIZE;
  const static int SCREEN_HEIGHT = WORLD_WIDTH * GRID_SIZE;
  RenderingEngine();
  RenderingEngine(World *world);
  virtual ~RenderingEngine ();
  
  std::tuple<int, int, int> getColor(const std::string& color);
  void drawCircle(double centerX, double centerY, double radius=GRID_SIZE/2, const std::string& color="red", int alpha=SDL_ALPHA_OPAQUE);
  void drawSquare(int x, int y, int size=GRID_SIZE, const std::string& color="red", int alpha=SDL_ALPHA_OPAQUE);
  void drawLine(double startX, double startY, double endX, double endY, const std::string& color="black", int alpha=SDL_ALPHA_OPAQUE);
  void setBackgroundColor(const std::string& color="red");
  void render();
  
  void renderAllPatches();
  void renderAllTurtles();
};


#endif