#ifndef BACTERIA_H
#define BACTERIA_H

#include "turtle.h"
#include <string>

class Bacteria : public Turtle {
private:
    // Type of the epitope
    int epitope_type;
    // Number of TI-ep
    int num_TI_ep;
    // Number of TD-ep
    int num_TD_ep;

public:
    // You can add specific methods and properties for this subclass
    Bacteria(int x, int y, int id, int heading=90);

    // Override Turtle methods if needed

    // Getter for epitope type
    int getEpitopeType() const { return epitope_type; }

    // Setter for epitope type
    void setEpitopeType(const int epitope_type) { this->epitope_type = epitope_type; }

    // Getter for num_TI_ep
    int getNumTIep() const { return num_TI_ep; }

    // Setter for num_TI_ep
    void setNumTIep(int num_TI_ep) { this->num_TI_ep = num_TI_ep; }

    // Getter for num_TD_ep
    int getNumTDep() const { return num_TD_ep; }

    // Setter for num_TD_ep
    void setNumTDep(int num_TD_ep) { this->num_TD_ep = num_TD_ep; }
};

#endif
