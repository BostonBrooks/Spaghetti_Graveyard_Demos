/**
 * @file
 *
 * @ingroup 
 * @brief Plot drawables to viewport
 */

#define BBDRAWABLE_PLOT
//-----------------------------INCLUDES----------------------------//
#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif


#ifndef BBDRAWABLE
#define BBDRAWABLE
#include "../headers/bbDrawable.h"
#endif
//-----------------------------GLOBALS----------------------------//

//Drawfunction types:
/*
#define DRAW_BASIC      0
#define DRAW_NULL       1
#define DRAW_REPEAT     2
#define DRAW_BALLOON    3
#define DRAW_SHADOW     4
*/

extern int (*bbDrawfunction_vtable[NUMBER_OF_DRAW_FUNCTIONS])(bbDrawable* tobedrawn, int i);
extern int Current_Time; //TODO this should be in a header/code file

extern sfCircleShape* Circle;

//-----------------------------CODE----------------------------//

/** Plot one bbDrawable to the viewport */
int bbDrawable_plot(bbDrawable* tobedrawn);

/** Plot one bbDrawable to the viewport given location in pool */
int bbDrawable_plot_int(int tobedrawn_int);

/** Plot all drawable sin pool wihtout regard to order */
int bbDrawable_plotPool(void);

/** Plot one animation from a drawable, do not advance the frame */
int basicdraw(bbDrawable* tobedrawn, int i);

/** Plot one animation from a drawable, do not advance the frame */
int nulldraw(bbDrawable* tobedrawn, int i);

/** Plot one frame from an animation and advance the animation*/
int repeatdraw(bbDrawable* tobedrawn, int i);

/** Plot a baloon, floating up and down */
int balloondraw(bbDrawable* tobedrawn, int i);

/** Plot the shadow under a unit */
int shadowdraw(bbDrawable* tobedrawn, int i);

/** Initialise vtable of draw functions */
int bbDrawfunction_initAll();

