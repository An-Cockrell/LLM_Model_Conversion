#ifndef ANTIBODIES_H
#define ANTIBODIES_H

#include "turtle.h"
#include <string>

class Antibodies : public Turtle {
private:
    // Type of the antibody
    std::string antibody_type;

public:
    // You can add specific methods and properties for this subclass
    Antibodies(int x, int y, int id, int heading=90);

    // Override Turtle methods if needed

    // Getter for antibody type
    std::string getAntibodyType() const { return antibody_type; }

    // Setter for antibody type
    void setAntibodyType(const std::string& antibody_type) { this->antibody_type = antibody_type; }
};

#endif
