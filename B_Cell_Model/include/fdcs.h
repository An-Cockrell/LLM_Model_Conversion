#ifndef FDCS_H
#define FDCS_H

#include "turtle.h"
#include <string>

class FDCs : public Turtle {
private:
    // Presented antigen
    int presented_antigen;
    // Time presenting
    int time_presenting;
    // Type of presented antigen
    int presented_antigen_type;
    // Responsiveness
    int responsiveness;

public:
    // You can add specific methods and properties for this subclass
    FDCs(int x, int y, int id, int heading=90);

    // Override Turtle methods if needed

    // Getter for presented antigen
    int getPresentedAntigen() const { return presented_antigen; }

    // Setter for presented antigen
    void setPresentedAntigen(int new_presented_antigen) { this->presented_antigen = new_presented_antigen; }

    // Getter for time presenting
    int getTimePresenting() const { return time_presenting; }

    // Setter for time presenting
    void setTimePresenting(int time_presenting) { this->time_presenting = time_presenting; }

    // Getter for presented antigen type
    int getPresentedAntigenType() const { return presented_antigen_type; }

    // Setter for presented antigen type
    void setPresentedAntigenType(int presented_antigen_type) { this->presented_antigen_type = presented_antigen_type; }

    // Getter for responsiveness
    int getResponsiveness() const { return responsiveness; }

    // Setter for responsiveness
    void setResponsiveness(int responsiveness) { this->responsiveness = responsiveness; }

    void calculate_responsiveness();
};

#endif
