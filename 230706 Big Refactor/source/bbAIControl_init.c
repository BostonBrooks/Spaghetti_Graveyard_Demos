/**
 * @file
 *
 * @ingroup 
 * @brief Code for updating AI is stored in ./source/AI. There is one file per unit type. This file includes all of those files and initialises a vtable containing the functions 
 */

#define BBAICONTROL_INIT

//-----------------------------INCLUDES----------------------------//


#ifndef BBAICONTROL
#define BBAICONTROL
#include "../headers/bbAIControl.h"
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

//one #include per ai type

#include "../source/AI/AI_null.h"


//-----------------------------CODE------------------------------//

//define table of ai constructors
//define table of ai functions
//define table of RPCs

int (*bbAI_constructor_vtable[NUMBER_OF_EXTERNAL_STATES])(bbMapCoords mc);
int (*bbAI_update_vtable[NUMBER_OF_EXTERNAL_STATES])(bbAIControl* aicontroller);
int (*bbAI_RPC_vtable[NUMBER_OF_EXTERNAL_STATES])(bbAIControl* aicontroller, bbMessage* message);

int bbAIControl_init(){

//initialise table of ai constructors
//initialise table of ai functions
//initialise table of RPCs

bbAI_constructor_vtable[AI_NULL] = AI_null_new;
bbAI_update_vtable[AI_NULL] = AI_null_update;
bbAI_RPC_vtable[AI_NULL] = AI_null_RPC;


}



