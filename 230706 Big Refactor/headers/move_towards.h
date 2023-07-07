/**
 * @file
 *
 * @ingroup 
 * @brief Moves a map coords ubject towards a goal point with no local avoidance
 */


#define MOVE_TOWARDS
//-----------------------------INCLUDES----------------------------//


#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

/** Moves a map coords ubject towards a goal point with no local avoidance */
int bbMapCoords_move_toward(bbMapCoords* subject, bbMapCoords object, float distance);

