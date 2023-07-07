#define BBMESSAGE_FUNCTIONS


//-----------------------------INCLUDES----------------------------//

#ifndef BBMESSAGE
#define BBMESSAGE
#include "../headers/bbMessage.h"
#endif


#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

#ifndef BBDRAWABLE
#define BBDRAWABLE
#include "../headers/bbDrawable.h"
#endif


#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

#ifndef BBMESSAGE_PASSING
#define BBMESSAGE_PASSING
#include "../headers/bbMessage_passing.h"
#endif

//-----------------------------GLOBALS----------------------------//

extern int Current_Time; //TODO Move to appropriate header file.

int (*bbMessage_type_vtable[NUMBER_OF_MESSAGE_TYPES])(bbMessage* message);


//-----------------------------CODE----------------------------//


int bbMessage_move_drawable(bbMessage* message){

    bbDrawable_setLocation(message->subject, message->mapcoords);
    return DELETE_MESSAGE;


}


int bbMessage_initTypes(){

    bbMessage_type_vtable[MESSAGE_MOVE] = bbMessage_move_drawable;
    
    return 0;
}
