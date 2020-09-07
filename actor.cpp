#include <iostream> 
#include <cstdlib>
#include "constants.hpp"
#include "actor.hpp"
#include "grid.hpp"
#include "skills.hpp"
#include "math_utils.hpp"

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

void Actor::lookForFood(Grid *actGrid)
{
    // Current grid index of actor
    int ind = ny * getGridY() + getGridX();
    int ind_n;

    float idist; // Incremental update distance
    setSightFood(false);

    int iter_max = profSkillCheck(getWis(), getPro(), 1) / 4; // How far can this actor look
    iter_max = max(iter_max, 1); // Always able to see next to us

    // Look for nearest food and return it in the pointer integers
    for (int i = (-1 * iter_max); i < iter_max; i++)
    {
        for (int j = (-1 * iter_max); j < iter_max; j++)
        {
            // Index we want to look at
            ind_n = ny * (getGridY() + i) + (getGridX() + j);

            // Valid index checking
            if ((ind_n >= 0) & (ind_n < (nx * ny)))
            {
                // If this isn't the index we are in and there is food at the index, mark it
                if (actGrid[ind_n].food > 0)
                {
                    // Calculate the euclidean distance
                    idist = pow(pow(i,2) + pow(j,2),0.5);

                    // If it's closer or we couldn't see food before,
                    // update position and food dustabce
                    if ((idist < getDistFood()) | (!getSightFood()))
                    {
                        setPosFood(j, i, idist);
                        setSightFood(true);
                    }
                } // Grid has food 
            } // Valid index 
        } // j loop
    } // i loop
    return;
}


int Actor::grabFood(Grid *actGrid)
{
    // Current grid index of actor
    int ind = ny * getGridY() + getGridX();

    // Make sure there is food on the grid
    if (actGrid[ind].food > 0)
    {
        addHistInt(GRABBING_FOOD);
        // Give the actor the food
        int ifood = food + actGrid[ind].food;

        // Error check value
        ifood = max(ifood, 0);
        // Min b/w given food and total available weight left
        ifood = min(ifood, carry - getTotalWeight() + food);
        food = ifood;

        // Remove food from grid.
        actGrid[ind].setFood(0);

        // No longer see food
        setSightFood(false);
        return 1; // Grabbed food
    }
    return 0; // Unable to grab food
    
}

int mRandMove()
{
    // Random positive/negative direction movement
    return rand()%2 == 1 ? -1 : 1;
}

int Actor::doFoodActions(Grid *actGrid)
{
    // Hungry action
    if (food <= 5)
    {
        addHistInt(IS_HUNGRY);

        // If we're standing on food just grab it, doesn't require 
        // any looking
        if (grabFood(actGrid) != 0)
        {
            return 1;
        }

        // Looking for food and update the position to move to
        addHistInt(LOOKING_FOOD);
        lookForFood(actGrid);
 
        // Switch food movement
        int x = 0;
        int y = 0;
        int iNeedMoveFood = needMoveFood();
        addHistInt(iNeedMoveFood);
        switch (iNeedMoveFood)
        {
            case CANT_SEE_FOOD: // Move randomly (need to make this much better)
                x = mRandMove();
                y = mRandMove();
            case SEE_FOOD_CAN_MOVE: // Get movement toward it
                getMoveFood(&x, &y);
        } // Switch food movement

        // Move towards food
        moveOnGrid(x, y);
        return 1;

    } // Hungry action

    return 0; // No action taken
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
    // Step through actions in the action list
    switch (actionId)
    {
        case DO_FOOD_ACTION:
            return doFoodActions(actGrid);
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
