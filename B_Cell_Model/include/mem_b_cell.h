#ifndef MEM_B_CELL_H
#define MEM_B_CELL_H

#include "turtle.h"

class MemBCell : public Turtle {
private:
    // Time in follicle for memory B cells
    int time_in_follicle;

public:
    // You can add specific methods and properties for this subclass
    MemBCell(int x, int y, int id, int heading=90);

    // Override Turtle methods if needed

    // Getter for time in follicle
    int getTimeInFollicle() const { return time_in_follicle; }

    // Setter for time in follicle
    void setTimeInFollicle(int time_in_follicle) { this->time_in_follicle = time_in_follicle; }
};

#endif
