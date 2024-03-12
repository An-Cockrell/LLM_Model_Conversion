// agent.h
#ifndef AGENT_H
#define AGENT_H


#include <iostream>
#include <fstream>
#include <vector>

class Agent {
  protected:
    Agent(int x, int y) : x(x), y(y) {}
  private:
    std::string color = "";
    int alpha = 255;
    int x;
    int y;
  public:

    virtual double getX() const {return x;};
    virtual double getY() const {return y;};
    virtual void setX(int newX){x=newX;};
    virtual void setY(int newY){y=newY;};
    void setColor(std::string newColor){color = newColor;};
    std::string getColor() const {return color;};
    void setOpacity(int newAlpha){alpha = newAlpha;};
    int getOpacity() const {return alpha;};
};


#endif