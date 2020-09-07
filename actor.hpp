#ifndef ACTOR_HPP
#define ACTOR_HPP
/* Actor structures */
#include <string>
#include <vector>
#include "constants.hpp"
#include "grid.hpp"
#include "math_utils.hpp"

using namespace std;


class Vision
{

public:
    // Material looks
    int xfood, yfood;
    int xwater, ywater;
    float distFood, distWater;
    bool seesFood = false;
    bool seesWater = false;
    bool movingFood = false;
    bool movingWater = false;

    void setPosFood(int xfood, int yfood, float distFood)
    {
        this->xfood = xfood; 
        this->yfood = yfood;
        this->distFood = distFood;
    }
    void setPosWater(int xwater, int ywater, float distWater)
    {
        this->xwater = xwater;
        this->ywater = ywater;
        this->distWater = distWater;
    }

    int needMoveFood()
    {
        if (!getSightFood())
        {   
            // Can't see any food
            return CANT_SEE_FOOD;
        }
        if ((xfood != 0) | (yfood != 0))
        {
            // Can see food but aren't at it
            return SEE_FOOD_CAN_MOVE;
        }
        return STANDING_ON_FOOD;
    }
    
    void getMoveFood(int *x, int *y, int move_total = 1)
    {
        *x = clip(xfood, -1, 1);
        *y = clip(yfood, -1, 1);
    }
    float getDistFood(){return distFood;}
    float getDistWater(){return distWater;}

    // Setters, basic
    void setSightFood(bool seesFood){this->seesFood = seesFood;}
    void setSightWater(bool seesWater){this->seesWater = seesWater;}
    void setMovingFood(bool movingFood){this->movingFood = movingFood;}
    void setMovingWater(bool movingWater){this->movingWater = movingWater;}

    // Getters, basic
    bool getSightFood(){return seesFood;}
    bool getSightWater(){return seesWater;}
    bool getMovingFood(){return movingFood;}
    bool getMovingWater(){return movingWater;}

};

class DecisionMaker{
public:
    // Properties
    int food = 10; // Remaining food (in units of days)
    int water = 10; // Remaining water (in units of days)
    int health = 5; // Total health of actor (0 = dead, 10 = dying)
    int carry = 20; // Max amount of carrying capacity  

    // States
    bool starving = false; 
    bool dehydrated = false;
    bool dying = false; 
    bool alive = true; 



    // Property
    void setFood(int food){this->food = food;}
    void addFood(int food){this->food += food;}
    void setWater(int water){this->water = water;}
    void addWater(int water){this->water += water;}
    void setHealth(int health){this->health = health;}
    void addHealth(int health){this->health += health;}

    // State
    void setStarving(bool newStarving){starving = newStarving;}
    void setDying(bool newDying){dying = newDying;}
    void setAlive(bool newAlive){alive = newAlive;}

    // Property
    int getFood(){return food;}
    int getWater(){return water;}
    int getHealth(){return health;}
    int getCarry(){return carry;}

    // State
    bool getStarving(){return starving;}
    bool getDying(){return dying;}
    bool getAlive(){return alive;}
};

class Actor: public Vision, public DecisionMaker{


private:
    int gridX, gridY; // Grid location
    int nx, ny; // Grid size
    int moveIntId = IS_MOVING_FOOT;

    // Characteristics
    int Str, Dex, Con, Wis, Int, Cha, Pro;
    int numActions = 0; // Number of actions available in this increment
    int maxActions = 3; // Maximum number of actions available per increment
    
    // Identifiers
    string name; // Name of the actor
    unsigned int uuid; // Unique identifier
    string lastAction; // last action taken

    
    int curHistory[MAX_HISTORY] = {0};
    unsigned int histInd = 0;

public:

    // Constructor
    Actor(int iX, 
          int iY,
          int nx,
          int ny,
          int Str, 
          int Dex, 
          int Con,
          int Wis, 
          int Int, 
          int Cha,
          int Pro,
          int uuid){
        this->nx = nx;
        this->ny = ny;
        setGrid(iX, iY);
        this->Str = Str;
        this->Dex = Dex;
        this->Con = Con;
        this->Wis = Wis;
        this->Int = Int;
        this->Cha = Cha;
        this->Pro = Pro;
        this->uuid = uuid;
    }

    //
    // SETTERS
    //

    // Position and view
    void setGrid(int, int);
    void addGridX(int gridX){this->gridX += gridX;}
    void addGridY(int gridY){this->gridY += gridY;}
    void setView();

    // Stat
    void setStr(int Str){this->Str = Str;}
    void setDex(int Dex){this->Dex = Dex;}
    void setCon(int Con){this->Con = Con;}
    void setWis(int Wis){this->Wis = Wis;}
    void setInt(int Int){this->Int = Int;}
    void setCha(int Cha){this->Cha = Cha;}
    void setPro(int Pro){this->Pro = Pro;}

    void setActions(int numActions){this->numActions = numActions;}
    void addActions(int numActions){this->numActions += numActions;}
    void addMaxActions(int maxActions){this->maxActions += maxActions;}
    void setMaxActions(int maxActions){this->maxActions = maxActions;}
    void setLastAction(string lastAction){this->lastAction = lastAction;}


    // Identifiers
    void setName(string newName){name = newName;}
    void setUUID(uint64_t newUUID){uuid = newUUID;}

    // History
    void addHistInt(int history_id_int)
    {
        if (histInd < MAX_HISTORY)
        {
        curHistory[histInd] = history_id_int;
        histInd++;
        }
    }
    void resetHist()
    {
        for (int i = 0; i < MAX_HISTORY; i++)
        {
            curHistory[histInd] = 0;
        }
    }

    //
    // GETTERS
    //
    
    // Positions
    int getGridX(){ return gridX;}
    int getGridY(){ return gridY;}

    // Stat
    int getStr(){ return Str;}
    int getDex(){ return Dex;}
    int getCon(){ return Con;}
    int getWis(){ return Wis;}
    int getInt(){ return Int;}
    int getCha(){ return Cha;}
    int getPro(){ return Pro;}

    int getActions(){ return numActions;}
    int getMaxActions(){ return maxActions;}
    string getLastAction(){ return lastAction;}

    // Identifier
    string getName(){return name;}
    unsigned int getUUID(){return uuid;}

    // History
    int getHistAtInd(unsigned int ihistInd)
    {
        return curHistory[ihistInd];
    }
    
    //
    // OPERATIONS
    //
    void moveOnGrid(int, int);
    void incrementDay();
    void incrementTime(Grid*);
    int takeActions(Grid*, int);
    int getTotalWeight();
    bool encumbered();
    void checkGrid(Grid*);
    void lookForFood(Grid*);
    int doFoodActions(Grid*);
    int doExploreActions(Grid*);
    int grabFood(Grid*);
    void determineActivity(); // Given states, determine next activity to take
};

#endif //ACTOR_H