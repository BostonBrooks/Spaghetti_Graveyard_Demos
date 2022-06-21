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


//-----------------------------FUNCTIONS--------------------------//

int Init_Viewport(void){

    viewport = sfRenderTexture_create(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, sfFalse);
    viewport_texture = sfRenderTexture_getTexture(viewport);
    viewport_sprite = sfSprite_create();
    sfSprite_setTexture(viewport_sprite, viewport_texture, sfTrue);
    
    sfVector2f position;
    position.x = VIEWPORT_LEFT;
    position.y = VIEWPORT_TOP;
    
    sfSprite_setPosition(viewport_sprite, position);
    

}



