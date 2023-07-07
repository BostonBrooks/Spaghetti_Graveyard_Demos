/**
 * @file
 *
 * @ingroup 
 * @brief Types of message and what their effects are.
 */

#define BBMESSAGES_FUNCTIONS


//-----------------------------INCLUDES----------------------------//

#ifndef BBMESSAGE
#define BBMESSAGE
#include "../headers/bbMessage.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

//-----------------------------GLOBALS----------------------------//

/** vtable containing message types */
extern int (*bbMessage_type_vtable[NUMBER_OF_MESSAGE_TYPES])(bbMessage* message);


//-----------------------------CODE----------------------------//


//probably will not call this function directly:
/** when this message is processed, it changes the location of a drawable */
int bbMessage_move_drawable(bbMessage* message);

/** populate vtable of message types */
int bbMessage_initTypes();
