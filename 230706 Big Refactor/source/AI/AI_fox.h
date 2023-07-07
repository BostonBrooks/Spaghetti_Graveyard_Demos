
#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

#ifndef BBAICONTROL
#define BBAICONTROL
#include "../headers/bbAIControl.h"
#endif

#ifndef BBMESSAGE
#define BBMESSAGE
#include "../headers/bbMessage.h"
#endif

#ifndef BBDRAWABLE
#define BBDRAWABLE
#include "../headers/bbDrawable.h"
#endif

int AI_fox_new(bbMapCoords mc){

    //#ifdef DEBUG  
    //printf("Creating a null AI object\n");
    //#endif
    
    int drawable_int = bbDrawable_new(mc);
    bbDrawable* drawable = bbDrawable_Pool_Lookup(drawable_int);
    
    
    // change drawable animation 
    drawable->animation[0] = 4;
    drawable->angle[0]     = 0;
    drawable->frame[0]     = 0;
    
    
    //#ifdef DEBUG  
    //printf("New drawable is at the location %d\n", drawable_int );
    //#endif
    
    int aicontroller_int = bbAIControl_Pool_New(NEXT_AVAILABLE);
    
    //#ifdef DEBUG  
    //printf("New AI Controller is at the location %d\n", aicontroller_int );
    //#endif
    
    
    bbAIControl* aicontroller =  bbAIControl_Pool_Lookup(aicontroller_int);
    
    
    aicontroller->SquareCoords = bbMapCoords_getSquareCoords(mc);
    

    aicontroller->drawables[0] = drawable_int;
    
    for(int i = 1; i<DRAWABLES_PER_AI; i++){
        aicontroller->drawables[i] = -1;
    
    }

    aicontroller->external_state  = AI_NULL;
    aicontroller->internal_state  = 0;
    
    aicontroller->clock           = 0;
    aicontroller->clock2          = 0;
    
    //aicontroller->state_transition = 0; //TODO why is this line problematic?
    
    return aicontroller_int;
}

int AI_fox_update(bbAIControl* aicontroller){

    //#ifdef DEBUG  
    //printf("Updating a null AI object\n");
    //#endif

    //set position of drawable to viewpoint
    drawable_int = aicontroller->drawables[0];
    bbDrawable* drawable = bbDrawable_Pool_Lookup(drawable_int);
    
    
    message_movement_new(drawable_int, viewpoint);
    
    

    return NO_RETHUNK;

}

int AI_fox_RPC (bbAIControl* aicontroller, bbMessage* message){

    return 0; //what does the return value signify?
}
