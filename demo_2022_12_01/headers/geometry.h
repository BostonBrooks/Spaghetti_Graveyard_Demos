#define GEOMETRY

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

typedef float matrix[4][4];

//-----------------------------GLOBALS------------------------------//

extern int bbElevations[HEIGHT_MAP_SIZE][HEIGHT_MAP_SIZE];
//all of the vertices of map tiles plus a 1 tile buffer.


//Bicubic Interpolation Coefficients.
//extern float bicubic_coefficients[4][4];

//extern int bicubic_coefficients_tile_i;
//extern int bicubic_coefficients_tile_j;

extern bbMapCoords viewpoint;

extern bbFloat3D LightIncedence;


//-----------------------------FUNCTIONS------------------------------//

int bbElevations_load();

int bbTileCoords_getElevation(int, int);

bbFloat3D bbFloat3D_normalise (bbFloat3D);

bbTileCoords bbMapCoords_getTileCoords (bbMapCoords);

bbScreenCoords bbMapCoords_getScreenCoords_centre (bbMapCoords);

bbScreenCoords bbGetVertex(int tile_i, int tile_j, int square_i, int square_j);

bbMapCoords bbScreenCoords_getMapCoords (bbScreenCoords point);

bbScreenCoords bbTileCoords_getScreenCoords_centre (bbTileCoords TC);

bbSquareCoords bbMapCoords_getSquareCoords (bbMapCoords map);

int floordiv (int a , int b);

bbMapCoords bbScreenCoords_getMapCoords_k0 (bbScreenCoords sc);

int bbMapCoords_compare (bbMapCoords a, bbMapCoords b); 
//return 1 if a is closer to the screen than b, 0 otherwise
