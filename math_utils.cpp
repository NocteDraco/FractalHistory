#include <algorithm>
#include "math_utils.hpp"

// Distance in euclidean space from origin (0,0,0)
float eucl_dist_grid(int x, int y, int z)
{
    return pow(pow(x,2) + pow(y,2) + pow(z,2),0.5);
}

int mRandMove()
{
    // Random positive/negative direction movement
    return rand()%2 == 1 ? -1 : 1;
}