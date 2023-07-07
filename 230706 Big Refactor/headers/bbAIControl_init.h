/**
 * @file
 *
 * @ingroup 
 * @brief Code for updating AI is stored in ./source/AI. There is one file per unit type. This file includes all of those files and initialises a vtable containing the functions 
 */

#define BBAICONTROL_INIT


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


//declare table of ai constructors
//declare table of ai functions
//declare table of RPCs


extern int (*bbAI_constructor_vtable[NUMBER_OF_EXTERNAL_STATES])(bbMapCoords mc);
extern int (*bbAI_update_vtable[NUMBER_OF_EXTERNAL_STATES])(bbAIControl* aicontroller);
extern int (*bbAI_RPC_vtable[NUMBER_OF_EXTERNAL_STATES])(bbAIControl* aicontroller, bbMessage* message);


//initialise table of ai constructors
//initialise table of ai functions
//initialise table of RPCs
int bbAIControl_init();






