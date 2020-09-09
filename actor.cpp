#include <iostream> 
#include <cstdlib>
#include <assert.h>
#include "actor.hpp"
#include "skills.hpp"

using namespace std;

//
//
// ActorInventory
//
//
int ActorInventory::getInvVal(int const& key, int *invVal)
{
    InvMap::const_iterator iFind = myInv.find(key);
    if (iFind != myInv.end())
    {
        *invVal = iFind->second;
        return 1;
    }
    return 0;
}

int ActorInventory::getInvValOut(int const& key)
{
    int retVal;
    getInvVal(key, &retVal);
    return retVal;
}


int ActorInventory::setInvVal(int const& key, int invVal)
{
    InvMap::iterator iFind = myInv.find(key);
    if (iFind != myInv.end())
    {
        iFind->second = invVal;
        return 1;
    }
    return 0;
}

int ActorInventory::addInvVal(int const& key, int invVal)
{
    InvMap::iterator iFind = myInv.find(key);
    if (iFind != myInv.end())
    {
        iFind->second += invVal;
        return 1;
    }
    return 0;
}

int ActorInventory::getInvWeight(int *invWeight)
{
    // Add all keys together
    int keysToAdd[] = {RES_WATER, RES_FOOD_PLANT, RES_FOOD_ANIMAL,\
                       RES_METAL, RES_WOOD};
    int nKeys = sizeof(keysToAdd) / sizeof(keysToAdd[0]);
    int tVal;
    *invWeight = 0; // Empty at first

    for (int i = 0; i < nKeys; i++)
    {
        getInvVal(keysToAdd[i], &tVal);
        invWeight += tVal;
    }

    return 1;
}
//
//
// ActorInventory
//
//

//
//
// DecisionMaker
//
//
int DecisionMaker::getStateVal(int const& key, bool *state)
{
    StateMap::const_iterator iFind = myStateMap.find(key);
    if (iFind != myStateMap.end())
    {
        *state = iFind->second;
        return 1; // Success
    }
    return 0;
}

bool DecisionMaker::getStateValOut(int const& key)
{
    bool retVal;
    getStateVal(key, &retVal);
    return retVal;
}

int DecisionMaker::setStateVal(int const& key, bool state)
{
    StateMap::iterator iFind = myStateMap.find(key);
    if (iFind != myStateMap.end())
    {
        iFind->second = state;
        return 1; // Success
    }
    return 0;
}

int DecisionMaker::getInterestVal(int const& key, int *interestVal)
{
    InterestMap::const_iterator iFind = myInterest.find(key);
    if (iFind != myInterest.end())
    {
        *interestVal = iFind->second;
        return 1;
    }
    return 0;
}

int DecisionMaker::getInterestValOut(int const& key)
{
    int interestVal;
    getInterestVal(key, &interestVal);
    return interestVal;
}
//
//
// DecisionMaker
//
//

//
//
// Actor
//
//
void Actor::moveOnGrid(int gridXStep, int gridYStep)
{
    addHistInt(moveIntId);
    // X
    int newGridX = gridX + gridXStep;

    // Bound check x
    if ((newGridX < 0) | (newGridX > nx))
    {
        newGridX = gridX;
    }

    // Y
    int newGridY = gridY + gridYStep;

    // Bound check y
    if ((newGridY < 0) | (newGridY > ny))
    {
        newGridY = gridY;
    }

    // Do move
    gridX = newGridX;
    gridY = newGridY;
}

void Actor::setGrid(int newGridX, int newGridY)
{
    gridX = newGridX;
    gridY = newGridY;
}

void Actor::incrementDay()
{
    /* 
    A bunch of checks about the state of the actor
    */

    // Declarations upfront
    int foodAnimal = 0, foodPlant = 0, totalFood = 0;

    //
    // WATER
    //
    if (getInvValOut(RES_WATER) <= 0)
    {
        setStateVal(STATE_DEHYDRATED, true);
    }
    else if (getInvValOut(RES_WATER) > 0)
    {
        addInvVal(RES_WATER, -1);
    }

    //
    //FOOD
    //

    // Get food and total
    getInvVal(RES_FOOD_ANIMAL, &foodAnimal);
    getInvVal(RES_FOOD_PLANT, &foodPlant);
    totalFood = foodAnimal + foodPlant;


    // Run through logic of food
    if (totalFood <= 0)
    {
        setStateVal(STATE_STARVING, true);
    }
    else if (totalFood <= 10)
    {
        setStateVal(STATE_HUNGRY, false);
    }
    else if (foodAnimal > 0)
    {
        addInvVal(RES_FOOD_ANIMAL, -1);
    }
    else if (foodPlant > 0)
    {
        addInvVal(RES_FOOD_PLANT, -1);
    }

    // Reset states if we've exited them
    if (totalFood > 0)
    {
        setStateVal(STATE_STARVING, false);
        if (totalFood > 10)
        {
         setStateVal(STATE_HUNGRY, false);   
        }
    }

    
    // Starving but alive
    if (getStateValOut(STATE_STARVING) & getStateValOut(STATE_ALIVE))
    {
        addHealth(-1);
    }

    // Dead
    if (getHealth() <= 0)
    {
        setStateVal(STATE_ALIVE, false);
    }

}

int Actor::doSkillCheck(int SKILL_ID)
{
    switch(SKILL_ID)
    {
        // Strenth
        case ATHLETICS:
            incStatCount(STR);
            return profSkillCheck(Str, Pro, 1);
            
        // Dexterity
        case ACROBATICS:
        case SLEIGHT_OF_HAND:
        case STEALTH:
            incStatCount(DEX);
            return profSkillCheck(Dex, Pro, 1);
            
        // Intelligence
        case ARCANA:
        case HISTORY:
        case INVESTIGATION:
        case NATURE:
        case RELIGION:
            incStatCount(INT);
            return profSkillCheck(Int, Pro, 1);
        
        // Wisdom
        case ANIMAL_HANDLING:
        case INSIGHT:
        case MEDICINE:
        case PERCEPTION:
        case SURVIVAL:
            incStatCount(WIS);
            return profSkillCheck(Wis, Pro, 1);
        
        // Charisma
        case DECEPTION:
        case INTIMIDATION:
        case PERFORMANCE:
        case PERSUASION:
            incStatCount(CHA);
            return profSkillCheck(Cha, Pro, 1);
    }
    return 0;

}

int Actor::resSightSkillCheck(int RESOURCE_ID)
{
    // Given the actor's skills, determine how far they can look for
    // the resource they are looking for
    switch (RESOURCE_ID)
    {
        case RES_WATER:
        case RES_FOOD_ANIMAL:
        case RES_FOOD_PLANT:
        case RES_SHELTER:
            return doSkillCheck(SURVIVAL)/inefficientFactor;
            
    }
    return 0;
}

void Actor::resourceGatherSkillCheck(int RESOURCE_ID, int *amountGathered)
{
    *amountGathered = 0;
    // Given the actor's skills, determine how much they can gather
    // of the resource
    switch (RESOURCE_ID)
    {
        case RES_FOOD_ANIMAL:
            *amountGathered = doSkillCheck(SURVIVAL)/inefficientFactor;

        case RES_FOOD_PLANT:
        case RES_WATER:
            *amountGathered = doSkillCheck(NATURE)/inefficientFactor;


        
    }
   
}

void Actor::gatherResourceGrid(Grid *actGrid, int RESOURCE_ID, int amount)
{
    // Declarations
    int resAtGrid = 0, ind, canGrabAmt = 0, doGrabAmt = 0, invWeight = 0;
    // Current grid index of actor
    ind = ny * getGridY() + getGridX();

    // Get the total at the grid
    actGrid[ind].getGridResVal(RESOURCE_ID, &resAtGrid);

    // Of what we tried to gather, how much can grab from the terrain?
    canGrabAmt = min(amount, resAtGrid);

    // How much can we hold?
    getInvWeight(&invWeight);
    doGrabAmt = min(canGrabAmt, getCarry() - invWeight);

    // Remove strange negatives
    doGrabAmt = max(doGrabAmt, 0);

    if (doGrabAmt > 0)
    {
        // Add it to our inventory TODO !!!!!!!!!!!!!
        addInvVal(RESOURCE_ID, doGrabAmt);

        // Remove it from the grid
        actGrid[ind].addGridResVal(RESOURCE_ID, -1 * doGrabAmt);
    }


    if (doGrabAmt >= resAtGrid)
    {
        // We gathered as much as available here, will need to look again
        setResourceSight(RESOURCE_ID, false);
    }

}

void Actor::lookForResource(Grid *actGrid, int RESOURCE_ID)
{
    // Current grid index of actor
    int ind = ny * getGridY() + getGridX();
    int ind_n;
    bool canSeeResource;
    int resAtGrid;

    float jdist; // Distance to resource
    float idist; // Incremental update distance
    setResourceSight(RESOURCE_ID, false);

    // See how far we can see on the grid
    int iter_max = resSightSkillCheck(RESOURCE_ID);
    iter_max = max(iter_max, 1); // Always able to see next to us

    // First check the trivial solution that the actor is standing on
    // the resource already
    actGrid[ind].getGridResVal(RESOURCE_ID, &resAtGrid);
    if (resAtGrid > 0)
    {
        setResourcePos(RESOURCE_ID, 0, 0); // Relative to the actor
        setResourceSight(RESOURCE_ID, true);
        return;
    }

    // Look for nearest food and return it in the pointer integers
    for (int i = (-1 * iter_max); i < iter_max; i++)
    {
        for (int j = (-1 * iter_max); j < iter_max; j++)
        {
            // Index we want to look at
            ind_n = ny * (getGridY() + i) + (getGridX() + j);

            if (!(ind_n >= 0) | !(ind_n < (nx * ny)))
            {
                // Index is out of grid, continue
                continue;
            }

            // If this isn't the index we are in and there is food at the index, mark it
            actGrid[ind].getGridResVal(RESOURCE_ID, &resAtGrid);
            if (resAtGrid> 0)
            {
                // Calculate the euclidean distance
                idist = eucl_dist_grid(i, j, 0);

                // If it's closer or we couldn't see food before,
                // update position and food dustabce
                getResourceSight(RESOURCE_ID, &canSeeResource);
                getResourceDist(RESOURCE_ID, &jdist);
                if ((idist < jdist) | (!canSeeResource))
                {
                    setResourcePos(RESOURCE_ID, j, i);
                    setResourceSight(RESOURCE_ID, true);
                }
            } // Grid has food 
        } // j loop
    } // i loop
    return;
}

int needsResourceDescriptor(int RESOURCE_ID, int *resDiscInt)
{
    // Map from 
    switch (RESOURCE_ID)
    {
        // Food cases
        case RES_FOOD_ANIMAL:
        case RES_FOOD_PLANT:
            *resDiscInt = IS_HUNGRY;
            return 1;
    }
    return 0;
}


int Actor::needResource(int RESOURCE_ID, bool *needResFlag)
{
    /*
    Determine if the actor needs the resource
    */
   int res1, res2, res3; 
   // Declarations for basic resources that can be combined tonight
    switch (RESOURCE_ID)
    {
        // Check for both foods
        case RES_FOOD_ANIMAL:
        case RES_FOOD_PLANT:
            getInvVal(RES_FOOD_ANIMAL, &res1);
            getInvVal(RES_FOOD_PLANT, &res2);
            *needResFlag = (res1 + res2) <= 5;
            return SUCCESS;
    }
    return ERROR_BAD_SWITCH_RESOURCE;
}

int Actor::doResourceActions(Grid *actGrid, int RESOURCE_ID)
{
    
    bool needResFlag;
    int resDiscInt;
    int error_code;
    int xToMove, yToMove;
    bool canSeeResource;

    // Do we need this resource?
    if (needResource(RESOURCE_ID, &needResFlag) != SUCCESS)
    {
        return ERROR_BAD_SWITCH_RESOURCE;
    }

    // We do need this resource
    if (needResFlag)
    {
        // Track that actor needed this resource
        error_code = needsResourceDescriptor(RESOURCE_ID, &resDiscInt);
        if (error_code == SUCCESS)
        {
        addHistInt(resDiscInt);
        }

        // Look for the resource
        lookForResource(actGrid, RESOURCE_ID);

           
        
        getResourceSight(RESOURCE_ID, &canSeeResource);
        // Can not see the resource
        if (!canSeeResource)
        {
            
            // Construct random movement
            xToMove = mRandMove();
            yToMove = mRandMove();

            // Do the movement
            moveOnGrid(xToMove, yToMove);
            return SUCCESS;

        }
        // We can see the resource, how far to go?
        getResourcePos(RESOURCE_ID, &xToMove, &yToMove);        

        // Check if we need to move and do so
        if ((xToMove != 0) | (yToMove != 0))
        {
            moveOnGrid(xToMove, yToMove);
            return SUCCESS;
        }

        // If the above logic is false then we must be standing on it
        int amountGathered;
        resourceGatherSkillCheck(RESOURCE_ID, &amountGathered) ;
        if (amountGathered > 0)
        {
        gatherResourceGrid(actGrid, RESOURCE_ID, amountGathered);
        }



    }
    return SUCCESS_NO_ACTION;

}


int Actor::doExploreActions(Grid *actGrid)
{
    addHistInt(IS_EXPLORING);
    int x, y;
    x = mRandMove();
    y = mRandMove();
    moveOnGrid(x, y);
    return 1;
}

int Actor::takeActions(Grid *actGrid, int actionId)
{
    addHistInt(actionId);
    int success;
    // Step through actions in the action list
    switch (actionId)
    {
        case DO_WATER_ACTION:
            success = doResourceActions(actGrid, RES_WATER);
            return 1;
        case DO_FOOD_ANIMAL_ACTION:
            success = doResourceActions(actGrid, RES_FOOD_ANIMAL);
            return 1;
        case DO_FOOD_PLANT_ACTION:
            success = doResourceActions(actGrid, RES_FOOD_PLANT);
            return 1;
        case DO_EXPLORE_ACTION:
        case DO_BATTLE_ACTION:
        case DO_SOCIAL_ACTION:
        case DO_WEALTH_ACTION:
            return doExploreActions(actGrid);
        case DO_LAZY_ACTION:
            return 1;

    }
    return 0;
}

// TODO: Make a priority creation function here
int Actor::getPriorityAction()
{
    int water, foodAnimal, foodPlant, foodTotal;
    getInvVal(RES_WATER, &water);
    getInvVal(RES_FOOD_ANIMAL, &foodAnimal);
    getInvVal(RES_FOOD_PLANT, &foodPlant);
    foodTotal = foodAnimal + foodPlant;
    bool hungry = foodTotal <= 5;

    // Water actions
    if (water <= 5)
    {
        return DO_WATER_ACTION;
    }

    // Food actions
    if (hungry & getResourceSightOut(RES_FOOD_ANIMAL))
    {
        return DO_FOOD_ANIMAL_ACTION;
    }
    else if (hungry & getResourceSightOut(RES_FOOD_PLANT))
    {
        return DO_FOOD_PLANT_ACTION;
    }
    else if (hungry)
    {
        // Randomly pick one to look for
        return rand()%2==1 ? DO_FOOD_PLANT_ACTION : DO_FOOD_ANIMAL_ACTION;
    }

    // TODO FEAR ACTION

    // Interests last
    const int n = 5;
    int interests[n] = {INTEREST_WEALTH, INTEREST_SOCIAL,\
                        INTEREST_BATTLE, INTEREST_EXPLORE,\
                        INTEREST_LAZY};
    int actions[n] = {DO_WEALTH_ACTION, DO_SOCIAL_ACTION,\
                      DO_BATTLE_ACTION, DO_EXPLORE_ACTION,\
                      DO_LAZY_ACTION};
    int interestRnd = rand()%getInterestValOut(INTEREST_TOTAL);
    int curInterestWeight;
    
    // Get the action to take
    for (int i = 0; i < n; i++)
    {
        getInterestVal(interests[i], &curInterestWeight);
        if(interestRnd < curInterestWeight)
        {
            return actions[i];
        }
        interestRnd -= curInterestWeight;
    }
    return DO_LAZY_ACTION;

}

void Actor::incrementTime(Grid *actGrid)
{
    // Reset the number of actions
    setActions(getMaxActions());

    // Clear out old history
    resetHist();

    // Make array of actions switch identifiers that can be taken
    // int actionSwitchId = 0;
    // int actionPriority[] = {DO_FOOD_ANIMAL_ACTION, DO_FOOD_PLANT_ACTION, DO_EXPLORE_ACTION, DO_LAZY_ACTION};
    // int actionListSize = sizeof(actionPriority) / sizeof(actionPriority[0]);
    int tookAction, actionTaken;
    // While we have actions to take, take actions
    while (getActions() > 0)
    {
        // Run the current action list
        actionTaken = getPriorityAction();
        addHistInt(actionTaken);
        tookAction = takeActions(actGrid, actionTaken);

        addActions(-1 * tookAction);

        // // Increment the action switch id
        // actionSwitchId++;
        
        // // Action switch id reset if it increased beyond the limit
        // if (actionSwitchId >= actionListSize)
        // {
        //     actionSwitchId = 0;
        // }
    }
}
//
//
// ACTOR
//
//