#define BBDRAWABLE
//-----------------------------INCLUDES----------------------------//


#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

#ifndef POOLS
#define POOLS
#include "../headers/pools.h"
#endif

#ifndef MEDIA
#define MEDIA
#include "../headers/media.h"
#endif

//#define NO_HEALTHBAR -1  // what even is this?
//-----------------------------STRUCTS------------------------------//

/** A drawable is any object in the game that is drawn to the viewport */
 typedef struct {

/* Housekeeping Data: */
    bbSquareCoords SquareCoords;
    //what terrain square contains the drawable?
    //int i_Coord;
    //int j_Coord;
 
    int Pool_Self;
    int Pool_Prev;
    int Pool_Next;
    int Pool_In_Use;
    
    int Square_Prev;
    int Square_Next;
    
/* Cosmetic Data */

    bbMapCoords location;
    
    int health;
    int max_health;
    int display_health_until;
    int health_bar_height;
    int health_bar_width;
    int health_bar_length;
    
    int cosmetic_radius;

    int animation[ANIMATIONS_PER_DRAWABLE];
    int angle[ANIMATIONS_PER_DRAWABLE];
    int frame[ANIMATIONS_PER_DRAWABLE];
    int drawfunction[ANIMATIONS_PER_DRAWABLE];
    int start_time; //frame = ((current time - start time)/framerate)%frames
    
/* Avoidance Data */
    
    float avoidance_radius;
    float mass;
    float speed;
    int shape;        //NO_POTENTIAL for no repulsion
    bbMapCoords preferred_next_location;
    bbMapCoords target_location;
    int target_drawable;

/* Interactive Data: */

    int onclick_function; //might want to implement a vtable or just not use this variable
    int AI_Controller;
    int Interactivity; //(0 = none, 1 = enemy, 2 = chest)
    bbFloatRect Hit_Box;
    
    
     

} bbDrawable;

//-----------------------------CODE------------------------------//


DEFINE_POOL(bbDrawable, DRAWABLE_POOL_LEVEL_1, DRAWABLE_POOL_LEVEL_2)

/** return 1 if a is closer to the screen than b, 0 otherwise */
int bbDrawable_isCloser (bbDrawable* a, bbDrawable* b){ 

    bbMapCoords mca = a->location;
    bbMapCoords mcb = b->location;
    
    return bbMapCoords_isCloser(mca, mcb);

}
/** Create a new drawable (a barrel) */
int bbDrawable_new(bbMapCoords MC){

    //#ifdef DEBUG
    //printf("Create Drawable:\n");
    //#endif
    
    int drawable_int = bbDrawable_Pool_New(NEXT_AVAILABLE);
    assert(drawable_int >= 0);
    
    bbDrawable* drawable = bbDrawable_Pool_Lookup(drawable_int);

/* Cosmetic Data */
    
    //#ifdef DEBUG
    //printf("Set Cosmetic Data\n");
    //#endif
    
    
    drawable->location              = MC;
    drawable->health                = -1;
    drawable->max_health            = -1;
    drawable->display_health_until  = -1;
    drawable->health_bar_height     = -1;
    drawable->health_bar_width      = -1;
    drawable->health_bar_length     = -1;
    
    int cosmetic_radius             = -1;
    
    drawable->animation[0]          = 16;
    drawable->angle[0]              = 0;
    drawable->frame[0]              = 0;
    drawable->drawfunction[0]       = DRAW_BASIC;
     
    //#ifdef DEBUG
    //printf("Set Null Animations\n");
    //#endif
       
    for (int i=1; i < ANIMATIONS_PER_DRAWABLE; i++){
   
   
       drawable->animation[i] = -1;
       drawable->angle[i] = -1;
       drawable->frame[i] = -1;
       drawable->drawfunction[i] = -1;
       
    }
    
    bbMapCoords NULL_Coords;
    
    NULL_Coords.i = -1;
    NULL_Coords.j = -1;
    NULL_Coords.k = -1;

/* Avoidance Data */
    
    //#ifdef DEBUG
    //printf("Set Avoidance Data\n");
    //#endif
      
    drawable->avoidance_radius        = 20 * POINTS_PER_PIXEL;
    drawable->mass                    = -1;
    drawable->speed                   = -1;
    drawable->shape                   = AVOIDANCE_CIRCLULAR;
    drawable->preferred_next_location = NULL_Coords;
    drawable->target_location         = NULL_Coords;
    drawable->target_drawable         = -1;
    
/* Interactive Data: */
    
    //#ifdef DEBUG
    //printf("Set Interactive Data\n");
    //#endif
    
    bbFloatRect NULL_Hit_Box;
    NULL_Hit_Box.top      = -1;
    NULL_Hit_Box.left     = -1;
    NULL_Hit_Box.height   = -1;
    NULL_Hit_Box.width    = -1;

    drawable->onclick_function  = -1;
    drawable->AI_Controller     = -1;
    drawable->Interactivity     = -1;
    drawable->Hit_Box           = NULL_Hit_Box;
    

/* Add to Terrain Sqaure */

    //#ifdef DEBUG
    //printf("Add to Terrain Sqaure:\n");
    //#endif

    bbDrawable_addtoTS (drawable_int);

    return drawable_int;
}
