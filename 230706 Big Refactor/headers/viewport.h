/**
 * @file
 *
 * @ingroup 
 * @brief Contains shaders and textures for drawing to the viewport
 */
 
 
 #define VIEWPORT

//-----------------------------INCLUDES----------------------------//
#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

#ifndef MEDIA
#define MEDIA
    #include "../headers/media.h"
#endif

#ifndef WINDOW
#define WINDOW
    #include "../headers/window.h"
#endif

//-----------------------------CONSTANTS----------------------------//

//Constants may be found in constants.h

//-----------------------------STRUCURES------------------------------//



//-----------------------------GLOBALS----------------------------//

extern sfRenderTexture* viewport; //deprecated/dummy texture
extern sfTexture* viewport_texture;
extern sfSprite* viewport_sprite;



extern sfRenderTexture* bbViewport_background;
extern sfTexture* bbViewport_background_texture;
extern sfSprite* bbViewport_background_sprite;


extern sfRenderTexture* bbViewport_ground;
extern sfTexture* bbViewport_ground_texture;
extern sfSprite* bbViewport_ground_sprite;


extern sfRenderTexture* bbViewport_main;
extern sfTexture* bbViewport_main_texture;
extern sfSprite* bbViewport_main_sprite;


extern sfRenderTexture* bbViewport_highlights;
extern sfTexture* bbViewport_highlights_texture;
extern sfSprite* bbViewport_highlights_sprite;


extern sfRenderTexture* bbViewport_healthbars;
extern sfTexture* bbViewport_healthbars_texture;
extern sfSprite* bbViewport_healthbars_sprite;


extern sfRenderStates bbViewport_renderer; 
extern sfShader* bbViewport_shader;

//-----------------------------FUNCTIONS--------------------------//

/** Create shaders for drawing drawables to vieport */
int bbViewport_shaders_create (void);

/** Create textures for drawing to the viewport */
int bbViewport_init(void);

