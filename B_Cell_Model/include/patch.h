// patch.h
#ifndef PATCH_H
#define PATCH_H

#include "parameters.h"
#include "agent.h"
#include "turtle.h"
#include <memory>
#include <vector>

class Patch : public Agent{
private:
    std::vector<std::shared_ptr<Turtle>> turtles_here;

    int patch_type;
    double s1p_level, cxcl13_level, ccl19_level, ebi2_level;
    double il2, il4, il6, il10, il12, il15, il21;
    double if_g, if_a, tnf_a, tgf_b;

    double temp_var; // placeholder for diffusion totals before updating
    double temp_s1p_level, temp_cxcl13_level, temp_ccl19_level, temp_ebi2_level;
    double temp_il2, temp_il4, temp_il6, temp_il10, temp_il12, temp_il15, temp_il21;
    double temp_if_g, temp_if_a, temp_tnf_a, temp_tgf_b;

public:
    virtual ~Patch() = default;
    Patch():Agent(0,0){setColor("red");setModeColor("green");}; // base patch color is red
    Patch(int xCoord, int yCoord) : Agent(xCoord, yCoord) {setColor("red");setModeColor("green");};

    void setModeColor(std::string newColor){setSecondaryColor(newColor);}
    std::string getModeColor(){return getSecondaryColor();}

// In order to view a cytokine layer on top of everything,
// specify what mode to view here by putting a patch variable, otherwise return 0
    double getModeOpacity(){return il21;}

    void add_turtle(std::shared_ptr<Turtle> turtle_to_add);
    void remove_turtle(std::shared_ptr<Turtle> turtle_to_remove);
    const std::vector<std::shared_ptr<Turtle>>& getTurtlesHere() const {return turtles_here;}
    void display();
    bool is_at_capacity();

    void setTempVar(double newVal){temp_var=newVal;};
    double getTempVar() const {return temp_var;};

    void setPatchType(int type) { patch_type = type; }
    int getPatchType() const { return patch_type; }

    // Getter and setter for each new variable
    // s1p_level
    void setS1pLevel(double level) { s1p_level = level; }
    double getS1pLevel() const { return s1p_level; }

    // cxcl13_level
    void setCxcl13Level(double level) { cxcl13_level = level; }
    double getCxcl13Level() const { return cxcl13_level; }

    // ccl19_level
    void setCcl19Level(double level) { ccl19_level = level; }
    double getCcl19Level() const { return ccl19_level; }

    // ebi2_level
    void setEbi2Level(double level) { ebi2_level = level; }
    double getEbi2Level() const { return ebi2_level; }

    // il2
    void setIl2(double value) { il2 = value; }
    double getIl2() const { return il2; }

    // il4
    void setIl4(double value) { il4 = value; }
    double getIl4() const { return il4; }

    // il6
    void setIl6(double value) { il6 = value; }
    double getIl6() const { return il6; }

    // il10
    void setIl10(double value) { il10 = value; }
    double getIl10() const { return il10; }

    // il12
    void setIl12(double value) { il12 = value; }
    double getIl12() const { return il12; }

    // il15
    void setIl15(double value) { il15 = value; }
    double getIl15() const { return il15; }

    // il21
    void setIl21(double value) { il21 = value; }
    double getIl21() const { return il21; }

    // if_g
    void setIfG(double value) { if_g = value; }
    double getIfG() const { return if_g; }

    // if_a
    void setIfA(double value) { if_a = value; }
    double getIfA() const { return if_a; }

    // tnf_a
    void setTnfA(double value) { tnf_a = value; }
    double getTnfA() const { return tnf_a; }

    // tgf_b
    void setTgfB(double value) { tgf_b = value; }
    double getTgfB() const { return tgf_b; }

    // Getter and setter functions for temp_s1p_level
    void setTempS1pLevel(double value) { temp_s1p_level = value; }
    double getTempS1pLevel() const { return temp_s1p_level; }

    // Getter and setter functions for temp_cxcl13_level
    void setTempCxcl13Level(double value) { temp_cxcl13_level = value; }
    double getTempCxcl13Level() const { return temp_cxcl13_level; }

    // Getter and setter functions for temp_ccl19_level
    void setTempCcl19Level(double value) { temp_ccl19_level = value; }
    double getTempCcl19Level() const { return temp_ccl19_level; }

    // Getter and setter functions for temp_ebi2_level
    void setTempEbi2Level(double value) { temp_ebi2_level = value; }
    double getTempEbi2Level() const { return temp_ebi2_level; }

    // Getter and setter functions for temp_il2
    void setTempIl2(double value) { temp_il2 = value; }
    double getTempIl2() const { return temp_il2; }

    // Getter and setter functions for temp_il4
    void setTempIl4(double value) { temp_il4 = value; }
    double getTempIl4() const { return temp_il4; }

    // Getter and setter functions for temp_il6
    void setTempIl6(double value) { temp_il6 = value; }
    double getTempIl6() const { return temp_il6; }

    // Getter and setter functions for temp_il10
    void setTempIl10(double value) { temp_il10 = value; }
    double getTempIl10() const { return temp_il10; }

    // Getter and setter functions for temp_il12
    void setTempIl12(double value) { temp_il12 = value; }
    double getTempIl12() const { return temp_il12; }

    // Getter and setter functions for temp_il15
    void setTempIl15(double value) { temp_il15 = value; }
    double getTempIl15() const { return temp_il15; }

    // Getter and setter functions for temp_il21
    void setTempIl21(double value) { temp_il21 = value; }
    double getTempIl21() const { return temp_il21; }

    // Getter and setter functions for temp_if_g
    void setTempIfG(double value) { temp_if_g = value; }
    double getTempIfG() const { return temp_if_g; }

    // Getter and setter functions for temp_if_a
    void setTempIfA(double value) { temp_if_a = value; }
    double getTempIfA() const { return temp_if_a; }

    // Getter and setter functions for temp_tnf_a
    void setTempTnfA(double value) { temp_tnf_a = value; }
    double getTempTnfA() const { return temp_tnf_a; }

    // Getter and setter functions for temp_tgf_b
    void setTempTgfB(double value) { temp_tgf_b = value; }
    double getTempTgfB() const { return temp_tgf_b; }

    // patch is the same iff the x and y positions are equal
    bool operator== (Patch& that){return (this->getX()==that.getX() && this->getY()==that.getY());};

    // Add more patch specific functions below for the particular implementation of a net logo model being ported
};

#endif
