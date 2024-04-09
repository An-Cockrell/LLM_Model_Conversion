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
