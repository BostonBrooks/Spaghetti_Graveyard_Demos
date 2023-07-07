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

sfRenderTexture* viewport; //deprecated/dummy texture
sfTexture* viewport_texture;
sfSprite* viewport_sprite;



sfRenderTexture* bbViewport_background;
sfTexture* bbViewport_background_texture;
sfSprite* bbViewport_background_sprite;


sfRenderTexture* bbViewport_ground;
sfTexture* bbViewport_ground_texture;
sfSprite* bbViewport_ground_sprite;


sfRenderTexture* bbViewport_main;
sfTexture* bbViewport_main_texture;
sfSprite* bbViewport_main_sprite;


sfRenderTexture* bbViewport_highlights;
sfTexture* bbViewport_highlights_texture;
sfSprite* bbViewport_highlights_sprite;


sfRenderTexture* bbViewport_healthbars;
sfTexture* bbViewport_healthbars_texture;
sfSprite* bbViewport_healthbars_sprite;


sfRenderStates bbViewport_renderer; 
sfShader* bbViewport_shader;

//-----------------------------FUNCTIONS--------------------------//

int bbViewport_shaders_create (void);

int bbViewport_init(void){

    
    sfVector2f position;
    position.x = VIEWPORT_LEFT;
    position.y = VIEWPORT_TOP;

    viewport = sfRenderTexture_create(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, sfFalse);
    viewport_texture = sfRenderTexture_getTexture(viewport);
    viewport_sprite = sfSprite_create();
    sfSprite_setTexture(viewport_sprite, viewport_texture, sfTrue);
    
    sfSprite_setPosition(viewport_sprite, position);
    
    sfRenderTexture_clear(viewport, COLOUR_VIEWPORT_NULL);
    sfRenderTexture_display(viewport);
    
    

    bbViewport_background = sfRenderTexture_create(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, sfFalse);
    bbViewport_background_texture = sfRenderTexture_getTexture(bbViewport_background);
    bbViewport_background_sprite = sfSprite_create();
    sfSprite_setTexture(bbViewport_background_sprite, bbViewport_background_texture, sfTrue);
    
    sfSprite_setPosition(bbViewport_background_sprite, position);
    
    sfRenderTexture_clear(bbViewport_background, COLOUR_BACKGROUND_NULL);
    sfRenderTexture_display(bbViewport_background);
    
    

    bbViewport_ground = sfRenderTexture_create(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, sfFalse);
    bbViewport_ground_texture = sfRenderTexture_getTexture(bbViewport_ground);
    bbViewport_ground_sprite = sfSprite_create();
    sfSprite_setTexture(bbViewport_ground_sprite, bbViewport_ground_texture, sfTrue);
    
    sfSprite_setPosition(bbViewport_ground_sprite, position);
    
    sfRenderTexture_clear(bbViewport_ground, sfTransparent);
    sfRenderTexture_display(bbViewport_ground);
    
    

    bbViewport_main = sfRenderTexture_create(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, sfFalse);
    bbViewport_main_texture = sfRenderTexture_getTexture(bbViewport_main);
    bbViewport_main_sprite = sfSprite_create();
    sfSprite_setTexture(bbViewport_main_sprite, bbViewport_main_texture, sfTrue);
    
    sfSprite_setPosition(bbViewport_main_sprite, position);
    
    sfRenderTexture_clear(bbViewport_main, sfTransparent);
    sfRenderTexture_display(bbViewport_main);
    
    

    bbViewport_highlights = sfRenderTexture_create(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, sfFalse);
    bbViewport_highlights_texture = sfRenderTexture_getTexture(bbViewport_highlights);
    bbViewport_highlights_sprite = sfSprite_create();
    sfSprite_setTexture(bbViewport_highlights_sprite, bbViewport_highlights_texture, sfTrue);
    
    sfSprite_setPosition(bbViewport_highlights_sprite, position);
    
    sfRenderTexture_clear(bbViewport_highlights, sfTransparent);
    sfRenderTexture_display(bbViewport_highlights);
    
    

    bbViewport_healthbars = sfRenderTexture_create(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, sfFalse);
    bbViewport_healthbars_texture = sfRenderTexture_getTexture(bbViewport_healthbars);
    bbViewport_healthbars_sprite = sfSprite_create();
    sfSprite_setTexture(bbViewport_healthbars_sprite, bbViewport_healthbars_texture, sfTrue);
    
    sfSprite_setPosition(bbViewport_healthbars_sprite, position);
    
    sfRenderTexture_clear(bbViewport_healthbars, sfTransparent);
    sfRenderTexture_display(bbViewport_healthbars);
    
    bbViewport_shaders_create();

}

int bbViewport_shaders_create (void){
//vertex_shader
    char vertShader[] = "\
        uniform vec2 offset;\
        void main()\
        {\
         \
            vec4 offsetvertex = vec4(offset.x, offset.y, 0, 0);\
            gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + offsetvertex);\
         \
            gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\
         \
           gl_FrontColor = gl_Color;\
        }\
    ";
    
    //Fragment Shader
    char fragShader[] = "\
        uniform sampler2D Background;\
        uniform sampler2D Ground;\
        uniform sampler2D Drawables;\
        uniform sampler2D Highlights;\
        uniform sampler2D Healthbars;\
        \
        void main()\
        {\
            vec4 background = texture2D(Background, gl_TexCoord[0].xy);\
            vec4 ground = texture2D(Ground, gl_TexCoord[0].xy);\
            vec4 drawables = texture2D(Drawables, gl_TexCoord[0].xy);\
            vec4 highlights = texture2D(Highlights, gl_TexCoord[0].xy);\
            vec4 healthbars = texture2D(Healthbars, gl_TexCoord[0].xy);\
        \
            vec4 mix1 = mix(background, ground, ground.a);\
            vec4 mix2 = mix(mix1, drawables, drawables.a);\
            vec4 mix3 = mix(mix2, highlights, highlights.a);\
            vec4 mix4 = mix(mix3, healthbars, healthbars.a);\
            gl_FragColor =  mix4;\
            \
        }\
    ";

    
    bbViewport_shader = sfShader_createFromMemory(vertShader, NULL, fragShader);
    assert (bbViewport_shader != NULL);
    
    bbViewport_renderer.shader = bbViewport_shader;
    bbViewport_renderer.blendMode = sfBlendAlpha;
    bbViewport_renderer.transform = sfTransform_Identity;
    bbViewport_renderer.texture = sfTexture_vtable[12];
//Textures[12] is any texture with dimensions VIEWPORT_WIDTH, VIEWPORT_HEIGHT


    sfShader_setTextureUniform(bbViewport_shader,"Background", bbViewport_background_texture);
    sfShader_setTextureUniform(bbViewport_shader,"Ground", bbViewport_ground_texture);
    sfShader_setTextureUniform(bbViewport_shader,"Drawables", bbViewport_main_texture);
    sfShader_setTextureUniform(bbViewport_shader,"Highlights", bbViewport_highlights_texture);
    sfShader_setTextureUniform(bbViewport_shader,"Healthbars", bbViewport_healthbars_texture);

}



