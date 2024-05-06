#include <iostream>
#include <string>
#include <vector>
#include "patch.h"
#include <algorithm>
#include <fstream>

float Patch::getOxy() const {
 return oxy;
}

void Patch::setOxy(float value) {
 oxy = value;
}

float Patch::getEcActivation() const {
 return ec_activation;
}

void Patch::setEcActivation(float value) {
 ec_activation = value;
}

float Patch::getEcRoll() const {
 return ec_roll;
}

void Patch::setEcRoll(float value) {
 ec_roll = value;
}

float Patch::getEcStick() const {
 return ec_stick;
}

void Patch::setEcStick(float value) {
 ec_stick = value;
}

float Patch::getEcMigrate() const {
 return ec_migrate;
}

void Patch::setEcMigrate(float value) {
 ec_migrate = value;
}

float Patch::getCytotox() const {
 return cytotox;
}

void Patch::setCytotox(float value) {
 cytotox = value;
}

float Patch::getInfection() const {
 return infection;
}

void Patch::setInfection(float value) {
 infection = value;
}

float Patch::getEndotoxin() const {
 return endotoxin;
}

void Patch::setEndotoxin(float value) {
 endotoxin = value;
}

float Patch::getPAF() const {
 return PAF;
}

void Patch::setPAF(float value) {
 PAF = value;
}

float Patch::getTNF() const {
 return TNF;
}

void Patch::setTNF(float value) {
 TNF = value;
}

float Patch::getSTNFr() const {
 return sTNFr;
}

void Patch::setSTNFr(float value) {
 sTNFr = value;
}

float Patch::getIL_1() const {
 return IL_1;
}

void Patch::setIL_1(float value) {
 IL_1 = value;
}

float Patch::getSIL_1r() const {
 return sIL_1r;
}

void Patch::setSIL_1r(float value) {
 sIL_1r = value;
}

float Patch::getIL_1ra() const {
 return IL_1ra;
}

void Patch::setIL_1ra(float value) {
 IL_1ra = value;
}

float Patch::getINF_g() const {
 return INF_g;
}

void Patch::setINF_g(float value) {
 INF_g = value;
}

float Patch::getIL_2() const {
 return IL_2;
}

void Patch::setIL_2(float value) {
 IL_2 = value;
}

float Patch::getIL_4() const {
 return IL_4;
}

void Patch::setIL_4(float value) {
 IL_4 = value;
}

float Patch::getIL_6() const {
 return IL_6;
}

void Patch::setIL_6(float value) {
 IL_6 = value;
}

float Patch::getIL_8() const {
 return IL_8;
}

void Patch::setIL_8(float value) {
 IL_8 = value;
}

float Patch::getIL_10() const {
 return IL_10;
}

void Patch::setIL_10(float value) {
 IL_10 = value;
}

float Patch::getIL_12() const {
 return IL_12;
}

void Patch::setIL_12(float value) {
 IL_12 = value;
}

float Patch::getGCSF() const {
 return GCSF;
}

void Patch::setGCSF(float value) {
 GCSF = value;
}

/**
 * Adds a turtle to the list of turtles present on this patch.
 * @param turtle_to_add The shared pointer to the turtle to be added.
 */
void Patch::add_turtle(std::shared_ptr<Turtle> turtle_to_add) {
    turtles_here.emplace_back(turtle_to_add);
}

/**
 * Removes a specified turtle from the list of turtles present on this patch.
 * This method finds the turtle in the vector and removes it.
 * @param turtle_to_remove The shared pointer to the turtle to be removed.
 */
void Patch::remove_turtle(std::shared_ptr<Turtle> turtle_to_remove) {
    // Uses the erase-remove idiom to efficiently remove the turtle from the vector.
    turtles_here.erase(std::remove(turtles_here.begin(), turtles_here.end(), turtle_to_remove), turtles_here.end());
}

/**
 * Displays information about this patch, including its position and any turtles present.
 * This method iterates over all turtles on the patch and calls their display method.
 */
void Patch::display() {
    std::cout << "Patch position: (" << this->getX() << ", " << getY() << ")" << std::endl;
    // Displaying the patch color is optional and can be uncommented if needed.
    // std::cout << "Patch color: " << getColor() << std::endl;
    if (!turtles_here.empty()) { // Checks if there are turtles on the patch.
        for (auto &turtle : turtles_here) {
            turtle->display(); // Calls the display method for each turtle.
        }
    }
}

/**
 * Checks if the number of turtles on this patch has reached the maximum capacity.
 * @return True if at maximum capacity, otherwise false.
 */
bool Patch::is_at_capacity() {
    return (turtles_here.size() >= PATCH_MAX_CAPACITY);
}

// Additional patch-specific functionalities can be implemented here, following the simulation's requirements.
