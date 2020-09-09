#include <map>
#include "grid.hpp"
using namespace std;



int Grid::getGridResVal(int const& key, int *resVal)
{
    // Construct iterator to find it (make constant so we don't 
    // mess up data). See https://stackoverflow.com/questions/3113979/how-to-create-map-with-keys-values-inside-class-body-once-not-each-time-functio
    LandInfoMaps::const_iterator  iFind    = myInfo.find(key);
    if (iFind != myInfo.end())
    {
        // If we find it return the value.
        *resVal = iFind->second;
        return 1; //Success
    }
    return 0;
}
int Grid::setGridResVal(int const& key, int resVal)
{
    switch(key)
    {
        case LAND_ID_BASE:
            return 0; // Can't modify these values
    }
    // NOTE: Don't use const_iterator here since we need to modify
    // the value at it's location
    LandInfoMaps::iterator  iFind    = myInfo.find(key);
    
    // Only change the values that are in the map
    if (iFind != myInfo.end())
    {
        iFind->second = resVal;
        return 1; // Success
    }

    return 0; // Failed

}

int Grid::addGridResVal(int const& key, int resVal)
{
    /* 
    Same as the set operation but adding instead
    */

    // Values that can't be added to
    switch(key)
    {
        case LAND_ID_OWN:
        case LAND_ID_BASE:
            return 0; // Can't modify these values with add
    }
    // NOTE: Don't use const_iterator here since we need to modify
    // the value at it's location
    LandInfoMaps::iterator  iFind    = myInfo.find(key);
    
    // Only change the values that are in the map
    if (iFind != myInfo.end())
    {
        iFind->second += resVal;
        return 1; // Success
    }
    
    return 0; // Failed

}

