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

/** an ai controller is a function that gets run once per loop, updating drawables in the game */
typedef struct {
 
    int Pool_Self;
    int Pool_Prev;
    int Pool_Next;
    int Pool_In_Use;
    
    int Square_Prev;
    int Square_Next;

    bbSquareCoords SquareCoords;
    //bbMapCoords location;
      //use location of first drawable?
      //consider bbMessage_move_drawable
      //should be safe since bbAI controller is the only thing that calls bbMessage_move_drawable

    int drawables[DRAWABLES_PER_AI];
    
    //does the AI update when not on screen?

    int external_state; //what AI function/state/action function to call
    int internal_state; //parameter of function/state/action function 
    
    
    int clock; //Counts the number of loop iterations since last reset
    int clock2; //Specifies a point in future given by the number of loops processed since loading the game.
    
    //for the following two variables, refer to first drawable. is this really what we want?
    //int target_drawable;
    //bbMapCoords target_location;
    
    //union of structs containing ai data?
    
    int state_transition; //Set to 1 if the state has just been altered, otherwise 0


} bbAIControl;



//TODO I wish this didnt have to go here
#ifndef BBAICONTROL_INIT
#define BBAICONTROL_INIT
#include "../headers/bbAIControl_init.h"
#endif
//-----------------------------CODE------------------------------//

DEFINE_POOL(bbAIControl, AICONTROLLER_POOL_LEVEL_1, AICONTROLLER_POOL_LEVEL_2);

extern int (*bbAI_update_vtable[NUMBER_OF_EXTERNAL_STATES])(bbAIControl* aicontroller);


int bbAIControl_update(bbAIControl* aicontroller){
    
    int type;
    
    int rethunk = RETHUNK;
    
    //#ifdef DEBUG  
    //printf("About to update an AI object\n");
    //#endif
    
    
    while (rethunk == RETHUNK){
    
        type = 0;aicontroller->external_state;
        
    //#ifdef DEBUG  
    //printf("type = %d\n", type);
    //#endif      
        
        rethunk = bbAI_update_vtable[type](aicontroller);
    }

    return 0;
}

int bbAIControl_update_int(int aicontroller_int){

    bbAIControl* aicontroller = bbAIControl_Pool_Lookup(aicontroller_int);
    
    int type;
    
    int rethunk = RETHUNK;
    
    while (rethunk == RETHUNK){
    
        type = aicontroller->external_state;
        rethunk = bbAI_update_vtable[AI_NULL](aicontroller);
    }

    return 0;
}

int bbAIControl_updatePool(void){

    int tobeupdated_int = bbAIControl_Pool_In_Use_Head;
    
    if ( tobeupdated_int == -1) return 0;
    

    
    
    while (tobeupdated_int != -1){
    
        //#ifdef DEBUG  
        //printf("location of AI in vtable = %d\n", tobeupdated_int);
        //#endif  
    
        bbAIControl* tobeupdated = bbAIControl_Pool_Lookup(tobeupdated_int);
        bbAIControl_update(tobeupdated);
        tobeupdated_int = tobeupdated->Pool_Next;
        
    }
    return 0;
}
