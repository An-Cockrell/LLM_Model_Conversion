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

    double oxy;                     // oxygen
    int ec_activation;             // ec activation
    int ec_roll;                   // ec roll
    int ec_stick;                 // ec stick
    int ec_migrate;               // ec migrate
    double cytotox;               // cytotox
    double endotoxin;
    double PAF;
    double TNF;
    double sTNFr;
    double IL_1;
    double sIL_1r;
    double IL_1ra;
    double INF_g;
    double IL_2;
    double IL_4;
    double IL_6;
    double IL_8;
    double IL_10;
    double IL_12;
    double GCSF;

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

    double getOxy() const;
    void setOxy(double newOxy);

    int getECActivation() const;
    void setECActivation(int newECActivation);

    int getECRoll() const;
    void setECRoll(int newECRoll);

    int getECStick() const;
    void setECStick(int newECStick);

    int getECMigrate() const;
    void setECMigrate(int newECMigrate);

    double getCytotox() const;
    void setCytotox(double newCytotox);

    double getEndotoxin() const;
    void setEndotoxin(double newEndotoxin);

    double getPAF() const;
    void setPAF(double newPAF);

    double getTNF() const;
    void setTNF(double newTNF);

    double getSTNFr() const;
    void setSTNFr(double newSTNFr);

    double getIL_1() const;
    void setIL_1(double newIL_1);

    double getSIl_1r() const;
    void setSIl_1r(double newSIl_1r);

    double getIL_1ra() const;
    void setIL_1ra(double newIL_1ra);

    double getINF_g() const;
    void setINF_g(double newINF_g);

    double getIL_2() const;
    void setIL_2(double newIL_2);

    double getIL_4() const;
    void setIL_4(double newIL_4);

    double getIL_6() const;
    void setIL_6(double newIL_6);

    double getIL_8() const;
    void setIL_8(double newIL_8);

    double getIL_10() const;
    void setIL_10(double newIL_10);

    double getIL_12() const;
    void setIL_12(double newIL_12);

    double getGCSF() const;
    void setGCSF(double newGCSF);
};

#endif // PATCH_H


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
    double wbc_roll;            // Selectins
    double wbc_stick;           // Integrins
    double wbc_migrate;         // Diapedesis
    int pmn_age;               // PMN life span
    int pmn_pcd;               // PMN programmed cell death
    int mono_age;              // Monocyte life span
    double tnfr;               // TNF receptors
    double il_1r;              // IL-1 receptors
    int th0_age;               // TH0 cell age
    int th1_age;               // TH1 cell age
    int th2_age;               // TH2 cell age
    bool activation;           // Activation status
    double pro_th1;            // Level of pro-TH1 cytokines
    double pro_th2;            // Level of pro-TH2 cytokines
    double r_th1;              // Random holder for pro-TH1
    double r_th2;              // Random holder for pro-TH2


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
    // Getters and Setters for the new member variables.
    double getWbcRoll() const { return wbc_roll; }
    void setWbcRoll(double value) { wbc_roll = value; }

    double getWbcStick() const { return wbc_stick; }
    void setWbcStick(double value) { wbc_stick = value; }

    double getWbcMigrate() const { return wbc_migrate; }
    void setWbcMigrate(double value) { wbc_migrate = value; }

    int getPmnAge() const { return pmn_age; }
    void setPmnAge(int value) { pmn_age = value; }

    int getPmnPcd() const { return pmn_pcd; }
    void setPmnPcd(int value) { pmn_pcd = value; }

    int getMonoAge() const { return mono_age; }
    void setMonoAge(int value) { mono_age = value; }

    double getTnfr() const { return tnfr; }
    void setTnfr(double value) { tnfr = value; }

    double getIl1r() const { return il_1r; }
    void setIl1r(double value) { il_1r = value; }

    int getTh0Age() const { return th0_age; }
    void setTh0Age(int value) { th0_age = value; }

    int getTh1Age() const { return th1_age; }
    void setTh1Age(int value) { th1_age = value; }

    int getTh2Age() const { return th2_age; }
    void setTh2Age(int value) { th2_age = value; }

    bool getActivation() const { return activation; }
    void setActivation(bool value) { activation = value; }

    double getProTh1() const { return pro_th1; }
    void setProTh1(double value) { pro_th1 = value; }

    double getProTh2() const { return pro_th2; }
    void setProTh2(double value) { pro_th2 = value; }


};


#endif


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
  double system_oxy_;            // System's oxygen level
  double oxy_deficit_;           // Oxygendeficit
  int total_infection_;         // Total infection in the model
  double total_TNF_;             // Total concentration of TNF (Tumor Necrosis Factor)
  double total_sTNFr_;           // Total concentration of sTNFr (soluble Tumor Necrosis Factor receptor)
  double total_IL_10_;           // Total concentration of IL-10 (Interleukin 10)
  double total_IL_6_;             // Total concentration of IL-6 (Interleukin 6)
  double total_GCSF_;             // Total concentration of GCSF (Granulocyte Colony-stimulating Factor)
  int step_;                    // Current simulation step
  int time_;                    // Simulation time
  int loop_run_;                // Controls the loop runs
  double total_pro_TH1_;         // Total concentration of pro-TH1 cells
  double total_pro_TH2_;         // Total concentration of pro-TH2 cells

  std::vector<std::shared_ptr<Turtle>> all_turtles; // Vector storing all turtles in the world as shared pointers.
  std::vector<std::shared_ptr<Turtle>> all_injs;     // Vector for injury turtles
  std::vector<std::shared_ptr<Turtle>> all_pmns;     // Vector for PMN turtles
  std::vector<std::shared_ptr<Turtle>> all_bands;    // Vector for band turtles
  std::vector<std::shared_ptr<Turtle>> all_pmn_marrows; // Vector for PMN marrow turtles
  std::vector<std::shared_ptr<Turtle>> all_monos;     // Vector for monocyte turtles
  std::vector<std::shared_ptr<Turtle>> all_mono_marrows; // Vector for monocyte marrow turtles
  std::vector<std::shared_ptr<Turtle>> all_gen_mono_marrows; // Vector for generic monocyte marrow turtles
  std::vector<std::shared_ptr<Turtle>> all_th0_germs;     // Vector for TH0 germ turtles
  std::vector<std::shared_ptr<Turtle>> all_th0s;         // Vector for TH0 cells
  std::vector<std::shared_ptr<Turtle>> all_th1s;         // Vector for TH1 cells
  std::vector<std::shared_ptr<Turtle>> all_th1_germs;     // Vector for TH1 germ turtles
  std::vector<std::shared_ptr<Turtle>> all_th2s;         // Vector for TH2 cells
  std::vector<std::shared_ptr<Turtle>> all_th2_germs;     // Vector for TH2 germ turtles
  std::vector<std::shared_ptr<Turtle>> all_nks;          // Vector for NK cells
  std::vector<std::shared_ptr<Turtle>> all_nks_cells;     // Vector for NK cell turtles
  
  Patch all_patches[WORLD_WIDTH][WORLD_HEIGHT]; // Array of patches constituting the world.
  
  // Sets the seed for the random number generator.
  void set_rng_seed(int newSeed) { RNG_Engine.seed(newSeed); }
  
  // Adds a patch to the world at the specified coordinates.
  void add_patch(int x, int y);


public:
  std::vector<std::weak_ptr<Turtle>> all_turtles; // Vector storing all turtles in the world as weak pointers.


  // Getters:
  double getSystemOxygenLevel() const {return system_oxy_;}
  double getOxygenDeficit() const {return oxy_deficit_;}
  int getTotalInfection() const {return total_infection_;}
  double getTotalTFN() const {return total_TNF_;}
  double getTotalsTNFr() const {return total_sTNFr_;}
  double getTotalIL10() const {return total_IL_10_;}
  double getTotalIL6() const {return total_IL_6_;}
  double getTotalGCSF() const {return total_GCSF_;}
  int getSimulationStep() const {return step_;}
  int getSimulationTime() const {return time_;}
  int getLoopRun() const {return loop_run_;}
  double getTotalProTH1() const {return total_pro_TH1_;}
  double getTotalProTH2() const {return total_pro_TH2_;}

  // Setters:
  void setSystemOxygenLevel(double system_oxy) {system_oxy_ = system_oxy;}
  void setOxygenDeficit(double oxy_deficit) {oxy_deficit_ = oxy_deficit;}
  void setTotalInfection(int total_infection) {total_infection_ = total_infection;}
  void setTotalTFN(double total_TNF) {total_TNF_ = total_TNF;}
  void setTotalSTNFr(double total_sTNFr) {total_sTNFr_ = total_sTNFr;}
  void setTotalIL10(double total_IL_10) {total_IL_10_ = total_IL_10;}
  void setTotalIL6(double total_IL_6) {total_IL_6_ = total_IL_6;}
  void setTotalGCSF(double total_GCSF) {total_GCSF_ = total_GCSF;}
  void setSimulationStep(int step) {step_ = step;}
  void setSimulationTime(int time) {time_ = time;}
  void setLoopRun(int loop_run) {loop_run_ = loop_run;}
  void setTotalProTH1(double total_pro_TH1) {total_pro_TH1_ = total_pro_TH1;}
  void setTotalProTH2(double total_pro_TH2) {total_pro_TH2_ = total_pro_TH2;}
  // include vectors of shared_ptr's of other turtle sub-types here
  // for example:   std::vector<std::shared_ptr<Pmn>> all_pmns;

  
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

  
};


#endif