/**
 * @file
 *
 * @ingroup 
 * @brief Input key presses and mouse clicks
 */

#define INPUT


//-----------------------------INCLUDES----------------------------//


#ifndef PROMPT
#define PROMPT
#include "../headers/prompt.h"
#endif

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif


#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif


//-----------------------------GLOBALS----------------------------//

/** Remove this line and include window.h */
extern sfRenderWindow* window; //usually found in window.h


//-----------------------------FUNCTIONS--------------------------//

/** Get raw input */
int input_get(void);
