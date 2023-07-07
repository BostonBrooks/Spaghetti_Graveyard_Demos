/**
 * @file
 *
 * @ingroup 
 * @brief A message is a short bit of code, sent by an AI controller,
 * that is acted on an object at a latyer time, after all AI Controllers have completed proccessing.
 * Care should be taken to ensure these messages are associative (order doesnt matter)
 */
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


extern int bbMessage_head;
extern int bbMessage_tail;
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
DECLARE_POOL_GLOBALS(bbMessage, MESSAGE_POOL_LEVEL_1, MESSAGE_POOL_LEVEL_2)

/** Which message is defered to process at a later time. */
int bbMessage_isAfter (bbMessage* a, bbMessage* b);
