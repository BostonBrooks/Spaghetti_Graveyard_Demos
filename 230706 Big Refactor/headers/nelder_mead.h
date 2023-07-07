/**
 * @file
 *
 * @ingroup 
 * @brief This header file contains code to minimise the potential of a drawable, in order to calculate next location
 */

#define NELDER_MEAD

//-----------------------------INCLUDES----------------------------//


#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

//-----------------------------CODE----------------------------//

/** minimise the potential of a drawable, in order to calculate next location */
bbMapCoords nelder_mead (int drawable_int);
