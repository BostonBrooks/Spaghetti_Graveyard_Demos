#define MAIN

//-----------------------------SYSTEM FILES----------------------------//

#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

//-----------------------------LOCAL FILES-----------------------------//

#include "constants.h"
#include "media.h"
#include "window.h"
#include "viewport.h"
#include "geometry.h"
#include "groundsurface.h"
#include "sphere.h"

//-----------------------------CONSTANTS----------------------------//

//-----------------------------STRUCURES------------------------------//

//-----------------------------GLOBALS----------------------------//

//-----------------------------FUNCTIONS--------------------------//

int Display_Splash(void){

}

int Init_All(void){


    LightIncedence.i = 1;
    LightIncedence.j = 1;
    LightIncedence.k = 1;
    
    viewpoint.i = 2000;
    viewpoint.j = 3000;
    viewpoint.k = 0;
    
    
    bicubic_coefficients_tile_i = -1;
    bicubic_coefficients_tile_j = -1;

    Load_Media();
    load_elevations();
    Init_Window();
    Init_Viewport();
    Init_Ground_Surfaces();
    
    sfRenderTexture_drawSprite(viewport, Sprites[28], NULL); //viewport background
    Create_Ground_Shaders();
    
    

//Not used in this demo:
/*
    Draw_Ground_Square(0, 4);
    Draw_Ground_Square(0, 3);
    Draw_Ground_Square(1, 4);
    Draw_Ground_Square(0, 2);
    Draw_Ground_Square(1, 3);
    Draw_Ground_Square(2, 4);
    Draw_Ground_Square(0, 1);
    Draw_Ground_Square(1, 2);
    Draw_Ground_Square(2, 3);
    Draw_Ground_Square(3, 4);
*/
    Draw_Ground_Square(0, 0);

//Not used in this demo:
/*    
    Draw_Ground_Square(1, 1);
    Draw_Ground_Square(2, 2);
    Draw_Ground_Square(3, 3);
    Draw_Ground_Square(4, 4);
    
    Draw_Ground_Square(1, 0);
    Draw_Ground_Square(2, 1);
    Draw_Ground_Square(3, 2);
    Draw_Ground_Square(4, 3);
    Draw_Ground_Square(2, 0);
    Draw_Ground_Square(3, 1);
    Draw_Ground_Square(4, 2);
    Draw_Ground_Square(3, 0);
    Draw_Ground_Square(4, 1);
    Draw_Ground_Square(4, 0);
    
*/

//Not used in this demo:
/*
    Map_Coords mc;
        
    int i_coord;
    int j_coord;
    int k_coord;
    
    
    mc.i = viewpoint.i;
    mc.j = viewpoint.j;
    mc.k = viewpoint.k;
    
    Draw_Sphere_IJK (mc);
    
    
    i_coord = Square_Ground_Surfaces[0][1].i_Coord;
    j_coord = Square_Ground_Surfaces[0][1].j_Coord;
    k_coord = 0;
    
    mc.i = i_coord * POINTS_PER_SQUARE;
    mc.j = j_coord * POINTS_PER_SQUARE;
    mc.k = 0;
    
    Draw_Sphere_IJK (mc);
    
    
    i_coord = Square_Ground_Surfaces[0][0].i_Coord;
    j_coord = Square_Ground_Surfaces[0][0].j_Coord;
    k_coord = 0;
    
    mc.i = i_coord * POINTS_PER_SQUARE;
    mc.j = j_coord * POINTS_PER_SQUARE;
    mc.k = 0;
    
    Draw_Sphere_IJK (mc);
    
    
    i_coord = Square_Ground_Surfaces[1][1].i_Coord;
    j_coord = Square_Ground_Surfaces[1][1].j_Coord;
    k_coord = 0;
    
    mc.i = i_coord * POINTS_PER_SQUARE;
    mc.j = j_coord * POINTS_PER_SQUARE;
    mc.k = 0;
    
    Draw_Sphere_IJK (mc);
    
    
    i_coord = Square_Ground_Surfaces[1][0].i_Coord;
    j_coord = Square_Ground_Surfaces[1][0].j_Coord;
    k_coord = 0;
    
    mc.i = i_coord * POINTS_PER_SQUARE;
    mc.j = j_coord * POINTS_PER_SQUARE;
    mc.k = 0;
    
    Draw_Sphere_IJK (mc);
    
    
    i_coord = Square_Ground_Surfaces[0][1].i_Coord;
    j_coord = Square_Ground_Surfaces[0][1].j_Coord;
    k_coord = 0;
    
    mc.i = i_coord * POINTS_PER_SQUARE;
    mc.j = j_coord * POINTS_PER_SQUARE;
    mc.k = 0;
    
    Draw_Sphere_IJK (mc);
*/

    sfRenderTexture_display(viewport);
    sfRenderWindow_drawSprite(window, viewport_sprite, NULL);
    Display_Decal();
    sfRenderWindow_display(window);



}

int main(void){

    Init_All();
    
    sfSleep(sfSeconds(60));

}
