/**
 * @file
 *
 * @ingroup 
 * @brief Change the location of a drawable. Add to the appropriate terrain square
 */


#define BBDRAWABLE_LISTS
//-----------------------------INCLUDES----------------------------//

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif


#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

#ifndef BBDRAWABLE
#define BBDRAWABLE
#include "../headers/bbDrawable.h"
#endif

#ifndef MEDIA
#define MEDIA
#include "../headers/media.h"
#endif

#ifndef BBMESSAGE_FUNCTIONS
#define BBMESSAGE_FUNCTIONS
#include "../headers/bbMessage_functions.h"
#endif


#ifndef BBTERRAINSQUARE
#define BBTERRAINSQUARE
#include "../headers/bbTerrainSquare.h"
#endif


//-----------------------------CODE------------------------------//

/** Change the location of drawable and add to list in appropriate terrain square */
int bbDrawable_setLocation(int drawable_int, bbMapCoords mc);

/** When creating a new drawable, use this to add to the appropriate terrain square */
int bbDrawable_addtoTS (int drawable_int);


