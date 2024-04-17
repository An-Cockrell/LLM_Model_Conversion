#ifndef AGENT_H
#define AGENT_H

#include <iostream>
#include <fstream>
#include <vector>

/**
 * Abstract class representing a generic agent in the simulation.
 * An agent is characterized by its position (x, y), color, and opacity.
 * This class provides the basic functionalities for managing the agent's
 * state and behavior in the simulation world.
 */
class Agent {
protected:
    // Protected constructor to prevent instantiation of this abstract class
    // Initializes an agent at a specific position (x, y).
    Agent(int x, int y) : x(x), y(y) {}

private:
    std::string color = ""; // The agent's color, represented as a string.
    int alpha = 255; // The agent's opacity level, 255 is fully opaque.
    int x; // The x-coordinate of the agent's position in the simulation.
    int y; // The y-coordinate of the agent's position in the simulation.

public:
    // Gets the x-coordinate of the agent's position.
    virtual double getX() const { return x; };
    
    // Gets the y-coordinate of the agent's position.
    virtual double getY() const { return y; };
    
    // Sets the x-coordinate of the agent's position.
    virtual void setX(int newX) { x = newX; };
    
    // Sets the y-coordinate of the agent's position.
    virtual void setY(int newY) { y = newY; };
    
    // Sets the agent's color.
    void setColor(std::string newColor) { color = newColor; };
    
    // Gets the agent's color.
    std::string getColor() const { return color; };
    
    // Sets the agent's opacity.
    void setOpacity(int newAlpha) { alpha = newAlpha; };
    
    // Gets the agent's opacity level.
    int getOpacity() const { return alpha; };
};

#endif // AGENT_H


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
class Turtle final : public Agent{
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
    void wiggle(std::mt19937 &RNG_Engine);    void display();

// Displays the turtle's current state to the standard output.
    void display();

    // Getter for the turtle's ID.
    int getID() const;
    
    // Sets the turtle's heading.
    void setHeading(int newHeading);
    
    // Getter for the turtle's heading.
    int getHeading() const;
    
    // Sets the turtle's age.
    void setAge(int newAge);
    
    // Getter for the turtle's age.
    void getAge() const;
    
    // Getters for the turtle's precise (decimal) x and y coordinates.
    float get_x_dec() const;
    float get_y_dec() const;



    // Comparison operators to check if two turtles are the same based on their ID.
    bool operator== (const Turtle& turt2){return this->ID_num==turt2.ID_num;};
    bool operator== (const Turtle* turt2){return this->ID_num==turt2->ID_num;};

    
    // More turtle-specific functions can be added here.

};


#endif


#ifndef PATCH_H
#define PATCH_H

#include "parameters.h"
#include "agent.h"
#include "turtle.h"
#include <memory>

/**
 * Class representing a patch in the simulation world.
 * Inherits from Agent, providing functionalities specific to patches,
 * such as managing turtles present on the patch and handling diffusion processes.
 */
class Patch : public Agent {
private:
    std::vector<std::shared_ptr<Turtle>> turtles_here; // Stores turtles currently on this patch.
    
    double temp_var; // Placeholder for storing diffusion totals before they are updated.

public:
    /**
     * Default constructor for the Patch class.
     * Initializes the patch at coordinates (0, 0) with a default red color.
     */
    Patch() : Agent(0, 0) { setColor("red"); };

    /**
     * Constructor for the Patch class that specifies coordinates.
     * Initializes the patch at given coordinates with a default red color.
     *
     * @param xCoord The x-coordinate of the patch.
     * @param yCoord The y-coordinate of the patch.
     */
    Patch(int xCoord, int yCoord) : Agent(xCoord, yCoord) { setColor("red"); };
        
    /**
     * Adds a turtle to this patch.
     *
     * @param turtle_to_add A shared pointer to the Turtle to be added to the patch.
     */
    void add_turtle(std::shared_ptr<Turtle> turtle_to_add);

    /**
     * Removes a turtle from this patch.
     *
     * @param turtle_to_remove A shared pointer to the Turtle to be removed from the patch.
     */
    void remove_turtle(std::shared_ptr<Turtle> turtle_to_remove);

    /**
     * Displays information about the patch, including its position and the turtles present on it.
     */
    void display();

    /**
     * Checks whether the patch has reached its maximum capacity of turtles.
     *
     * @return A boolean value indicating if the patch is at capacity.
     */
    bool is_at_capacity();

    /**
     * Sets the temporary variable used for diffusion calculations.
     *
     * @param newVal The new value to be set for the temporary variable.
     */
    void setTempVar(double newVal) { temp_var = newVal; };

    /**
     * Retrieves the current value of the temporary variable used in diffusion calculations.
     *
     * @return The current value of the temporary variable.
     */
    double getTempVar() const { return temp_var; };
    
    /**
     * Comparison operator to check if two patches are at the same location.
     *
     * @param that A reference to another Patch object.
     * @return A boolean indicating if the two patches have the same coordinates.
     */
    bool operator== (Patch& that) { return (this->getX() == that.getX() && this->getY() == that.getY()); };
    
    // Additional patch-specific functions can be added here.
};

#endif // PATCH_H



// world.h
#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <memory>
#include <algorithm>

#include "parameters.h"
#include "agent.h"
#include "patch.h"
#include "turtle.h"


/**
 * Class representing the world in which the simulation takes place.
 * This class manages the entire simulation environment, including patches, turtles, and the simulation loop.
 */
class World {
private:
  std::mt19937 RNG_Engine; // Random number generator for the simulation.
  int global_ID_counter;  // Counter to assign unique IDs to new turtles.
  int step; // The current simulation step.
  int time; // The simulation time.
  int loop_run; // The loop run indicator, for controlling simulation loops.

  float system_oxy; // System's oxygen level.
  float oxy_deficit; // The oxygen deficit.
  float total_infection; // Total infection in the model.
  float total_TNF; // Total concentration of TNF (Tumor Necrosis Factor).
  float total_sTNFr; // Total concentration of sTNFr (soluble Tumor Necrosis Factor receptor).
  float total_IL_10; // Total concentration of IL-10 (Interleukin 10).
  float total_IL_6; // Total concentration of IL-6 (Interleukin 6).
  float total_GCSF; // Total concentration of GCSF (Granulocyte Colony-stimulating Factor).
  float total_pro_TH1; // Total concentration of pro-TH1 cells.
  float total_pro_TH2; // Total concentration of pro-TH2 cells.
  
  
  Patch all_patches[WORLD_WIDTH][WORLD_HEIGHT]; // Array of patches constituting the world.
  
  // Sets the seed for the random number generator.
  void set_rng_seed(int newSeed) { RNG_Engine.seed(newSeed); }
  
  // Adds a patch to the world at the specified coordinates.
  void add_patch(int x, int y);


public:
  std::vector<std::weak_ptr<Turtle>> all_turtles; // Vector storing all turtles in the world as weak pointers.

  // include vectors of shared_ptr's of other turtle sub-types here

  
  World(){}; // Default constructor.
  
  // Retrieves the patch at the specified coordinates.
  Patch& get_patch(int x, int y);
  // Retrieves the patch located ahead of a given turtle.
  Patch& getPatchAhead(std::shared_ptr<Turtle> turtle, float distance=1);
  // Retrieves the patch located ahead and to the right of a given turtle.
  Patch& getPatchAheadRight(std::shared_ptr<Turtle> turtle, float distance=1);
  // Retrieves the patch located ahead and to the left of a given turtle.
  Patch& getPatchAheadLeft(std::shared_ptr<Turtle> turtle, float distance=1);


  // Initializes patches in the world.
  void setup_patches();
  // Displays all patches in the world.
  void display_patches();
  
  // Adds a turtle to the world at the specified coordinates.
  void add_turtle(int x, int y, int id, int heading=0);
  // Moves a turtle according to its current heading.
  void move_turtle(std::shared_ptr<Turtle> turtle);
  // Moves a turtle to a random location.
  void move_turtle_random_jump(std::shared_ptr<Turtle> turtle);
  // Causes a turtle to wiggle in place.
  void turtle_wiggle(std::shared_ptr<Turtle> turtle);
  
  // Removes a turtle from the world.
  void kill_turtle(std::shared_ptr<Turtle> turtle);

  
  // templated function to kill any turtle or agent
  template <typename T>
  void kill(std::shared_ptr<T> &ptr){
    kill_turtle(ptr);
    ptr.reset();
  }

  // Updates vectors storing turtles to remove any that have been deleted.
  void updateTurtleVectors();

  // Sets up the initial state of the world.
  void setup();
  // Executes one simulation step.
  void go();
    
  // Diffuses variables across patches.
  void diffuse();
  // Evaporates variables from patches.
  void evaporate();

  // declare more turtle specific functions below. These are functions that will apply to all turtle types, not a single breed
  // Getters and Setters for the system_oxy variable
  float get_system_oxy() const { return system_oxy; }
  void set_system_oxy(float value) { system_oxy = value; }

  // Similar getters and setters for all other variables.

  float get_oxy_deficit() const { return oxy_deficit; }
  void set_oxy_deficit(float value) { oxy_deficit = value; }

  float get_total_infection() const { return total_infection; }
  void set_total_infection(float value) { total_infection = value; }

  float get_total_TNF() const { return total_TNF; }
  void set_total_TNF(float value) { total_TNF = value; }

  float get_total_sTNFr() const { return total_sTNFr; }
  void set_total_sTNFr(float value) { total_sTNFr = value; }

  float get_total_IL_10() const { return total_IL_10; }
  void set_total_IL_10(float value) { total_IL_10 = value; }

  float get_total_IL_6() const { return total_IL_6; }
  void set_total_IL_6(float value) { total_IL_6 = value; }

  float get_total_GCSF() const { return total_GCSF; }
  void set_total_GCSF(float value) { total_GCSF = value; }

  float get_total_pro_TH1() const { return total_pro_TH1; }
  void set_total_pro_TH1(float value) { total_pro_TH1 = value; }

  float get_total_pro_TH2() const { return total_pro_TH2; }
  void set_total_pro_TH2(float value) { total_pro_TH2 = value; }
  
};


#endif