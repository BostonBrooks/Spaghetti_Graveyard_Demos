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

#define BASICDRAW     1
#define NULLDRAW      2
#define REPEATDRAW    3

typedef struct {

    bbMapCoords location;

    int animation[ANIMATIONS_PER_DRAWABLE];
    int angle[ANIMATIONS_PER_DRAWABLE];
    int frame[ANIMATIONS_PER_DRAWABLE];
    int drawfunction[ANIMATIONS_PER_DRAWABLE];

} bbDrawable;

int (*bbDrawfunction_vtable[NUMBER_OF_DRAW_FUNCTIONS])(bbDrawable* tobedrawn, int i);

int bbDrawable_plot(bbDrawable* tobedrawn){



   int i;
   
   for (i=0; i < ANIMATIONS_PER_DRAWABLE; i++){
   
       if (tobedrawn->drawfunction[i] >= 0){
           bbDrawfunction_vtable[tobedrawn->drawfunction[i]](tobedrawn, i);
       }
   }

}

bbDrawable* PineTree_new(bbMapCoords mc){

    bbDrawable* PineTree =  malloc(sizeof(bbDrawable));
    
    PineTree->location = mc;
    PineTree->animation[0] = 0;
    PineTree->angle[0] = 0;
    PineTree->frame[0] = 0;
    PineTree->drawfunction[0] = BASICDRAW;
    
    
    PineTree->location = mc;
    PineTree->animation[1] = 1;
    PineTree->angle[1] = 0;
    PineTree->frame[1] = 0;
    PineTree->drawfunction[1] = REPEATDRAW;
    
    int i;
    
    for (i=2; i < ANIMATIONS_PER_DRAWABLE; i++){
   
   
       PineTree->animation[i] = -1;
       PineTree->angle[i] = -1;
       PineTree->frame[i] = -1;
       PineTree->drawfunction[i] = -1;
       
    }

}



//-----------------------------CODE----------------------------//

int basicdraw(bbDrawable* tobedrawn, int i){
    int animation = tobedrawn->animation[i];
    int angle = tobedrawn->angle[i];
    int frame = tobedrawn->frame[i];
    
    bbMapCoords mc = tobedrawn->location;
    bbScreenCoords sc = bbMapCoords_getScreenCoords_centre(mc);
    
    sfVector2f position;
    position.x = sc.x;
    position.y = sc.y;
    
    sfSprite* sprite = bbAnimation_getSprite(animation,angle,frame);
    
    sfSprite_setPosition(sprite, position);
    sfRenderTexture_drawSprite(bbViewport_main, sprite, NULL);
    
    
}

int nulldraw(bbDrawable* tobedrawn, int i){
    int animation = tobedrawn->animation[i];
    int angle = tobedrawn->angle[i];
    int frame = tobedrawn->frame[i];
    
    bbMapCoords mc = tobedrawn->location;
    bbScreenCoords sc = bbMapCoords_getScreenCoords_centre(mc);
    
    sfVector2f position;
    position.x = sc.x;
    position.y = sc.y;
    
    sfSprite* sprite = sfSprite_vtable[0];
    
    sfSprite_setPosition(sprite, position);
    sfRenderTexture_drawSprite(bbViewport_main, sprite, NULL);
    
    
}

int repeatdraw(bbDrawable* tobedrawn, int i){
    int animation = tobedrawn->animation[i];
    int angle = tobedrawn->angle[i];
    int frame = tobedrawn->frame[i];
    
    
    bbMapCoords mc = tobedrawn->location;
    bbScreenCoords sc = bbMapCoords_getScreenCoords_centre(mc);
    
    sfVector2f position;
    position.x = sc.x;
    position.y = sc.y;
    
    sfSprite* sprite = bbAnimation_getSprite(animation,angle,frame);
    
    sfSprite_setPosition(sprite, position);
    sfRenderTexture_drawSprite(bbViewport_main, sprite, NULL);
    
    bbAnimation* anim = bbAnimation_vtable[animation];
    
    int frames = anim->frames;
    
    tobedrawn->frame[i] = (frame+1)%frames;
    
    
}

int bbDrawfunction_initAll(){

    bbDrawfunction_vtable[BASICDRAW] = basicdraw;
    bbDrawfunction_vtable[NULLDRAW] = nulldraw;
    bbDrawfunction_vtable[REPEATDRAW] = repeatdraw;
}

