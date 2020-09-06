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
    const int nx = 100;
    const int ny = 100;
    Grid myGrid[nx * ny];
    default_random_engine generator;
    normal_distribution<float> distribution(0.0, 1.0);

    int flInd;
    int ifood;
    const siv::PerlinNoise perlin(10876);
    double frequency = 8.0;
    int octaves = 8;
    const double fx = nx / frequency;
    const double fy = ny / frequency;

    // Loop through grid and generate all of the grid information
    for (int i = 0; i < ny; i++)
    {
        for (int j = 0; j < nx; j++)
        {
            flInd = (i * ny) + j;            
            // ifood = (int) distribution(generator);
            // ifood = clip(ifood, 0, 9);
            ifood = rand()%15;
            myGrid[flInd].food = ifood;
            myGrid[flInd].height = (int) perlin.accumulatedOctaveNoise2D_0_1(j / fx, i / fy, octaves);
            myGrid[flInd].move_cost = rand() % 2 + 1;
        }
    }
    // Build the actor, test out movement
    Actor myactor(25, 25, nx, ny, 10, 10, 10, 10, 10, 10, 0, 1);    
    historyFile << "X " << myactor.getGridX() << "| Y " << myactor.getGridY() << endl;
    myactor.moveOnGrid(1, 1);
    historyFile << "X " << myactor.getGridX() << "| Y " << myactor.getGridY() << endl;

    // Loop over days till actor dies
    int n_days = 0;
    while (myactor.getAlive() & (n_days <= 40))
    {
        // Loop over time steps in days
        for (int i = 0; i < 2; i++){
            historyFile << "Actor " << myactor.getUUID() << " incrementing time" << endl;
            myactor.incrementTime(myGrid);
            historyFile << "Sees food: " << myactor.getSightFood() << endl;
        }

        // Increment the day
        myactor.incrementDay();
        historyFile << "End of day " << n_days;
        historyFile << " | Food "   << myactor.getFood();
        historyFile << " | Health " << myactor.getHealth();
        historyFile << " | Alive "  << myactor.getAlive();
        historyFile << " | Grid pos (x,y) : (" << myactor.getGridX() << "," << myactor.getGridY() << ")";        
        historyFile << endl;
        n_days++;
    }
    historyFile.close();
    
    return 0;
}