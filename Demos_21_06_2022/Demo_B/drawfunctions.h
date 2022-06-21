#define DRAWFUNCTIONS

//-----------------------------INCLUDES----------------------------//

#ifndef GEOMETRY
    #include "geometry.h"
#endif

#ifndef VIEWPORT
    #include "viewport.h"
#endif



//-----------------------------CONSTANTS----------------------------//

#ifndef CONSTANTS
    #include "constants.h"
#endif

//Constants may be found in constants.h

//-----------------------------STRUCURES------------------------------//


//-----------------------------GLOBALS----------------------------//

int (*draw_functions[NUMBER_OF_DRAW_FUNCTIONS])(int animation, int angle, int frame, Map_Coords Coords);

//-----------------------------FUNCTIONS--------------------------//

int lookup_sprite(int animation, int angle, int frame){

//lookup animation
    Animation* Anim = Animations[animation];
    
    int foo = frame + Anim->frames * angle;
    
    return Anim->sprites[foo];


}


int basic_draw(int animation, int angle, int frame, Map_Coords MC){



    //lookup sprite
    int foo = lookup_sprite(animation, angle, frame);
    sfSprite* Sprt = Sprites[foo];

    //convert to viewport coordinates

    Screen_Coords SC = map_to_screen_centre_viewpoint (MC);

    //draw to viewport
    sfVector2f position;
    position.x = SC.x;
    position.y = SC.y;
    
    
    sfSprite_setPosition(Sprt, position);
    
    sfRenderTexture_drawSprite(viewport, Sprt, NULL);


}

//init array of function pointers
int init_draw_functions(void){

    draw_functions[0] = basic_draw;
}
