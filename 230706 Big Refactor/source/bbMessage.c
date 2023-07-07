#define BBMESSAGE
//-----------------------------INCLUDES----------------------------//

#ifndef POOLS
#define POOLS
#include "../headers/pools.h"
#endif

#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

//-----------------------------GLOBALS------------------------------//


int bbMessage_head = -1;
int bbMessage_tail = -1;
//for the time being, we dont care what
// terrain square the message was sent to

//-----------------------------STRUCTS------------------------------//
typedef struct {
 
    int Pool_Self;
    int Pool_Prev;
    int Pool_Next;
    int Pool_In_Use;
    
    int Sorted_Prev;
    int Sorted_Next;

    int Message_Time;
    
    bbMapCoords location;
    
    int type;
    
    int subject;

    char message_string[16];
    
    bbMapCoords mapcoords;

} bbMessage;


//-----------------------------CODE------------------------------//
DEFINE_POOL(bbMessage, MESSAGE_POOL_LEVEL_1, MESSAGE_POOL_LEVEL_2)


int bbMessage_isAfter (bbMessage* a, bbMessage* b){ 
//return 1 if a is after b, 0 otherwise
    int time_a = a->Message_Time;
    int time_b = b->Message_Time;
    
    if (time_a > time_b) return 1;
    if (time_a == time_b) return 0;
    return -1;
}
