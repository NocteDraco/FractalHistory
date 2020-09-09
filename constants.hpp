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
static const int DO_FOOD_ANIMAL_ACTION  = 10;
static const int DO_FOOD_PLANT_ACTION   = 17;
static const int DO_WATER_ACTION        = 11;
static const int DO_SLEEP_ACTION        = 14;
static const int DO_REPRODUCE_ACTION    = 15;
static const int DO_RECOVER_ACTION      = 17;
static const int DO_LAZY_ACTION         = 99;

// INTERESTS
static const int DO_WEALTH_ACTION       = 20;
static const int DO_BATTLE_ACTION       = 13;
static const int DO_EXPLORE_ACTION      = 12;
static const int DO_SOCIAL_ACTION       = 16;

// Grid information
static const int LAND_ID_BASE           = 9000;
static const int LAND_ID_OWN            = 9100;
static const int LAND_ID_FERTILITY      = 9200;
static const int LAND_ID_HEIGHT         = 9300;

// LAND BASE TYPE
static const int LAND_GROUND            = 9001; // NORMAL GROUND
static const int LAND_WATER             = 9002; // WATER
static const int LAND_ARABLE            = 9003; // CAN FARM IT
static const int LAND_PATH              = 9004; // FASTER WALKING
static const int LAND_NOT_FOOT          = 9006; // GROUND BUT CAN'T TRAVEL VIA FOOT

// LAND OWNERSHIP
static const int LAND_UNCLAIMED         = 9199;
static const int LAND_OWNED_COM_RACE    = 9101;
static const int LAND_OWNED_MON_RACE    = 9102;
static const int LAND_OWNED_MONSTER     = 9103;

// RESOURCES
static const int RES_WATER              = 101;
static const int RES_AIR                = 102;
static const int RES_SHELTER            = 103;
static const int RES_PATH               = 104;
static const int RES_GROUND             = 106;
static const int RES_METAL              = 107;
static const int RES_WOOD               = 108;
static const int RES_MAGIC              = 199;
static const int RES_COIN               = 200; 
// What is more valuable than magic you say? why coin! but only by a pinch

// FOOD RESOURCE SPECIFICS
static const int RES_FOOD_ANIMAL        = 300;
static const int RES_FOOD_PLANT         = 301;
static const int RES_FOOD               = 302;

// 
//
// STATS
//
//
static const int STR                    = 500;
static const int DEX                    = 501;
static const int CON                    = 502;
static const int INT                    = 503;
static const int WIS                    = 504;
static const int CHA                    = 505;
// 
//
// STATS
//
//

// 
//
// STATES
//
//
static const int STATE_ALIVE            = 700;
static const int STATE_STARVING         = 701;
static const int STATE_DEHYDRATED       = 702;
static const int STATE_HUNGRY           = 703;
static const int STATE_PARCHED          = 704;
// 
//
// STATES
//
//


//
//
// SKILL CHECKS
//
//
static const int ATHLETICS              = 600;
// DEXTERITY
static const int ACROBATICS             = 601;
static const int SLEIGHT_OF_HAND        = 602;
static const int STEALTH                = 603;
// INTELLIGENCE
static const int ARCANA                 = 604;
static const int HISTORY                = 605;
static const int INVESTIGATION          = 606;
static const int NATURE                 = 607;
static const int RELIGION               = 608;
// WISDOM
static const int ANIMAL_HANDLING        = 609;
static const int INSIGHT                = 610;
static const int MEDICINE               = 611;
static const int PERCEPTION             = 612;
static const int SURVIVAL               = 613;
// CHARISMA
static const int DECEPTION              = 614;
static const int INTIMIDATION           = 615;
static const int PERFORMANCE            = 616;
static const int PERSUASION             = 617;
//
//
// SKILL CHECKS
//
//

//
//
// Interests
//
//
static const int INTEREST_WEALTH        = 801;
static const int INTEREST_SOCIAL        = 802;
static const int INTEREST_BATTLE        = 803;
static const int INTEREST_EXPLORE       = 804;
static const int INTEREST_LAZY          = 888;
static const int INTEREST_TOTAL         = 899;
//
//
// Interests
//
//


// PARAMETERS 
static const int MAX_HISTORY            = 100; // Maximum amount of history stored per increment

// INITIALIZATION PARAMETERS 
static const float FOOD_MEAN            = 0.0;
static const float FOOD_VAR             = 0.5;
static const int GRID_SIZE_X            = 250;
static const int GRID_SIZE_Y            = 250;
static const float NOISE_LAND_FREQUENCY = 8.0;
static const int NOISE_LAND_OCTAVES     = 8;
static const bool SAVE_HISTORY_INTS     = false;


//
//
// ERRORS
//
//
static const int SUCCESS                    = 1;
static const int SUCCESS_NO_ACTION          = 2;
static const int ERROR_BAD_SWITCH_RESOURCE  = -10;
static const int ERROR_BAD_SWITCH_ACTION    = -11;



#endif