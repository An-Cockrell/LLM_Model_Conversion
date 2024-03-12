// patch.h
#ifndef PATCH_H
#define PATCH_H

#include "parameters.h"
#include "agent.h"
#include "turtle.h"
#include <memory>

// class Turtle;

class Patch : public Agent{
  private:
    std::vector<std::shared_ptr<Turtle>> turtles_here;
    
    double temp_var; //placeholder for diffusion totals before updating

  public:
    Patch():Agent(0,0){setColor("red");};               // base patch color is red
    Patch(int xCoord, int yCoord) : Agent(xCoord, yCoord) {setColor("red");};
        
    void add_turtle(std::shared_ptr<Turtle> turtle_to_add);
    void remove_turtle(std::shared_ptr<Turtle> turtle_to_remove);
    void display();
    bool is_at_capacity();

    void setTempVar(double newVal){temp_var=newVal};
    double getTempVar() const {return temp_var};
    
    // patch is the same iff the x and y positions are equal
    bool operator== (Patch& that){return (this->getX()==that.getX() && this->getY()==that.getY());};
    
    
    // Add more patch specific functions below for the particular implementation of a net logo model being ported
    
    
};



#endif