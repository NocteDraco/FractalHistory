#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// FOOD
static const int CAN_SEE_FOOD           = 1001;
static const int CANT_SEE_FOOD          = 1002;
static const int SEE_FOOD_CAN_MOVE      = 1003;
static const int STANDING_ON_FOOD       = 1004;
static const int GRABBING_FOOD          = 1005;
static const int IS_STARVING            = 1006;
static const int NOT_HUNGRY             = 1007;
static const int IS_HUNGRY              = 1008;
static const int LOOKING_FOOD           = 1009;

// WATER
static const int NOT_THIRSTY            = 2000;
static const int IS_THIRSTY             = 2002;
static const int IS_PARCHED             = 2003;
static const int CAN_SEE_WATER          = 2004;
static const int CANT_SEE_WATER         = 2005;
static const int STANDING_NEXT_WATER    = 2006;
static const int GRABBING_WATER         = 2007;
static const int SEE_WATER_CAN_MOVE     = 2008;
static const int LOOKING_WATER          = 2009;

// EXPLORING/MOVING
static const int NOT_EXPLORING          = 3000;
static const int IS_EXPLORING           = 3001;
static const int SEE_INTERESTING        = 3002;
static const int IS_MOVING_FOOT         = 3003;
static const int IS_MOVING_AIR          = 3004;
static const int IS_MOVING_WATER        = 3005;
static const int IS_MOVING_UNDERGROUND  = 3006;
static const int BAD_MOVE_BORDER_X      = 3007;
static const int BAD_MOVE_BORDER_Y      = 3008;

// HEALTH
static const int TOOK_WOUNDS            = 4000;
static const int HEAL_WOUNDS            = 4001;
static const int LOSS_SKILL             = 4002;
static const int GAIN_SKILL             = 4003;
static const int IS_DYING               = 4004;
static const int IS_DEAD                = -1;


// ACTION SETS
static const int DO_FOOD_ACTION         = 10;
static const int DO_WATER_ACTION        = 11;
static const int DO_EXPLORE_ACTION      = 12;
static const int DO_FIGHT_ACTION        = 13;
static const int DO_SLEEP_ACTION        = 14;
static const int DO_REPRODUCE_ACTION    = 15;
static const int DO_SOCIAL_ACTION       = 16;
static const int DO_LAZY_ACTION         = 19;

// LAND
static const int LAND_GROUND            = 9000;
static const int LAND_WATER             = 9001;
static const int LAND_FARM              = 9002;
static const int LAND_PATH              = 9003;
static const int LAND_TRAVERSABLE       = 9004;
static const int LAND_NOT_TRAVERSABLE   = 9005;
static const int LAND_LIVING            = 9006;
static const int LAND_DEFENSE           = 9007;

// PARAMETERS 
static const int MAX_HISTORY            = 100; // Maximum amount of history stored per increment
static const float FOOD_MEAN            = 0.0;
static const float FOOD_VAR             = 0.5;
static const int GRID_SIZE_X            = 100;
static const int GRID_SIZE_Y            = 100;
static const float NOISE_LAND_FREQUENCY = 8.0;
static const int NOISE_LAND_OCTAVES     = 8;
static const bool SAVE_HISTORY_INTS     = false;


#endif