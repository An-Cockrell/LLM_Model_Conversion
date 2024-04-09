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
