#ifndef TFH_CELL_H
#define TFH_CELL_H

#include "turtle.h"

class TfhCell : public Turtle {
private:
    // Variables specific to TfhCell
    bool bcell_binding_status; // Assuming it's a boolean status, adjust type as needed

public:
    // You can add specific methods and properties for this subclass
    TfhCell(int x, int y, int id, int heading=90);

    // Override Turtle methods if needed

    // Getter for bcell_binding_status
    bool getBcellBindingStatus() const { return bcell_binding_status; }

    // Setter for bcell_binding_status
    void setBcellBindingStatus(bool status) { this->bcell_binding_status = status; }
};

#endif
