/**
 * @file
 *
 * @ingroup 
 * @brief Drawables create peaks in a potential field.
 * AI controllers move drawables to minimise their potential.
 * drawable_A is repelled by drawable_B.
 */


#define POTENTIAL_FIELDS


//-----------------------------INCLUDES----------------------------//


#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

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

#ifndef BBTERRAINSQUARE
#define BBTERRAINSQUARE
#include "../headers/bbTerrainSquare.h"
#endif

//-----------------------------GLOBALS----------------------------//

/** Specific potential functions for differently shaped drawables */
extern float (*potential_shape_vtable[NUMBER_OF_AVOIDANCE_SHAPES])(int drawable_A_int, int drawable_B_int, bbMapCoords test_point);

//-----------------------------CODE----------------------------//

/** The basic shape of the potential field */
float sigmoid_untuned(float x);

/** The tuned shape of the potential field */
float sigmoid(float distance);

/** Specific potential functions for differently shaped drawables */

/** Specific potential function for circlular drawables */
float Circular_Potential (int drawable_A_int, int drawable_B_int, bbMapCoords test_point);

/** Specific potential function for square drawables */
float Square_Potential (int drawable_A_int, int drawable_B_int, bbMapCoords test_point);


/** Specific potential function for lines */
float Line_Potential (int drawable_A_int, int drawable_B_int, bbMapCoords test_point);

/** Specific potential function for rectangles */
float Rectangle_Potential (int drawable_A_int, int drawable_B_int, bbMapCoords test_point);


/** Specific potential function for polygons */
float Polygon_Potential (int drawable_A_int, int drawable_B_int, bbMapCoords test_point);

/** Initiate vtable containing Specific potential functions for differently shaped drawables */
int potential_avoidance_initShapes(void);

/** Add up the potentials TODO write words */
float sum_potential_per_square(int drawable_A_int, bbMapCoords test_point, int Square_i, int Square_j);

/** Return potential at at test point ignoring the variable being updated */
float sum_potential_Nearby(int drawable_A_int, bbMapCoords test_point);

/** Output a csv file for plotting the potential field. */
int plot_potential(int drawable_int);
