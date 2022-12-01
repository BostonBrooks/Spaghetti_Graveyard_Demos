#define VIEWPORT

//-----------------------------INCLUDES----------------------------//
#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

//-----------------------------CONSTANTS----------------------------//

//Constants may be found in constants.h

//-----------------------------STRUCURES------------------------------//



//-----------------------------GLOBALS----------------------------//

extern sfRenderTexture* viewport;
extern sfTexture* viewport_texture;
extern sfSprite* viewport_sprite;
//deprecated


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

int bbViewport_shaders_create (void);

int bbViewport_init(void);



