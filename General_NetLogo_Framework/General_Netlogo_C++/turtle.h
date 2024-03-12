// turtle.h
#ifndef TURTLE_H
#define TURTLE_H

#include "agent.h"
#include <random>

class Turtle : public Agent{
  public :
    Turtle(int x, int y, int ID_num, int heading=0) : Agent(x, y), ID_num(ID_num), heading(heading) {x_dec=x; y_dec=y;};
    virtual ~Turtle() = default;

  private:
    int ID_num;
    int heading;
    int age;
    float x_dec; //x and y decimal coordinates
    float y_dec;
    float temp_x; // temporary coords for turtle move, before actual turtle coords are moved in case destination patch is full.
    float temp_y; // Turtle cant know about the world cause circular dependency in #include, so world will handle turtle movement between patches
    

    // add more variables that apply to all turtles here for the specific netlogo model being ported

  public:
    std::pair<int,int> move(float distance=1);
    std::pair<int,int> jumpRandom(std::mt19937 &RNG_Engine);
    void execute_move(bool didMove);
    void wiggle(std::mt19937 &RNG_Engine);
    void display();

    int getID() const {return ID_num;};
    
    void setHeading(int newHeading){heading = newHeading;};
    int getHeading() const {return heading;};
    
    void setAge(int newAge){age=newAge;};
    void getAge() const { return age;};
    
    float get_x_dec() const {return x_dec;}       // already has regular getX() and getY() from agent parent class
    float get_y_dec() const {return y_dec;}


    bool operator== (const Turtle& turt2){return this->ID_num==turt2.ID_num;};
    bool operator== (const Turtle* turt2){return this->ID_num==turt2->ID_num;};
    
    // add more turtle specific functions below. These are functions that will apply to all turtle types, not a single breed

};


#endif
