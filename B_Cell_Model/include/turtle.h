// turtle.h
#ifndef TURTLE_H
#define TURTLE_H

#include "agent.h"

// Comparator function for std::find with std::weak_ptr
template <typename T>
struct WeakPtrComparator {
    int targetID;

    WeakPtrComparator(int id) : targetID(id) {}

    bool operator()(const std::weak_ptr<T>& ptr) const {
        if (auto locked = ptr.lock()) {
            return locked->getID() == targetID;
        }
        return false;
    }
};

class Turtle : public Agent{
  public :
    Turtle(int x, int y, int ID_num, int heading=90) : Agent(x, y), ID_num(ID_num), heading(heading) {x_dec=x; y_dec=y;};
    virtual ~Turtle() = default;

  private:
    std::vector<std::weak_ptr<Turtle>> linkedTurtles;


    int type =0;  //agent type, will be used for the get-one-of functions
    int ID_num;
    int heading;
    // int age;
    int time_alive;
    bool is_alive=true;
    float x_dec; //x and y decimal coordinates
    float y_dec;
    float temp_x; // temporary coords for turtle move, before actual turtle coords are moved in case destination patch is full.
    float temp_y; // Turtle cant know about the world cause circular dependency in #include, so world will handle turtle movement between patches

    // added turtles-own variables
    bool in_blood;
    int bcr;
    std::string isotype;
    bool csr_bool;
    float tnfa_threshold;
    float breg_threshold;
    float cd21_level;
    float s1pr1_level;
    float s1pr2_level;
    float cxcr5_level;
    float ccr7_level;
    float ebi2r_level;
    bool pro_breg;
    int level_of_activation;
    float tnf_a_stimulation;
    int exposure_number;

    // add more variables that apply to all turtles here for the specific NetLogo model being ported

  public:
    std::pair<double,double> move(float distance=1);
    std::pair<int,int> jumpRandom(std::mt19937 &RNG_Engine);
    void execute_move(bool didMove);
    void wiggle(std::mt19937 &RNG_Engine);
    void display();
    void copy_other_turtle_attributes(std::shared_ptr<Turtle> otherTurtle); // meant to be used when netlogo sets breed, cause this will copy values from original cell to new cell

    int getID() const {return ID_num;};
    int getType() const {return type;};

    void setHeading(int newHeading){heading = newHeading;};
    int getHeading() const {return heading;};

    float get_x_dec() const {return x_dec;}       // already has regular getX() and getY() from agent parent class
    float get_y_dec() const {return y_dec;}
    void set_x_dec(double new_x_dec) {x_dec = new_x_dec;}
    void set_y_dec(double new_y_dec) {y_dec = new_y_dec;}

    float get_temp_x() const {return temp_x;}
    float get_temp_y() const {return temp_y;}
    void set_temp_x(double new_temp_x) {temp_x = new_temp_x;}
    void set_temp_y(double new_temp_y) {temp_y = new_temp_y;}


    void addLinkedTurtle(std::weak_ptr<Turtle> linkedTurtle);
    void removeLinkedTurtle();
    std::vector<std::weak_ptr<Turtle>>& getLinkedTurtles();




    // Add setter and getter for new variables here
    void set_is_alive(bool new_is_alive) { is_alive = new_is_alive; }
    bool get_is_alive() const { return is_alive; }
    // in_blood
    void setInBlood(bool inBlood) { in_blood = inBlood; }
    bool getInBlood() const { return in_blood; }

    // bcr
    void setBcr(int bcrValue) { bcr = bcrValue; }
    int getBcr() const { return bcr; }

    // isotype
    void setIsotype(const std::string& isotypeValue) { isotype = isotypeValue; }
    std::string getIsotype() const { return isotype; }

    // csr_bool
    void setCsrBool(bool csrBool) { csr_bool = csrBool; }
    bool getCsrBool() const { return csr_bool; }

    // time_alive
    void setTimeAlive(int timeAlive) { time_alive = timeAlive; }
    int getTimeAlive() const { return time_alive; }

    // tnfa_threshold
    void setTnfaThreshold(float threshold) { tnfa_threshold = threshold; }
    float getTnfaThreshold() const { return tnfa_threshold; }

    // breg_threshold
    void setBregThreshold(float threshold) { breg_threshold = threshold; }
    float getBregThreshold() const { return breg_threshold; }

    // cd21_level
    void setCd21Level(float level) { cd21_level = level; }
    float getCd21Level() const { return cd21_level; }

    // s1pr1_level
    void setS1pr1Level(float level) { s1pr1_level = level; }
    float getS1pr1Level() const { return s1pr1_level; }

    // s1pr2_level
    void setS1pr2Level(float level) { s1pr2_level = level; }
    float getS1pr2Level() const { return s1pr2_level; }

    // cxcr5_level
    void setCxcr5Level(float level) { cxcr5_level = level; }
    float getCxcr5Level() const { return cxcr5_level; }

    // ccr7_level
    void setCcr7Level(float level) { ccr7_level = level; }
    float getCcr7Level() const { return ccr7_level; }

    // ebi2r_level
    void setEbi2rLevel(float level) { ebi2r_level = level; }
    float getEbi2rLevel() const { return ebi2r_level; }

    // pro_breg
    void setProBreg(bool proBreg) { pro_breg = proBreg; }
    bool getProBreg() const { return pro_breg; }

    // level_of_activation
    void setLevelOfActivation(int level) { level_of_activation = level; }
    int getLevelOfActivation() const { return level_of_activation; }

    // tnf_a_stimulation
    void setTnfaStimulation(float stimulation) { tnf_a_stimulation = stimulation; }
    float getTnfaStimulation() const { return tnf_a_stimulation; }

    // exposure_number
    void setExposureNumber(int exposureNumber) { exposure_number = exposureNumber; }
    int getExposureNumber() const { return exposure_number; }

    std::weak_ptr<Turtle> createWeakTurtlePtr() {
      std::weak_ptr<Turtle> test = std::dynamic_pointer_cast<Turtle>(createWeakPtr().lock());
      std::cout<<"greated weak turtle pointer in turtle function"<<std::endl;
       return test;
     }

    bool operator== (const Turtle& turt2){return this->ID_num==turt2.ID_num;};
    bool operator== (const Turtle* turt2){return this->ID_num==turt2->ID_num;};
    bool operator== (const std::weak_ptr<Turtle> turt2){return this->ID_num==turt2.lock()->ID_num;};

    // add more turtle specific functions below. These are functions that will apply to all turtle types, not a single breed

};


#endif
