#define BBMESSAGE_CONSTRUCTORS



//-----------------------------INCLUDES----------------------------//

#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif


#ifndef BBMESSAGE
#define BBMESSAGE
#include "../headers/bbMessage.h"
#endif

#ifndef BBMESSAGE_PASSING
#define BBMESSAGE_PASSING
#include "../headers/bbMessage_passing.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

#ifndef BBMESSAGE_FUNCTIONS
#define BBMESSAGE_FUNCTIONS
#include "../headers/bbMessage_functions.h"
#endif

//-----------------------------CODE------------------------------//

int message_movement_new(int drawable_int, bbMapCoords mc){

    int message_int = bbMessage_Pool_New(NEXT_AVAILABLE);
    bbMessage* message =  bbMessage_Pool_Lookup(message_int);
    message->Message_Time = 1;
    message->type = MESSAGE_MOVE;
    message->subject = drawable_int;
    message->mapcoords = mc;
    bbMessage_submit(message_int);
    
}
