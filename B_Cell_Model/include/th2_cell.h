#ifndef TH2_CELL_H
#define TH2_CELL_H

#include "turtle.h"

class Th2Cell : public Turtle {
private:
    // Variables specific to Th2Cell
    bool bcell_binding_status; // Assuming it's a boolean status, adjust type as needed

public:
    // You can add specific methods and properties for this subclass
    Th2Cell(int x, int y, int id, int heading=0);

    // Override Turtle methods if needed

    // Getter for bcell_binding_status
    bool getBcellBindingStatus() const { return bcell_binding_status; }

    // Setter for bcell_binding_status
    void setBcellBindingStatus(bool status) { this->bcell_binding_status = status; }
};

#endif
