#define DRAWABLES

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

#ifndef POOLS
    #include "pools.h"
#endif

#ifndef CONSTANTS
    #include "contants.h"
#endif

#ifndef GEOMETRY
    #include "geometry.h"
#endif

#ifndef DRAWFUNCTIONS
    #include "drawfunctions.h"
#endif

//-----------------------------CONSTANTS----------------------------//

//Constants may be found in constants.h

//-----------------------------STRUCURES------------------------------//

typedef struct {

    int Pool_Self;
    int Pool_Prev;
    int Pool_Next;
    int Pool_In_Use;

    int Local_Next;
    int Local_Prev;
    int Visible_Next;
    int Visible_Prev;
    
    Map_Coords location;

    int animation[ANIMATIONS_PER_DRAWABLE];
    int angle[ANIMATIONS_PER_DRAWABLE];
    int frame[ANIMATIONS_PER_DRAWABLE];
} Drawable;

//-----------------------------GLOBALS----------------------------//

DECLARE_POOL(Drawable, DRAWABLE_POOL_LEVEL_1, DRAWABLE_POOL_LEVEL_2)

//-----------------------------FUNCTIONS--------------------------//


int plot_drawable(int drawable_int){

   Drawable* Drwg = Drawables[drawable_int];
   
   Map_Coords MC = Drwg->location;

   int i;
   
   for (i = 0; i < ANIMATIONS_PER_DRAWABLE; i++){
   
       int animation_int = Drwg-> animation [i];
       int angle_int = Drwg-> angle [i];
       int frame_int = Drwg-> frame [i];

       if (animation_int < 0) continue;
       
       
       Animation* Anim = Animations[animation_int];
       int draw_fn_int = Anim->Vdrawfunction;
       
       draw_functions[draw_fn_int](animation_int, angle_int, frame_int, MC);
   
   }


}

int new_tree(Map_Coods MC){

    int Drwg = Drawable_New(NEXT_AVAILABLE);
    Drawable* Tree = Drawables[Drwg];
    
    Tree->location = MC;

    //Add to list contained in terrain_square
    
    Tree->animation = 0;
    Tree->anlge = 0;
    Tree->frame = 0;
    
}
    

}
