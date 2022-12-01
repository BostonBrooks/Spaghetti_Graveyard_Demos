#define DRAWABLES
//-----------------------------INCLUDES----------------------------//

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif


#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

#ifndef MEDIA
#define MEDIA
#include "../headers/media.h"
#endif

#ifndef DRAWFUNCTIONS
#define DRAWFUNCTIONS
#include "../headers/drawfunctions.h"
#endif

typedef struct {

    bbMapCoords location;

    int animation[ANIMATIONS_PER_DRAWABLE];
    int angle[ANIMATIONS_PER_DRAWABLE];
    int frame[ANIMATIONS_PER_DRAWABLE];
    int drawfunction[ANIMATIONS_PER_DRAWABLE];

} bbDrawable;


int bbDrawable_plot(bbDrawable* tobedrawn);

bbDrawable* PineTree_new(bbMapCoords mc);


#define BASICDRAW 1
#define NULLDRAW 2

extern int (*bbDrawfunction_vtable[NUMBER_OF_DRAW_FUNCTIONS])(bbDrawable* tobedrawn, int i);

//-----------------------------CODE----------------------------//

int basicdraw(bbDrawable* tobedrawn, int i);
int nulldraw(bbDrawable* tobedrawn, int i);

int bbDrawfunction_initAll();
