/**
 * @file
 *
 * @ingroup 
 * @brief A drawable is any object in the game that is drawn to the viewport 
 */


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

#define NO_HEALTHBAR -1
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


DECLARE_POOL_GLOBALS(bbDrawable, DRAWABLE_POOL_LEVEL_1, DRAWABLE_POOL_LEVEL_2)

/** return 1 if a is closer to the screen than b, 0 otherwise */
int bbDrawable_isCloser (bbDrawable* a, bbDrawable* b);

/** create a new drawable object (in the form of a barrel */
int bbDrawable_new(bbMapCoords MC);

