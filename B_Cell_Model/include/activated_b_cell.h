#ifndef ACTIVATED_B_CELL_H
#define ACTIVATED_B_CELL_H

#include "turtle.h"

class ActivatedBCell : public Turtle {
private:
    // Response type for activated B cells
    int response_type;

public:
    // You can add specific methods and properties for this subclass
    ActivatedBCell(int x, int y, int id, int heading);

    // Override Turtle methods if needed

    // Getter for response type
    int getResponseType() const { return response_type; }

    // Setter for response type
    void setResponseType(int response_type) { this->response_type = response_type; }

};

#endif
