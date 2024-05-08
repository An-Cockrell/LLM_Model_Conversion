// turtle.cpp
#include <iostream>
#include <string>
#include "agent.h"
#include <cmath>
#include "turtle.h"
#include "parameters.h"

/**
 * Moves the turtle based on its current heading and a specified distance.
 * Calculates the new position considering the world's dimensions to allow wrapping around edges.
 * 
 * @param distance The distance to move in the direction of the turtle's heading. Default is 1.
 * @return A pair of integers representing the new (x, y) position of the turtle.
 */
std::pair<int,int> Turtle::move(float distance) {
    // Save current decimal coordinates in case the move is not executed.
    temp_x = x_dec;
    temp_y = y_dec;
    // Calculate movement vector based on heading and distance.
    float d_x = cos(heading * M_PI / 180) * distance;
    float d_y = sin(heading * M_PI / 180) * distance;
    // Adjust for discrete movement if necessary, ensuring turtles land on patches.
    if (!TURTLE_CONTINOUS_MOVEMENT) {
        d_x = round(d_x);
        d_y = round(d_y);
    }
    // Update decimal coordinates.
    x_dec += d_x;
    y_dec += d_y;
    // Wrap coordinates around world edges.
    x_dec = fmod(fmod(x_dec, WORLD_WIDTH) + WORLD_WIDTH, WORLD_WIDTH);
    y_dec = fmod(fmod(y_dec, WORLD_HEIGHT) + WORLD_HEIGHT, WORLD_HEIGHT);
    // Convert decimal coordinates to integer grid coordinates.
    int newx = trunc(x_dec);
    int newy = trunc(y_dec);

    return {newx, newy};
}

/**
 * Moves the turtle to a random position by generating a random heading and distance.
 * 
 * @param RNG_Engine A reference to a random number generator engine.
 * @return A pair of integers representing the new (x, y) position of the turtle.
 */
std::pair<int,int> Turtle::jumpRandom(std::mt19937 &RNG_Engine) {
    int random_heading = RNG_Engine() % 360; // Generate a random heading.
    int random_distance = RNG_Engine() % MAX_RANDOM_DISTANCE; // Generate a random distance.
    
    this->setHeading(random_heading); // Update turtle's heading.
    return this->move(random_distance); // Move turtle and return new position.
}


/**
 * Executes the movement calculated by move() or jumpRandom(), updating the turtle's position.
 * 
 * @param didMove Boolean flag indicating whether the move should be executed.
 */
void Turtle::execute_move(bool didMove) {
    if (didMove) {
        // Update the turtle's grid position to the new coordinates.
        setX(trunc(x_dec));
        setY(trunc(y_dec));
    } else {
        // Revert to the original coordinates if the move is not executed.
        x_dec = temp_x;
        y_dec = temp_y;
    }
}

/**
 * Modifies the turtle's heading by a random amount to simulate a wiggle motion.
 * 
 * @param RNG_Engine A reference to a random number generator engine.
 */
void Turtle::wiggle(std::mt19937 &RNG_Engine) {
    int random_left = RNG_Engine() % 45; // Random angle to turn left.
    int random_right = RNG_Engine() % 45; // Random angle to turn right.
    // Adjust heading by the difference between left and right random values.
    this->setHeading(this->getHeading() + random_left - random_right);
}

/**
 * Prints the current state of the turtle, including position, exact position (decimal coordinates),
 * heading, and ID, to the standard output.
 */
void Turtle::display() {
    std::cout << "Turtle position: (" << this->getX() << ", " << this->getY() << ")" << std::endl;
    std::cout << "Turtle exact position: (" << x_dec << ", " << y_dec << ")" << std::endl;
    std::cout << "Turtle Heading: (" << heading << ")" << std::endl;
    std::cout << "Turtle ID: (" << ID_num << ")" << std::endl;
}

// Additional turtle-specific functions can be implemented below.
