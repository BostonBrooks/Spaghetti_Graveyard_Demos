/**
 * @file
 *
 * @ingroup 
 * @brief Create messages and add to queue
 */

#define BBMESSAGE_CONSTRUCTORS



//-----------------------------INCLUDES----------------------------//

#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif


//-----------------------------CODE------------------------------//

/** move a drawable to a new location */
int message_movement_new(int drawable_int, bbMapCoords mc);
