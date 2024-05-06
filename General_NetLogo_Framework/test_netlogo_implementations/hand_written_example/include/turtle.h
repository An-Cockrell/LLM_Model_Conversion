// turtle.h
#ifndef TURTLE_H
#define TURTLE_H

#include "agent.h"
#include <random>

/**
 * Class representing a turtle in the simulation.
 * Inherits from the Agent class and extends it with turtle-specific attributes and behaviors,
 * such as ID, heading, age, and movement mechanics.
 */
class Turtle : public Agent{
      /**
     * Constructor for the Turtle class.
     * Initializes a turtle with specific coordinates, ID, and optionally heading.
     * The decimal coordinates are also initialized to the given integer coordinates.
     *
     * @param x The x-coordinate of the turtle's position.
     * @param y The y-coordinate of the turtle's position.
     * @param ID_num The unique identifier for the turtle.
     * @param heading The initial heading (direction) of the turtle. Defaults to 0.
     */
  public :
    Turtle(int x, int y, int ID_num, int heading = 0) : Agent(x, y), ID_num(ID_num), heading(heading) {
        x_dec = x;
        y_dec = y;
    };

    virtual ~Turtle() = default; // Default destructor.

  private:
    int ID_num; // Unique identifier for the turtle.
    int heading; // Current direction the turtle is facing.
    int age; // Age of the turtle.
    float x_dec; // X-coordinate in decimal, for precise movement.
    float y_dec; // Y-coordinate in decimal, for precise movement.
    float temp_x; // Temporary x-coordinate, used during movement calculations.
    float temp_y; // Temporary y-coordinate, used during movement calculations.
    
    // More variables specific to turtles can be added here.



  public:
    /**
     * Moves the turtle a specified distance in its current heading.
     *
     * @param distance The distance to move. Defaults to 1.
     * @return A pair of integers representing the new coordinates after moving.
     */
    std::pair<int, int> move(float distance = 1);

    /**
     * Moves the turtle to a random position based on a random heading and distance.
     *
     * @param RNG_Engine A reference to the random number generator engine.
     * @return A pair of integers representing the new coordinates after jumping.
     */
    std::pair<int, int> jumpRandom(std::mt19937 &RNG_Engine);

    /**
     * Executes the movement of the turtle, updating its position if the move was successful.
     *
     * @param didMove A boolean indicating whether the move was successful.
     */
    void execute_move(bool didMove);

    /**
     * Randomly adjusts the turtle's heading to simulate a wiggle movement.
     *
     * @param RNG_Engine A reference to the random number generator engine.
     */
    void wiggle(std::mt19937 &RNG_Engine);
    
// Displays the turtle's current state to the standard output.
    void display();

    // Getter for the turtle's ID.
    int getID() const;
    
    // Sets the turtle's heading.
    void setHeading(int newHeading){heading = newHeading;};
    
    // Getter for the turtle's heading.
    int getHeading() const {return heading;};
    
    // Sets the turtle's age.
    void setAge(int newAge){age = newAge;};
    
    // Getter for the turtle's age.
    int getAge() const{return age;};
    
    // Getters for the turtle's precise (decimal) x and y coordinates.
    float get_x_dec() const;
    float get_y_dec() const;



    // Comparison operators to check if two turtles are the same based on their ID.
    bool operator== (const Turtle& turt2){return this->ID_num==turt2.ID_num;};
    bool operator== (const Turtle* turt2){return this->ID_num==turt2->ID_num;};

    
    // More turtle-specific functions can be added here.

};


#endif
