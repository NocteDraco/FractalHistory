#ifndef ACTOR_HPP
#define ACTOR_HPP
/* Actor structures */
#include <string>
#include <vector>
#include <map>
#include "constants.hpp"
#include "grid.hpp"
#include "math_utils.hpp"

using namespace std;


class StatHistory
{
    // Tracking both raw stat rolls and skill check rolls
private:
    typedef map <int, int> TrackStatHistory;
    TrackStatHistory myStatHistory;
    
public:

    StatHistory()
    {
        myStatHistory[STR] = 0;
        myStatHistory[DEX] = 0;
        myStatHistory[CON] = 0;
        myStatHistory[INT] = 0;
        myStatHistory[WIS] = 0;
        myStatHistory[CHA] = 0;
        // myStatHistory["athletics"] = 0;
        // myStatHistory["acrobatics"] = 0;
        // myStatHistory["sleight_of_hand"] = 0;
        // myStatHistory["stealth"] = 0;
        // myStatHistory["arcana"] = 0;
        // myStatHistory["history"] = 0;
        // myStatHistory["investigation"] = 0;
        // myStatHistory["nature"] = 0;
        // myStatHistory["religion"] = 0;
        // myStatHistory["animal_handling"] = 0;
        // myStatHistory["insight"] = 0;
        // myStatHistory["medicine"] = 0;
        // myStatHistory["perception"] = 0;
        // myStatHistory["survival"] = 0;
        // myStatHistory["deception"] = 0;
        // myStatHistory["intimidation"] = 0;
        // myStatHistory["performance"] = 0;
        // myStatHistory["persuasion"] = 0;

    }
    
    
    // Public interface.
    //    Returns a const reference to the value.
    //    The interface use static methods (means we dont need an instance)
    //    Internally we refer to the only instance.
    int getStatCount(int const& key)
    {
        // Use find rather than operator[].
        // This way you dont go inserting garbage into your data store.
        // Also it allows the data store to be const (as operator may modify the data store
        // if the value is not found).

        TrackStatHistory::const_iterator  iFind    = myStatHistory.find(key);
        if (iFind != myStatHistory.end())
        {
            // If we find it return the value.
            return iFind->second;
        }

        // What happens when we don;t find anything.
        // Your original code created a garbage entry and returned that.
        // Could throw an exception or return a temporary reference.
        // Maybe ->  throw int(1);
        return -1;
    }

    void incStatCount(int const& key)
    {
        // NOTE: Don't use const_iterator here since we need to modify
        // the value at it's location
        TrackStatHistory::iterator  iFind    = myStatHistory.find(key);
        
        // Only change the values that are in the map
        if (iFind != myStatHistory.end())
        {
            iFind->second += 1;
        }

    }

};

class ResourceAwareness
{
private:
    typedef map <int, bool> ResAwaMap;
    ResAwaMap myResSightMap;
    typedef map <int, int> ResPosMap;
    ResPosMap myResPosXMap, myResPosYMap;

public:
    ResourceAwareness()
    {
        myResSightMap[RES_WATER]            = false;
        myResSightMap[RES_AIR]              = false;
        myResSightMap[RES_SHELTER]          = false;
        myResSightMap[RES_PATH]             = false;
        myResSightMap[RES_GROUND]           = false;
        myResSightMap[RES_METAL]            = false;
        myResSightMap[RES_FOOD_ANIMAL]      = false;
        myResSightMap[RES_FOOD_PLANT]       = false;
        
        myResPosXMap[RES_WATER]            = 0;
        myResPosXMap[RES_AIR]              = 0;
        myResPosXMap[RES_SHELTER]          = 0;
        myResPosXMap[RES_PATH]             = 0;
        myResPosXMap[RES_GROUND]           = 0;
        myResPosXMap[RES_METAL]            = 0;
        myResPosXMap[RES_FOOD_ANIMAL]      = 0;
        myResPosXMap[RES_FOOD_PLANT]       = 0;

        myResPosYMap[RES_WATER]            = 0;
        myResPosYMap[RES_AIR]              = 0;
        myResPosYMap[RES_SHELTER]          = 0;
        myResPosYMap[RES_PATH]             = 0;
        myResPosYMap[RES_GROUND]           = 0;
        myResPosYMap[RES_METAL]            = 0;
        myResPosYMap[RES_FOOD_ANIMAL]      = 0;
        myResPosYMap[RES_FOOD_PLANT]       = 0;
        
    }

    int getResourceSight(int const& key, bool *canSeeResource)
    {
        // Construct iterator to find it (make constant so we don't 
        // mess up data). See https://stackoverflow.com/questions/3113979/how-to-create-map-with-keys-values-inside-class-body-once-not-each-time-functio
        ResAwaMap::const_iterator  iFind    = myResSightMap.find(key);
        if (iFind != myResSightMap.end())
        {
            // If we find it return the value.
            *canSeeResource = iFind->second;
            return 1; //Success
        }
        return 0;
    }

    bool getResourceSightOut(int const& key)
    {
        bool retVal;
        getResourceSight(key, &retVal);
        return retVal;
    }

    int setResourceSight(int const& key, bool canSeeResource)
    {
        // NOTE: Don't use const_iterator here since we need to modify
        // the value at it's location
        ResAwaMap::iterator  iFind    = myResSightMap.find(key);
        
        // Only change the values that are in the map
        if (iFind != myResSightMap.end())
        {
            iFind->second = canSeeResource;
            return 1; // Success
        }
        return 0; // Failed

    }

    int getResourcePos(int const& key, int *xPos, int *yPos)
    {
        bool fXPos = false, fYPos = false;
        // Construct iterator to find it (make constant so we don't 
        // mess up data). See https://stackoverflow.com/questions/3113979/how-to-create-map-with-keys-values-inside-class-body-once-not-each-time-functio
        ResPosMap::const_iterator  iFindX    = myResPosXMap.find(key);
        if (iFindX != myResPosXMap.end())
        {
            // If we find it return the value.
            *xPos = iFindX->second;
            fXPos = true;
        }
        ResPosMap::const_iterator  iFindY    = myResPosYMap.find(key);
        if (iFindY != myResPosYMap.end())
        {
            // If we find it return the value.
            *yPos = iFindY->second;
            fYPos = true;
        }
        if (fXPos & fYPos)
        {
            return 1; // Success
        }

        return 0;
    }

    int getResourceDist(int const& key, float *dist)
    {
        int xPos, yPos;
        getResourcePos(key, &xPos, &yPos);
        *dist = eucl_dist_grid(xPos, yPos, 0);
        return 1;
        
    }

    int setResourcePos(int const& key, int xPos, int yPos)
    {
        bool setXPos = false, setYPos = false;
        // NOTE: Don't use const_iterator here since we need to modify
        // the value at it's location
        ResPosMap::iterator  iFindX    = myResPosXMap.find(key);
        
        // Only change the values that are in the map
        if (iFindX != myResPosXMap.end())
        {
            iFindX->second = xPos;
            setXPos = true;
        }

        // NOTE: Don't use const_iterator here since we need to modify
        // the value at it's location
        ResPosMap::iterator  iFindY    = myResPosYMap.find(key);
        
        // Only change the values that are in the map
        if (iFindY != myResPosYMap.end())
        {
            iFindY->second = yPos;
            setYPos = true;
        }
        if (setXPos & setYPos)
        {
            return 1; //Success
        }
        return 0; // Failed

    }

};


class ActorInventory
{
private:
    typedef std::map <int, int> InvMap;
    InvMap myInv;

    int carry = 100;
public:
    ActorInventory()
    {
        // NECESSITIES
        myInv[RES_WATER]                = 100;
        myInv[RES_FOOD_ANIMAL]          = 0;
        myInv[RES_FOOD_PLANT]           = 100;

        // OTHER RESOURCES
        myInv[RES_METAL]                = 0;
        myInv[RES_WOOD]                 = 0;
        myInv[RES_MAGIC]                = 0;
        myInv[RES_COIN]                 = 0;

    }

    int getInvVal(int const&, int*);
    int getInvValOut(int const&);
    int setInvVal(int const&, int);
    int addInvVal(int const&, int);
    int getInvWeight(int *);
    
    int getCarry(){return carry;}
    void setCarry(int carry){this->carry = carry;}
};


class DecisionMaker{
private:
    typedef std::map <int, bool> StateMap;
    StateMap myStateMap;
    typedef std::map <int, int> InterestMap;
    InterestMap myInterest;
public:
    int inefficientFactor = 10; 
    // How bad they are at skills (lower is better)

    DecisionMaker()
    {
        myStateMap[STATE_ALIVE]             = true;
        myStateMap[STATE_STARVING]          = false;
        myStateMap[STATE_DEHYDRATED]        = false;
        myStateMap[STATE_HUNGRY]            = false;
        myStateMap[STATE_PARCHED]           = false;

        // Random interest generation
        int rWealth, rSocial, rBattle, rExplore, rLazy, rTotal;
        rWealth = rand()%100;
        rSocial = rand()%100;
        rBattle = rand()%100;
        rLazy   = rand()%100;
        rExplore = rand()%100;
        rTotal = rWealth + rSocial + rBattle + rExplore + rLazy;
        myInterest[INTEREST_WEALTH]         = rWealth;
        myInterest[INTEREST_SOCIAL]         = rSocial;
        myInterest[INTEREST_BATTLE]         = rBattle;
        myInterest[INTEREST_EXPLORE]        = rExplore;
        myInterest[INTEREST_LAZY]           = rLazy;
        myInterest[INTEREST_TOTAL]          = rTotal;

    }
    int getStateVal(int const&, bool*);
    bool getStateValOut(int const&);
    int setStateVal(int const&, bool);

    int getInterestVal(int const&, int*);
    int getInterestValOut(int const&);
};

class Actor: \
public DecisionMaker,\
public StatHistory, \
public ResourceAwareness,\
public ActorInventory{


private:
    int gridX, gridY; // Grid location
    int nx, ny; // Grid size
    int moveIntId = IS_MOVING_FOOT;

    // Characteristics
    int Str, Dex, Con, Wis, Int, Cha, Pro;
    int numActions = 0; // Number of actions available in this increment
    int maxActions = 3; // Maximum number of actions available per increment
    int health = 10; // Total health of the actor;
    
    // Identifiers
    string name; // Name of the actor
    unsigned int uuid; // Unique identifier
    
    int curHistory[MAX_HISTORY] = {0};
    unsigned int histInd = 0;

public:

    // Constructor
    Actor(int iX, 
          int iY,
          int nx,
          int ny,
          int uuid,
          int Str = 10, 
          int Dex = 10, 
          int Con = 10,
          int Wis = 10, 
          int Int = 10, 
          int Cha = 10,
          int Pro = 0
          ){
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

    void setHealth(int health){this->health = health;}
    void addHealth(int health){this->health += health;}
    int getHealth(){return health;}

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
    int getPriorityAction();

    // Resource finding
    void lookForResource(Grid*, int);
    int resSightSkillCheck(int);
    int doSkillCheck(int);
    float getDistResource(int);
    void gatherResourceGrid(Grid*, int, int);
    void resourceGatherSkillCheck(int, int*);
    int doResourceActions(Grid*, int);
    int needResource(int, bool*);

    int doExploreActions(Grid*);
    int grabFood(Grid*, int);
    void determineActivity(); // Given states, determine next activity to take
};

#endif //ACTOR_H