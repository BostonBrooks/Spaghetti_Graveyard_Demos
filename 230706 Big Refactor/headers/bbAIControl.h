/**
 * @file
 *
 * @ingroup 
 * @brief An ai controller is a function that gets run once per loop, updating drawables in the game
          there are two types of ai function
 */

#define BBAICONTROL

//-----------------------------INCLUDES----------------------------//


#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

#ifndef POOLS
#define POOLS
#include "../headers/pools.h"
#endif


//-----------------------------STRUCTS------------------------------//

/** An ai controller is a function that gets run once per loop, updating drawables in the game
          there are two types of ai function
          an action is one that immediately changes to an outer state after being thunk
          an outer state is one that persists until conditions change */
          
typedef struct {
 
    int Pool_Self;
    int Pool_Prev;
    int Pool_Next;
    int Pool_In_Use;
    
    int Square_Prev;
    int Square_Next;

    bbSquareCoords SquareCoords;
    bbMapCoords location; //or use location of first drawable?

    int drawables[DRAWABLES_PER_AI];
    
    //does the AI update when not on screen?

    /** What AI function/state/action function to call */
    int external_state; 
    
    /** Parameter of function/state/action function */
    int internal_state; 
    
    /** Counts the number of loop iterations since last reset */
    int clock;
    
    /**Specifies a point in future given by the number of loops processed since loading the game.*/
    int clock2; 
    
    //for the following two variables, refer to first drawable. is this really what we want?
      //int target_drawable;
      //bbMapCoords target_location;
    
    //union of structs containing ai data?
    
    /**Set to 1 if the state has just been altered, otherwise 0 */
    int state_transition;


} bbAIControl;

//-----------------------------CODE------------------------------//

DECLARE_POOL_GLOBALS(bbAIControl, AICONTROLLER_POOL_LEVEL_1, AICONTROLLER_POOL_LEVEL_2);

/** Update individual AI */
int bbAIControl_update(bbAIControl* aicontroller);

/** Update individual AI given its location in pool */
int bbAIControl_update_int(int aicontroller_int);

/** Update all AIs in pool */
int bbAIControl_updatePool(void);
