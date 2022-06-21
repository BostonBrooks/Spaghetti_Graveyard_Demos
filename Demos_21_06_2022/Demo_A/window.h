#define WINDOW

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

//-----------------------------CONSTANTS----------------------------//

//Constants may be found in constants.h

//-----------------------------STRUCURES------------------------------//

//-----------------------------GLOBALS----------------------------//

sfRenderWindow* window;


//-----------------------------FUNCTIONS--------------------------//

int Init_Window(void){

    sfVideoMode mode = {WINDOW_WIDTH, WINDOW_HEIGHT, 32};
    
    window = sfRenderWindow_create(mode, "Maths RPG", sfResize | sfClose, NULL);
    
    //assert(window != 0);
    

}


int Display_Decal(void){

    sfRenderWindow_drawSprite(window, Sprites[27],NULL);

}
