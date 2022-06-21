#define SPHERE

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
//-----------------------------CONSTANTS----------------------------//

#ifndef GEOMETRY
#include "geometry.h"
#endif

int Draw_Sphere_IJK ( Map_Coords mc){

    Screen_Coords sc;
    
    sc = map_to_screen_centre_viewpoint(mc);
    
    sfVector2f position;
    position.x = sc.x;
    position.y = sc.y;
    
    sfSprite_setPosition(Sprites[29], position);
    
    sfRenderTexture_drawSprite(viewport_drawable, Sprites[29], NULL);

}
