#ifndef TH0_CELL_H
#define TH0_CELL_H

#include "turtle.h"

class Th0Cell : public Turtle {
private:
    // Variables specific to Th0Cell
    float r1;
    float r2;
    float rf;
    float th1_activation;
    float th2_activation;
    float tfh_activation;

public:
    // You can add specific methods and properties for this subclass
    Th0Cell(int x, int y, int id, int heading=90);

    // Override Turtle methods if needed

    // Getter for r1
    float getR1() const { return r1; }

    // Setter for r1
    void setR1(float r1) { this->r1 = r1; }

    // Getter for r2
    float getR2() const { return r2; }

    // Setter for r2
    void setR2(float r2) { this->r2 = r2; }

    // Getter for rf
    float getRf() const { return rf; }

    // Setter for rf
    void setRf(float rf) { this->rf = rf; }

    // Getter for th1_activation
    float getTh1Activation() const { return th1_activation; }

    // Setter for th1_activation
    void setTh1Activation(float th1_activation) { this->th1_activation = th1_activation; }

    // Getter for th2_activation
    float getTh2Activation() const { return th2_activation; }

    // Setter for th2_activation
    void setTh2Activation(float th2_activation) { this->th2_activation = th2_activation; }

    // Getter for tfh_activation
    float getTfhActivation() const { return tfh_activation; }

    // Setter for tfh_activation
    void setTfhActivation(float tfh_activation) { this->tfh_activation = tfh_activation; }
};

#endif
