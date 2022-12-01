#define GROUNDSURFACE

//-----------------------------INCLUDES----------------------------//

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

//TODO window contains sfRender_Texture ViewPort;
#ifndef WINDOW
#define WINDOW
#include "../headers/window.h"
#endif

#ifndef INTERPOLATION
#define INTERPOLATION
#include "../headers/interpolation.h"
#endif


//-----------------------------CONSTANTS----------------------------//

#ifndef CONSTANTS
#include "./headers/constants.h"
#endif
/*
#define DEFAULT_COLOR_R 104
#define DEFAULT_COLOR_G 151
#define DEFAULT_COLOR_B 100
#define DEFAULT_COLOR_A 255
*/
//-----------------------------STRUCTS------------------------------//

typedef struct {
    int i_Coord;
    int j_Coord;
    
    sfRenderTexture*  Base_Render_Texture;
    sfTexture*        Base_Texture;
    
    
    sfImage*          Hill_Shading_CPU;
    sfUint8*          Hill_Shading_Data;
    sfRenderTexture*  Hill_Shading_Render_Texture;
    sfTexture*        Hill_Shading_Texture;
    
    sfRenderTexture*  Footprints_Render_Texture;
    sfTexture*        Footprints_Texture;
    
    sfRenderTexture*  Auras_Render_Texture;
    sfTexture*        Auras_Texture;
    
    sfVertexArray*    Vertex_Array;
} bbGroundSurface;



//-----------------------------GLOBALS----------------------------//

extern bbGroundSurface bbGroundSurface_grid[SQUARES_PER_MAP][SQUARES_PER_MAP];


extern sfRenderStates GroundRenderer; 
extern sfShader* GroundShader;

extern sfRenderTexture* null_render_texture;
extern sfTexture* null_texture;

//-----------------------------FUNCTIONS--------------------------//

int bbGroundSurface_initVertexArray(int square_i, int square_j);


int bbGroundSurface_calcHillShading(int Square_i, int Square_j);

int bbGroundSurface_initAll(void);

int Create_Ground_Shaders (void);


int bbGroundSurface_draw(int Square_i, int Square_j); 

int bbGroundSurface_drawVisible();
