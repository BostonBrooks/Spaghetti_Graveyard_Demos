/**
 * @file
 *
 * @ingroup 
 * @brief The ground surface is the thing units walk around on.
 * A curved suface is approximated using vertex arrays and hill shading.
 */

#define BBGROUNDSURFACE

//-----------------------------INCLUDES----------------------------//

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

#ifndef WINDOW
#define WINDOW
#include "../headers/window.h"
#endif

#ifndef INTERPOLATION
#define INTERPOLATION
#include "../headers/interpolation.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

#ifndef MEDIA
#define MEDIA
#include "../headers/media.h"
#endif



#ifndef VIEWPORT
#define VIEWPORT
#include "../headers/viewport.h"
#endif

//-----------------------------CONSTANTS----------------------------//

//-----------------------------STRUCTS------------------------------//

typedef struct {
    int i_Coord; //what terrain square are we looking at?
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
    
    sfRenderTexture*  Circles_Render_Texture;
    sfTexture*        Circles_Texture;
    
    sfVertexArray*    Vertex_Array;
} bbGroundSurface;



//-----------------------------GLOBALS----------------------------//


extern bbGroundSurface bbGroundSurface_grid[SQUARES_PER_MAP][SQUARES_PER_MAP];

/** Used for temporary storage of hill shading data, not thread safe!!! */
extern sfUint8 Hill_Shading_Data[PIXELS_PER_SQUARE * PIXELS_PER_SQUARE * 4];

extern sfRenderStates GroundRenderer; 
extern sfShader* GroundShader;

/** All of the textures being rendered to the ground surface have to be sfRenderTextures,
 * or the result will be flipped. Thanks SFML Team!
*/
extern sfRenderTexture* null_render_texture;
extern sfTexture* null_texture;

//-----------------------------FUNCTIONS--------------------------//

/** Initialise vertex array for one map square */
int bbGroundSurface_initVertexArray(int square_i, int square_j);

/** Initialise hill shading for one map square */
int bbGroundSurface_calcHillShading(int Square_i, int Square_j);

/** Initialise all textures, hill shading and vetex arrays */
int bbGroundSurface_initAll(void);

/** Create shaders used to draw to the ground surface */
int Create_Ground_Shaders (void);

/** Draw one sqaure to the screen */
int bbGroundSurface_draw(int Square_i, int Square_j);

/** Draw all sqaures visible on the screen */
int bbGroundSurface_drawVisible();
