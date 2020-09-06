#include <cstdlib>
#include "actor.hpp"
#include "grid.hpp"
#include "skills.hpp"


void Actor::moveOnGrid(int gridXStep, int gridYStep)
{
    int newGridX = gridX + gridXStep;
    int newGridY = gridY + gridYStep;
    setGrid(newGridX, newGridY);
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
    return getFood() + getWater();
}

bool Actor::encumbered()
{
    return getTotalWeight() > getCarry();
}

void getNearestFood(Grid *actGrid, 
                    int nx, 
                    int ny, 
                    int x,
                    int y,
                    int *yF, 
                    int *xF, 
                    int perception)
{
    
        

}


bool Actor::lookForFood(Grid *actGrid, int *x, int *y, float *dist)
{
    // Current grid index of actor
    int ind = ny * getGridY() + getGridX();
    *dist = 999.0; // Distance
    float idist;
    bool canSeeFood = false;

    int iter_max = profSkillCheck(getWis(), getPro(), 1) / 4; // How far can this actor look

    // Already standing on the point, don't try to move
    if (actGrid[ind].food > 0)
    {
        *dist = 0;
        *x = 0;
        *y = 0;
        canSeeFood = true;
        return canSeeFood;
    }

    // Look for nearest food and return it in the pointer integers
    for (int i = (-1 * iter_max); i < iter_max; i++)
    {
        for (int j = (-1 * iter_max); j < iter_max; j++)
        {
            // Index we want to look at
            int ind_n = ny * (getGridY() + i) + (getGridX() + j);

            // Valid index checking
            if ((ind_n > 0) & (ind_n < (nx * ny)))
            {
                // If this isn't the index we are in and there is food at the index, mark it
                if (actGrid[ind_n].food > 0)
                {
                    // Calculate the euclidean distance
                    idist = pow(pow(i,2) + pow(j,2),0.5);
                    if (idist < *dist)
                    {
                        // Update values
                        *dist = idist;
                        *x = j;
                        *y = i;
                        canSeeFood = true;
                    }
                }
            }
        }
    }

    return canSeeFood;
}

void Actor::checkGrid(Grid *actGrid)
{
    // If not encumbered, attempt to pick up non essential things
    if (!encumbered())
    {
        
    }

}

void Actor::grabFood(Grid *actGrid)
{
    // Current grid index of actor
    int ind = ny * getGridY() + getGridX();

    if (actGrid[ind].food > 0)
    {
        addFood(actGrid[ind].food);
        actGrid[ind].setFood(0);
    }
}

void Actor::incrementTime(Grid *actGrid)
{
    // Increment time forward

    // Hungry
    if (getFood() <= 5)
    {
        // Look for food nearby
        int x_food, y_food;
        float distFood = 999.0;
        setSightFood(lookForFood(actGrid, &x_food, &y_food, &distFood));

        // We see it but aren't standing on it
        if (getSightFood() & (abs(x_food) > 0) & (abs(y_food) > 0))
        {
            addGridX(x_food);
            addGridY(y_food);
        }
        // Standing on food
        else if (getSightFood())
        {
            grabFood(actGrid);
        }

    }
}

void Actor::determineActivity()
{

    // Plan movement
        // Hungry
        // Reproduce
        // Tired
        // 
    
}