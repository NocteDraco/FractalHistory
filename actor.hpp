#ifndef ACTOR_HPP
#define ACTOR_HPP
/* Actor structures */
#include "grid.hpp"
#include <string>
using namespace std;


class Actor{


private:
    int gridX, gridY; // Grid location
    int nx, ny; // Grid size

    // Characteristics
    int Str, Dex, Con, Wis, Int, Cha, Pro; 

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
    bool seesFood = false;
    bool seesWater = false;
    bool movingFood = false;
    bool movingWater = false;

    // Identifiers
    string name; // Name of the actor
    unsigned int uuid; // Unique identifier
    

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
    void setSightFood(bool seesFood){this->seesFood = seesFood;}
    void setSightWater(bool seesWater){this->seesWater = seesWater;}
    void setMovingFood(bool movingFood){this->movingFood = movingFood;}
    void setMovingWater(bool movingWater){this->movingWater = movingWater;}

    // Identifiers
    void setName(string newName){name = newName;}
    void setUUID(uint64_t newUUID){uuid = newUUID;}

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


    // Property
    int getFood(){return food;}
    int getWater(){return water;}
    int getHealth(){return health;}
    int getCarry(){return carry;}

    // State
    bool getStarving(){return starving;}
    bool getDying(){return dying;}
    bool getAlive(){return alive;}
    bool getSightFood(){return seesFood;}
    bool getSightWater(){return seesWater;}
    bool getMovingFood(){return movingFood;}
    bool getMovingWater(){return movingWater;}

    // Identifier
    string getName(){return name;}
    unsigned int getUUID(){return uuid;}
    
    //
    // OPERATIONS
    //
    void moveOnGrid(int, int);
    void incrementDay();
    void incrementTime(Grid*);
    int getTotalWeight();
    bool encumbered();
    void checkGrid(Grid*);
    bool lookForFood(Grid*, int*, int*, float*);
    void grabFood(Grid*);
    void determineActivity(); // Given states, determine next activity to take


};



#endif //ACTOR_H