#define VIEWPORT

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

#ifndef CONSTANTS
    #include "constants.h"
#endif

#ifndef MEDIA
    #include "media.h"
#endif

#ifndef WINDOW
    #include "window.h"
#endif

//-----------------------------CONSTANTS----------------------------//

//Constants may be found in constants.h

//-----------------------------STRUCURES------------------------------//



//-----------------------------GLOBALS----------------------------//

sfRenderTexture* viewport;
sfTexture* viewport_texture;
sfSprite* viewport_sprite;



sfRenderTexture* viewport_background;
sfTexture* viewport_background_texture;
sfSprite* viewport_background_sprite;


sfRenderTexture* viewport_ground;
sfTexture* viewport_ground_texture;
sfSprite* viewport_ground_sprite;


sfRenderTexture* viewport_drawable;
sfTexture* viewport_drawable_texture;
sfSprite* viewport_drawable_sprite;


sfRenderTexture* viewport_highlight;
sfTexture* viewport_highlight_texture;
sfSprite* viewport_highlight_sprite;


sfRenderTexture* viewport_healthbars;
sfTexture* viewport_healthbars_texture;
sfSprite* viewport_healthbars_sprite;


sfRenderStates ViewportRenderer; 
sfShader* ViewportShader;

//-----------------------------FUNCTIONS--------------------------//

int Create_Viewport_Shaders (void);

int Init_Viewport(void){

    
    sfVector2f position;
    position.x = VIEWPORT_LEFT;
    position.y = VIEWPORT_TOP;

    viewport = sfRenderTexture_create(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, sfFalse);
    viewport_texture = sfRenderTexture_getTexture(viewport);
    viewport_sprite = sfSprite_create();
    sfSprite_setTexture(viewport_sprite, viewport_texture, sfTrue);
    
    sfSprite_setPosition(viewport_sprite, position);
    
    sfRenderTexture_clear(viewport, sfMagenta);
    sfRenderTexture_display(viewport);
    
    

    viewport_background = sfRenderTexture_create(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, sfFalse);
    viewport_background_texture = sfRenderTexture_getTexture(viewport_background);
    viewport_background_sprite = sfSprite_create();
    sfSprite_setTexture(viewport_background_sprite, viewport_background_texture, sfTrue);
    
    sfSprite_setPosition(viewport_background_sprite, position);
    
    sfRenderTexture_clear(viewport_background, sfBlue);
    sfRenderTexture_display(viewport_background);
    
    

    viewport_ground = sfRenderTexture_create(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, sfFalse);
    viewport_ground_texture = sfRenderTexture_getTexture(viewport_ground);
    viewport_ground_sprite = sfSprite_create();
    sfSprite_setTexture(viewport_ground_sprite, viewport_ground_texture, sfTrue);
    
    sfSprite_setPosition(viewport_ground_sprite, position);
    
    sfRenderTexture_clear(viewport_ground, sfTransparent);
    sfRenderTexture_display(viewport_ground);
    
    

    viewport_drawable = sfRenderTexture_create(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, sfFalse);
    viewport_drawable_texture = sfRenderTexture_getTexture(viewport_drawable);
    viewport_drawable_sprite = sfSprite_create();
    sfSprite_setTexture(viewport_drawable_sprite, viewport_drawable_texture, sfTrue);
    
    sfSprite_setPosition(viewport_drawable_sprite, position);
    
    sfRenderTexture_clear(viewport_drawable, sfTransparent);
    sfRenderTexture_display(viewport_drawable);
    
    

    viewport_highlight = sfRenderTexture_create(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, sfFalse);
    viewport_highlight_texture = sfRenderTexture_getTexture(viewport_highlight);
    viewport_highlight_sprite = sfSprite_create();
    sfSprite_setTexture(viewport_highlight_sprite, viewport_highlight_texture, sfTrue);
    
    sfSprite_setPosition(viewport_highlight_sprite, position);
    
    sfRenderTexture_clear(viewport_highlight, sfTransparent);
    sfRenderTexture_display(viewport_highlight);
    
    

    viewport_healthbars = sfRenderTexture_create(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, sfFalse);
    viewport_healthbars_texture = sfRenderTexture_getTexture(viewport_healthbars);
    viewport_healthbars_sprite = sfSprite_create();
    sfSprite_setTexture(viewport_healthbars_sprite, viewport_healthbars_texture, sfTrue);
    
    sfSprite_setPosition(viewport_healthbars_sprite, position);
    
    sfRenderTexture_clear(viewport_healthbars, sfTransparent);
    sfRenderTexture_display(viewport_healthbars);
    
    Create_Viewport_Shaders();

}

int Create_Viewport_Shaders (void){
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

    
    ViewportShader = sfShader_createFromMemory(vertShader, NULL, fragShader);
    assert (ViewportShader != NULL);
    
    ViewportRenderer.shader = ViewportShader;
    ViewportRenderer.blendMode = sfBlendAlpha;
    ViewportRenderer.transform = sfTransform_Identity;
    ViewportRenderer.texture = Textures[12];
//Textures[12] is any texture with dimensions VIEWPORT_WIDTH, VIEWPORT_HEIGHT


    sfShader_setTextureUniform(ViewportShader,"Background", viewport_background_texture);
    sfShader_setTextureUniform(ViewportShader,"Ground", viewport_ground_texture);
    sfShader_setTextureUniform(ViewportShader,"Drawables", viewport_drawable_texture);
    sfShader_setTextureUniform(ViewportShader,"Highlights", viewport_highlight_texture);
    sfShader_setTextureUniform(ViewportShader,"Healthbars", viewport_healthbars_texture);

}



