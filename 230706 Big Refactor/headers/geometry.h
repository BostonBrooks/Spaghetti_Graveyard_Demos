/**
 * @file
 *
 * @ingroup 
 * @brief This header file contains structures that represent different dimensions of geometric data.
 * It also includes geometric transformations between types of data.
 */

#define GEOMETRY


//-----------------------------INCLUDES----------------------------//

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif



//-----------------------------STRUCTS------------------------------//


typedef struct {
    float x;         //x axis is the number of pixels to the right of top left
    float y;         //y axis is down number of pixels down from top left
} bbScreenCoords;

typedef struct {
    int i;           //i axis points down and to the right, in units of points
    int j;           //j axis points up and to the right, in units of points
    int k;           //k axis points up, in units of points, negative indicates invalid
} bbMapCoords;

typedef struct {
    int i;           //i axis points down and to the right, in units of tiles
    int j;           //j axis points up and to the right, in units of tiles
    int k;           //k axis points up, in units of points, negative indicates invalid
} bbTileCoords;

typedef struct {
    int i;           //i axis points down and to the right, in units of tiles
    int j;           //j axis points up and to the right, in units of tiles
    int k;           //k axis points up, in units of points, negative indicates invalid
} bbSquareCoords;


typedef struct {
    float i;           //i axis points down and to the right, in whatever units
    float j;           //j axis points up and to the right, in whatever units
    float k;           //k axis points up, whatever units
} bbFloat3D;


typedef struct {       //used to store the hit box of a drawable relative to the drawable's location on screen.

    float top;
    float left;
    float height;
    float width;
    

} bbFloatRect;



//-----------------------------GLOBALS------------------------------//

/** Stores the elevation data within the game map.
 * There is a padding on this array of size HEIGHT_MAP_PADDING,
 * used to look up height values that are just outside the actual map
 */
extern int bbElevations[HEIGHT_MAP_SIZE][HEIGHT_MAP_SIZE];

/**  The players view screen is centred on this point
 */
extern bbMapCoords viewpoint;

/** A vector representing the angle that light comes down from the sky.
 */
extern bbFloat3D LightIncedence;

//-----------------------------FUNCTIONS------------------------------//

/** Multiply 4 by 4 matrices A and B and store in matrix C.
 * We used this a lot when we were previously using bicubic interpolation to calculate hill shading
 */
int matrix_multiply (float A[4][4], float B[4][4], float C[4][4]);


/** Print a 4 by 4 matrix to the screen*/
int print_matrix(float A[4][4]);

/** print a screen coords struct to the screen */
int bbScreenCoords_print(bbScreenCoords sc);

/** print a map coords struct to the screen */
int bbMapCoords_print(bbMapCoords mc);

/** Load map elevations from a file */
int bbElevations_load();

/** Return a vector parallel to foo, but with absolute value = 1 */
bbFloat3D bbFloat3D_normalise (bbFloat3D foo) ;

/** Look up the elevation of a point on the map given by tile coordinates */
int bbTileCoords_getElevation(int i, int j);

/** Divide a by b and round towards negative infinity */
int floordiv (int a , int b);

/** Find the coordinates of the tile containing the point given in map coordinates */
bbTileCoords bbMapCoords_getTileCoords (bbMapCoords map);

/** Find the coordinates of the square containing the point given in map coordinates */
bbSquareCoords bbMapCoords_getSquareCoords (bbMapCoords map);

/** Find the screen coordinates of a point if (0,0,0) is in the top left corner of the screen */
bbScreenCoords bbMapCoords_getScreenCoords_topleft (bbMapCoords mc);

/** Find the screen coordinates of a point if (0,0,0) is in the centre of the screen */
bbScreenCoords bbMapCoords_getScreenCoords_centre (bbMapCoords mc);

/** Find the screen coordinates of a point, in tile coordinates, if (0,0,0) is in the centre of the screen */
bbScreenCoords bbTileCoords_getScreenCoords_centre (bbTileCoords TC);

/** Find the screen coords given the tile coords in an odd format,
 * relative to the top left of the screen.
 * Used to generate vertices for plotting curved ground surfaces
 */
bbScreenCoords bbGetVertex(int tile_i, int tile_j, int square_i, int square_j);

/** Find the map coords of a point on screen (in fact, on the viewport), given k = 0 */
bbMapCoords bbScreenCoords_getMapCoords_k0 (bbScreenCoords sc);

/** Find the map coords of a point on screen (in fact, on the viewport), given k is at it's maximum value */
bbMapCoords bbScreenCoords_getMapCoords_kmax (bbScreenCoords sc);


/** Find the value of k, given i and j are map coordinates */
int bbElevations_interpolate ( int i, int j);

/** Find the value of k and update the map coordinates struct */
int bbMapCoords_updateElevation(bbMapCoords* map_coords);

/** Calculates the signed are of a triangle on screen, positive if the vertices are listed in clockwise order */
float signed_area (bbScreenCoords, bbScreenCoords, bbScreenCoords);

/** determines if a point on screen is within a triangle given by vertices in screen coordinates, 1 for true, 0 for false */
int bbScreenCoords_withinTriangle_screen (bbScreenCoords point, bbScreenCoords vertex1, bbScreenCoords vertex2, bbScreenCoords vertex3);

/** determines if a point on screen is within a triangle given by vertices in map coordinates, 1 for true, 0 for false */
int bbScreenCoords_withinTriangle_map (bbScreenCoords p, bbMapCoords vertex1, bbMapCoords vertex2, bbMapCoords vertex3);

/** given 3 points in 3 dimensional space, define a plane. Return the coordinates of a point given by screen coordinates projected onto plane. */
bbMapCoords bbScreenCoords_interpolateMapCoords (bbScreenCoords p, bbMapCoords vertex1, bbMapCoords vertex2, bbMapCoords vertex3);

/** Is the point on the left (1) or right (0) side of the tile containg the point */
int bbMapCoords_withinTile_LR (bbMapCoords point);

/** project a screen coordinate onto curved ground surface */
bbMapCoords bbScreenCoords_getMapCoords (bbScreenCoords point);

/** Which coordinate is closer to the screen. 1 if a is closer, 0 if b is closer */
int bbMapCoords_isCloser(bbMapCoords a, bbMapCoords b);

/** calculate the distance between 2 points, on the i, j plane */
float bbMapCoords_getDistance(bbMapCoords a, bbMapCoords b);

