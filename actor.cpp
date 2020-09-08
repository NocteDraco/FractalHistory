#include <iostream> 
#include <cstdlib>
#include "actor.hpp"
#include "skills.hpp"

using namespace std;

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
    // A bunch of checks about the state of the actor

    // Food at end of day
    if (getFood() <= 0){
        setStarving(true);
    } else{
        addFood(-1);
    }

    // Starving but alive
    if (getStarving() & getAlive()){
        addHealth(-1);
    }

    // Dying
    if ((getHealth() <= 10) & getAlive()){
        setDying(true);
    }

    // Dead
    if (getHealth() <= 0)
    {
        setDying(false);
        setStarving(false);
        setAlive(false);

    }

}


int Actor::getTotalWeight()
{
    return food + water;
}

bool Actor::encumbered()
{
    return getTotalWeight() > getCarry();
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
        case RES_FOOD:
        case RES_WATER:
        case RES_FOOD_MEAT:
        case RES_FOOD_BERRY:
        case RES_FOOD_FRUIT:
        case RES_FOOD_GRAIN:
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
        case RES_FOOD:
        case RES_FOOD_MEAT:
            *amountGathered = doSkillCheck(SURVIVAL)/inefficientFactor;

        case RES_FOOD_BERRY:
        case RES_FOOD_FRUIT:
        case RES_FOOD_GRAIN:
        case RES_WATER:
            *amountGathered = doSkillCheck(NATURE)/inefficientFactor;


        
    }
   
}

int checkGridResource(Grid *actGrid, int ind, int RESOURCE_ID)
{
    // Return resource count at grid index ind, otherwise return 0
    switch (RESOURCE_ID)
    {
        case RES_FOOD:
        case RES_FOOD_MEAT:
        case RES_FOOD_BERRY:
        case RES_FOOD_FRUIT:
        case RES_FOOD_GRAIN:
            return actGrid[ind].food;
        
        case RES_WATER:
            return actGrid[ind].water;
    }
    return 0;
}

int Actor::grabFood(Grid *actGrid, int amount)
{
    
    // Current grid index of actor
    int ind = ny * getGridY() + getGridX();

    // Make sure there is food on the grid
    if (actGrid[ind].food > 0)
    {
        // How much can we grab from the terrain?
        int canGrab = min(amount, actGrid[ind].food);

        // How much can we hold in inventory?
        int doGrab = min(canGrab, carry - getTotalWeight());

        // Make sure to remove negatives
        doGrab = max(doGrab, 0);

        // We are actually grabbing food
        if (doGrab > 0)
        {
            // Add to inventory
            addHistInt(GRABBING_FOOD);
            food += doGrab;

            // Remove grabbed food from grid.
            actGrid[ind].addFood(-1 * doGrab);
        }
        return 1; // Grabbed food
    }
    return 0; // Unable to grab food
    
}

void Actor::gatherResourceGrid(Grid *actGrid, int RESOURCE_ID, int amount)
{
    switch (RESOURCE_ID)
    {
        case RES_FOOD:
        case RES_FOOD_MEAT:
        case RES_FOOD_BERRY:
        case RES_FOOD_FRUIT:
        case RES_FOOD_GRAIN:
            grabFood(actGrid, amount);
            setResourceSight(RESOURCE_ID, false);

    }
    
}

void Actor::lookForResource(Grid *actGrid, int RESOURCE_ID)
{
    // Current grid index of actor
    int ind = ny * getGridY() + getGridX();
    int ind_n;
    bool canSeeResource;

    float jdist; // Distance to resource
    float idist; // Incremental update distance
    setResourceSight(RESOURCE_ID, false);

    // See how far we can see on the grid
    int iter_max = resSightSkillCheck(RESOURCE_ID);
    iter_max = max(iter_max, 1); // Always able to see next to us

    // First check the trivial solution that the actor is standing on
    // the resource already
    if (checkGridResource(actGrid, ind, RESOURCE_ID) > 0)
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
            if (checkGridResource(actGrid, ind_n, RESOURCE_ID) > 0)
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

// void Actor::lookForFood(Grid *actGrid)
// {
//     // Current grid index of actor
//     int ind = ny * getGridY() + getGridX();
//     int ind_n;

//     float idist; // Incremental update distance
//     setSightFood(false);

//     int iter_max = profSkillCheck(getWis(), getPro(), 1) / 4; // How far can this actor look
//     iter_max = max(iter_max, 1); // Always able to see next to us

//     // Look for nearest food and return it in the pointer integers
//     for (int i = (-1 * iter_max); i < iter_max; i++)
//     {
//         for (int j = (-1 * iter_max); j < iter_max; j++)
//         {
//             // Index we want to look at
//             ind_n = ny * (getGridY() + i) + (getGridX() + j);

//             // Valid index checking
//             if ((ind_n >= 0) & (ind_n < (nx * ny)))
//             {
//                 // If this isn't the index we are in and there is food at the index, mark it
//                 if (actGrid[ind_n].food > 0)
//                 {
//                     // Calculate the euclidean distance
//                     idist = pow(pow(i,2) + pow(j,2),0.5);

//                     // If it's closer or we couldn't see food before,
//                     // update position and food dustabce
//                     if ((idist < getDistFood()) | (!getSightFood()))
//                     {
//                         setPosFood(j, i);
//                         setSightFood(true);
//                     }
//                 } // Grid has food 
//             } // Valid index 
//         } // j loop
//     } // i loop
//     return;
// }



int needsResourceDescriptor(int RESOURCE_ID, int *resDiscInt)
{
    // Map from 
    switch (RESOURCE_ID)
    {
        // Food cases
        case RES_FOOD:
        case RES_FOOD_MEAT:
        case RES_FOOD_BERRY:
        case RES_FOOD_FRUIT:
        case RES_FOOD_GRAIN:
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
    switch (RESOURCE_ID)
    {
        case RES_FOOD:
        case RES_FOOD_MEAT:
        case RES_FOOD_BERRY:
        case RES_FOOD_FRUIT:
        case RES_FOOD_GRAIN:
            *needResFlag = food <= 5;
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
            cout << xToMove << " " << yToMove << endl;
            moveOnGrid(xToMove, yToMove);
            return SUCCESS;
        }

        // If the above logic is false then we must be standing on it
        int amountGathered;
        resourceGatherSkillCheck(RESOURCE_ID, &amountGathered) ;
        cout << amountGathered << endl;
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
    cout << "Exploring " << x << " " << y << endl;
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
        case DO_FOOD_ACTION:
            success = doResourceActions(actGrid, RES_FOOD);
            return 1;
        case DO_EXPLORE_ACTION:
            return doExploreActions(actGrid);
        case DO_LAZY_ACTION:
            return 1;

    }
    return 0;
}

// TODO: Make a priority creation function here

void Actor::incrementTime(Grid *actGrid)
{
    // Reset the number of actions
    setActions(getMaxActions());

    // Clear out old history
    resetHist();

    // Make array of actions switch identifiers that can be taken
    int actionSwitchId = 0;
    int actionPriority[] = {DO_FOOD_ACTION, DO_FOOD_ACTION, DO_EXPLORE_ACTION, DO_LAZY_ACTION};
    int actionListSize = sizeof(actionPriority) / sizeof(actionPriority[0]);

    // While we have actions to take, take actions
    while (getActions() > 0)
    {
        // Run the current action list
        int tookAction = takeActions(actGrid, actionPriority[actionSwitchId]);

        addActions(-1 * tookAction);

        // Increment the action switch id
        actionSwitchId++;
        
        // Action switch id reset if it increased beyond the limit
        if (actionSwitchId >= actionListSize)
        {
            actionSwitchId = 0;
        }
    }
}
