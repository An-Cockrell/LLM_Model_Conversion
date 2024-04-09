#include <iostream>
#include <string>
#include <vector>
#include "patch.h"
#include <algorithm>
#include <fstream>

/**
 * Adds a turtle to the list of turtles present on this patch.
 * @param turtle_to_add The shared pointer to the turtle to be added.
 */
void Patch::add_turtle(std::shared_ptr<Turtle> turtle_to_add) {
    turtles_here.emplace_back(turtle_to_add);
}

/**
 * Removes a specified turtle from the list of turtles present on this patch.
 * This method finds the turtle in the vector and removes it.
 * @param turtle_to_remove The shared pointer to the turtle to be removed.
 */
void Patch::remove_turtle(std::shared_ptr<Turtle> turtle_to_remove) {
    // Uses the erase-remove idiom to efficiently remove the turtle from the vector.
    turtles_here.erase(std::remove(turtles_here.begin(), turtles_here.end(), turtle_to_remove), turtles_here.end());
}

/**
 * Displays information about this patch, including its position and any turtles present.
 * This method iterates over all turtles on the patch and calls their display method.
 */
void Patch::display() {
    std::cout << "Patch position: (" << this->getX() << ", " << getY() << ")" << std::endl;
    // Displaying the patch color is optional and can be uncommented if needed.
    // std::cout << "Patch color: " << getColor() << std::endl;
    if (!turtles_here.empty()) { // Checks if there are turtles on the patch.
        for (auto &turtle : turtles_here) {
            turtle->display(); // Calls the display method for each turtle.
        }
    }
}

/**
 * Checks if the number of turtles on this patch has reached the maximum capacity.
 * @return True if at maximum capacity, otherwise false.
 */
bool Patch::is_at_capacity() {
    return (turtles_here.size() >= PATCH_MAX_CAPACITY);
}

// Additional patch-specific functionalities can be implemented here, following the simulation's requirements.
