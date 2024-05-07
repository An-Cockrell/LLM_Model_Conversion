// agent.h
#ifndef AGENT_H
#define AGENT_H

#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include "parameters.h"

class Agent: public std::enable_shared_from_this<Agent> {
  protected:
    Agent(int x, int y) : x(x), y(y) {}
  private:
    std::string label = ""; // label for info about what type of cell this is
    std::string color = ""; // color of the agent to be displayed
    std::string secondaryColor = ""; // secondary color for displaying specific tracking things
    std::string shape = ""; // shape of the agent to render
    int size=1; // size of the agent to render
    int alpha = 255;  // opacity of the agent to render
    int x;  // x coordinate as an int
    int y;  // y coordinate as an int
    bool visible = true;  // whether or not to render this agent
  public:
    // position getters and setters
    virtual int getX() const {return x;};
    virtual int getY() const {return y;};
    virtual void setX(int newX){x=newX;};
    virtual void setY(int newY){y=newY;};
    // label getters and setters
    void setLabel(std::string newLabel){label = newLabel;};
    std::string getLabel() const {return label;};
    //color getters and setters
    void setColor(std::string newColor){color = newColor;};
    std::string getColor() const {return color;};
    void setSecondaryColor(std::string newColor){secondaryColor = newColor;};
    std::string getSecondaryColor() const {return secondaryColor;};
    // shape getter and setter
    void setShape(std::string newShape){shape = newShape;};
    std::string getShape() const {return shape;};
    // opacity getter and setter
    void setOpacity(int newAlpha){alpha = newAlpha;};
    int getOpacity() const {return alpha;};

    // size getter and setter
    void setSize(int newSize){size = newSize;};
    int getSize() const {return size;};

    // visibility getter and setter
    void setVisible(bool status){visible = status;}
    bool getVisible() const {return visible;}

    // function that creates and returns a weak ptr to self
    std::weak_ptr<Agent> createWeakPtr() {std::cout<<"getting pointer from this in agent" <<std::endl; return shared_from_this();}

    // templated function to get absolute angle from self to a target agent. Angle is given according to world coordinate system. returns in degrees
    template <typename agent>
    double angle_to(agent& target){
      int targetx = target.getX();
      int targety = target.getY();

      int delta_x = targetx - x;
      int delta_y = y - targety; //do this opposite to X because the origin is in the top left corner

      // dealing with when the world wraps and target is all the way on the left, and from is all the way on the right
      // it would give a delta_x close to -WORLD_WIDTH,
      if ((delta_x + WORLD_WIDTH) < abs(delta_x)){
        delta_x = (delta_x + WORLD_WIDTH)% WORLD_WIDTH;
      }
      if ((delta_y + WORLD_HEIGHT) < abs(delta_y)){
        delta_y = (delta_y + WORLD_HEIGHT)% WORLD_HEIGHT;
      }
      // opposite case as above
      if ((abs(delta_x - WORLD_WIDTH)) < abs(delta_x)){
        delta_x -= WORLD_WIDTH;
      }
      if ((abs(delta_y - WORLD_WIDTH)) < abs(delta_y)){
        delta_y -= WORLD_HEIGHT;
      }

      // std::cout<<"Angle To neighbor 0-TL(degrees)"<<atan2(1,-1) * 180/M_PI<<std::endl;
      // std::cout<<"Angle To neighbor 1-T(degrees)"<<atan2(1,0) * 180/M_PI<<std::endl;
      // std::cout<<"Angle To neighbor 2-TR(degrees)"<<atan2(1,1) * 180/M_PI<<std::endl;
      // std::cout<<"Angle To neighbor 3-L(degrees)"<<atan2(0,-1) * 180/M_PI<<std::endl;
      // std::cout<<"Angle To neighbor 4-R(degrees)"<<atan2(0,1) * 180/M_PI<<std::endl;
      // std::cout<<"Angle To neighbor 5-BL(degrees)"<<atan2(-1,-1) * 180/M_PI<<std::endl;
      // std::cout<<"Angle To neighbor 6-B(degrees)"<<atan2(-1,0) * 180/M_PI<<std::endl;
      // std::cout<<"Angle To neighbor 7-BR(degrees)"<<atan2(-1,1) * 180/M_PI<<std::endl;

      double angle_radian = atan2(delta_y, delta_x);
      while (angle_radian<0){angle_radian+=2*M_PI;}

      return angle_radian *  180/M_PI;
    }

};


#endif
