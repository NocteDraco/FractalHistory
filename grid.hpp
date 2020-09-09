#ifndef GRID_HPP
#define GRID_HPP
#include <map>
#include "constants.hpp"

class Grid{
private:
    typedef std::map <int, int> LandInfoMaps;
    LandInfoMaps myInfo;

public:
    Grid()
    {
        // Resources
        myInfo[RES_FOOD_ANIMAL]         = 0;
        myInfo[RES_FOOD_PLANT]          = 0;
        myInfo[RES_WATER]               = 0;
        myInfo[RES_AIR]                 = 0;
        myInfo[RES_METAL]               = 0;
        myInfo[RES_WOOD]                = 0;
        myInfo[RES_PATH]                = 0;
        myInfo[RES_GROUND]              = 0;
        myInfo[RES_MAGIC]               = 0;

        // Descriptors
        myInfo[LAND_ID_BASE]            = LAND_GROUND;
        myInfo[LAND_ID_OWN]             = LAND_UNCLAIMED;
        myInfo[LAND_ID_FERTILITY]       = 0;
        myInfo[LAND_ID_HEIGHT]          = 0;
    }
    int getGridResVal(int const&, int*);
    int setGridResVal(int const&, int);
    int addGridResVal(int const&, int); 
};

#endif //GRID_H