#define TERRAINSQUARES

//-----------------------------INCLUDES----------------------------//

/*
#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

*/

//#ifndef CONSTANTS
//    #include "contants.h"
//#endif

#ifndef GROUNDSURFACE
    #include "groundsurface.h"
#endif



//-----------------------------CONSTANTS----------------------------//

//Constants may be found in constants.h


//-----------------------------STRUCURES------------------------------//

typedef struct {
    XY_Point Location;        //location of Terrain Square on map.
                     // Ie leftmost square is at (0,0),
                     // the first TS down and to the right is at (1,0)
                     //the first TS up and to the right is at (0,1)



//    int ElevationData[TILES_PER_SQUARE+3][TILES_PER_SQUARE+3];
                     //made up of SUBDIVISIONS*SUBDIVISIONS grid of upper and lower triangles.
                     //Meaning a SUBDIVISIONS+1*SUBDIVISIONS+1 grid of vertices.
                     //Plus a 1 vertex buffer of vertices used to calculate terrain shaddows.
                     
                     
                     
    //sfTexture* GroundSurface;
   // sfImage* CPULightMap;
   // sfUint8* LightMapPtr;
   // sfTexture* GPULightMap;
   // sfVertexArray* GPU_Elevations;
    int Drawable_Head;
    int Drawable_Tail;

} TerrainSquare;

//-----------------------------GLOBALS----------------------------//

TerrainSquare TerrainSquares[SQUARES_PER_MAP][SQUARES_PER_MAP];
