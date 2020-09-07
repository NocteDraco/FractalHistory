// Create a grid

// Create an actor

// Place actor on the grid

// Have actor move to new location on the grid

// Print resulting location of the actor on the grid

// Do not let actor exit grid
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <random>

#include "constants.hpp"
#include "actor.hpp"
#include "array2d.h"
#include "grid.hpp"
#include "PerlineNoise.hpp"


using namespace std;

int main() {
    // Create and open the file
    ofstream historyFile;
    historyFile.open ("history.log", ios::out);

    // Build the grid
    const int nx = GRID_SIZE_X;
    const int ny = GRID_SIZE_Y;
    Grid myGrid[nx * ny];
    default_random_engine generator;
    normal_distribution<float> distribution(FOOD_MEAN, FOOD_VAR);

    int flInd;
    int ifood;
    const siv::PerlinNoise perlin(10876);
    double frequency = NOISE_LAND_FREQUENCY;
    int octaves = NOISE_LAND_OCTAVES;
    const double fx = nx / frequency;
    const double fy = ny / frequency;

    // Loop through grid and generate all of the grid information
    cout << "Constructing grid of existence" << endl;
    for (int i = 0; i < ny; i++)
    {
        for (int j = 0; j < nx; j++)
        {
            flInd = (i * ny) + j;            
            ifood = static_cast<int> (distribution(generator));
            ifood = max(ifood, 0);
            ifood = min(ifood, 9);
            myGrid[flInd].food = ifood;
            myGrid[flInd].height = static_cast<int> (perlin.accumulatedOctaveNoise2D_0_1(j / fx, i / fy, octaves));
            myGrid[flInd].move_cost = rand() % 2 + 1;
        }
    }
    // Build the actor, test out movement
    Actor myactor(25, 25, nx, ny, 10, 10, 10, 10, 10, 10, 0, 1);    

    // Loop over days till actor dies
    int n_days = 0;
    int MAX_DAYS = 40;
    int n_inc_per_day = 2;
    int i_hist_file;
    cout << "Running sim for " << MAX_DAYS << endl;
    while (myactor.getAlive() & (n_days <= MAX_DAYS))
    {
        // Loop over time steps in days
        for (int i = 0; i < n_inc_per_day; i++){
            myactor.incrementTime(myGrid);
        }

        if (SAVE_HISTORY_INTS)
        {
            // Loop over all the actions and track their actions to the file
            historyFile << myactor.getUUID() << " ";
            for (unsigned int i = 0; i < MAX_HISTORY; i++)
            {
                i_hist_file = myactor.getHistAtInd(i);
                if (i_hist_file == 0)
                {
                    break;
                }
                else
                {
                    historyFile << i_hist_file << " ";
                }
            }
            historyFile << endl;
        }// SAVE_HISTORY_INTS


        // Increment the day
        myactor.incrementDay();
        historyFile << "End of day " << n_days;
        historyFile << " | Food "   << myactor.getFood();
        historyFile << " | Health " << myactor.getHealth();
        historyFile << " | Grid pos (x,y) : (" << myactor.getGridX() << "," << myactor.getGridY() << ")";        
        historyFile << endl;
        n_days++;
    }
    historyFile.close();
    
    return 0;
}