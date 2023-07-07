#define BBDRAWABLE_PLOT
//-----------------------------INCLUDES----------------------------//

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef VIEWPORT
#define VIEWPORT
#include "../headers/viewport.h"
#endif

#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

#ifndef BBDRAWABLE
#define BBDRAWABLE
#include "../headers/bbDrawable.h"
#endif

#ifndef BBDRAWABLE_LISTS
#define BBDRAWABLE_LISTS
#include "../headers/bbDrawable_lists.h"
#endif

#ifndef BBGROUNDSURFACE
#define BBGROUNDSURFACE
#include "../headers/bbGroundSurface.h"
#endif

#ifndef MEDIA
#define MEDIA
#include "../headers/media.h"
#endif

//-----------------------------GLOBALS----------------------------//



int (*bbDrawfunction_vtable[NUMBER_OF_DRAW_FUNCTIONS])(bbDrawable* tobedrawn, int i);
extern int Current_Time; //TODO this should be in a header/code file

sfCircleShape* Circle;

//-----------------------------CODE----------------------------//
int bbDrawable_plot(bbDrawable* tobedrawn){



   int i;
   
   for (i=0; i < ANIMATIONS_PER_DRAWABLE; i++){
   
       if (tobedrawn->drawfunction[i] >= 0){
           bbDrawfunction_vtable[tobedrawn->drawfunction[i]](tobedrawn, i);
       }
   }
   
   //if (tobedrawn->display_health_until != NO_HEALTHBAR
   // && tobedrawn->display_health_until <= Current_Time)
   
   if (tobedrawn->display_health_until != NO_HEALTHBAR && tobedrawn->display_health_until >= Current_Time){
    
        bbMapCoords mc = tobedrawn->location;
        bbScreenCoords sc = bbMapCoords_getScreenCoords_centre(mc);
        
        float top, bottom, left, middle, right;
        
        float health_bar_height = tobedrawn->health_bar_height;
        float health_bar_width = tobedrawn->health_bar_width;
        float health_bar_length = tobedrawn->health_bar_length;
        float health = tobedrawn->health;
        float max_health = tobedrawn->max_health;
        
        
        bottom = sc.y - health_bar_height;
        top = bottom - health_bar_width;
        left = sc.x - health_bar_length/2.0;
        middle = left + health_bar_length * (health*1.0) / max_health;
        right = left + health_bar_length;
        
        sfVertex points[4];

        points[0].color = sfRed;
        points[1].color = sfRed;
        points[2].color = sfRed;
        points[3].color = sfRed;
        
        
        points[0].position.x = left;
        points[0].position.y = top;
        points[1].position.x = right;
        points[1].position.y = top;
        points[2].position.x = left;
        points[2].position.y = bottom;
        points[3].position.x = right;
        points[3].position.y = bottom;
        
        sfRenderTexture_drawPrimitives 	( 	bbViewport_healthbars,
		                                &points[0],
		                                4,
		                                sfTrianglesStrip,
		                                NULL//const sfRenderStates *  	states 
	                                );
	                                
	                                

        points[0].color = sfGreen;
        points[1].color = sfGreen;
        points[2].color = sfGreen;
        points[3].color = sfGreen;
        
        
        points[0].position.x = left;
        points[0].position.y = top;
        points[1].position.x = middle;
        points[1].position.y = top;
        points[2].position.x = left;
        points[2].position.y = bottom;
        points[3].position.x = middle;
        points[3].position.y = bottom;
        
        sfRenderTexture_drawPrimitives 	( 	bbViewport_healthbars,
		                                &points[0],
		                                4,
		                                sfTrianglesStrip,
		                                NULL//const sfRenderStates *  	states 
	                                );
        
    }
   
    if(tobedrawn->cosmetic_radius > 0) //or whatever else conditions?
    {
        sfCircleShape_setRadius(Circle, tobedrawn->cosmetic_radius);
        
        sfVector2f origin;
        origin.x = tobedrawn->cosmetic_radius;
        origin.y = tobedrawn->cosmetic_radius;
        sfCircleShape_setOrigin(Circle, origin);
      
        bbMapCoords mc = tobedrawn->location;
        bbSquareCoords sc = bbMapCoords_getSquareCoords (mc);
        if (sc.i < 0 || sc.j < 0 || sc.i >= SQUARES_PER_MAP || sc.j >= SQUARES_PER_MAP) return 0;
    
        bbGroundSurface* Ground_Surface = &bbGroundSurface_grid[sc.i][sc.j];
    
        int pointi = mc.i - sc.i*POINTS_PER_PIXEL * PIXELS_PER_TILE * TILES_PER_SQUARE;
        int pointj = mc.j - sc.j*POINTS_PER_PIXEL * PIXELS_PER_TILE * TILES_PER_SQUARE;
    
        int pixeli = floordiv(pointi, POINTS_PER_PIXEL);
        int pixelj = floordiv(pointj, POINTS_PER_PIXEL);
    
        sfVector2f position;
        position.x = pixelj;
        position.y = pixeli;
    
        sfCircleShape_setPosition(Circle, position);
    
    
        sfRenderTexture* Circles = Ground_Surface->Circles_Render_Texture;
        sfRenderTexture_drawCircleShape(Circles, Circle, NULL);
      
      
      
    
    }
    return 0;
}

int bbDrawable_plot_int(int tobedrawn_int){

   bbDrawable* tobedrawn = bbDrawable_Pool_Lookup(tobedrawn_int);

   int i;
   
   for (i=0; i < ANIMATIONS_PER_DRAWABLE; i++){
   
       if (tobedrawn->drawfunction[i] >= 0){
           bbDrawfunction_vtable[tobedrawn->drawfunction[i]](tobedrawn, i);
       }
   }

   if (tobedrawn->display_health_until != NO_HEALTHBAR
    && tobedrawn->display_health_until <=Current_Time){
    
        bbMapCoords mc = tobedrawn->location;
        bbScreenCoords sc = bbMapCoords_getScreenCoords_centre(mc);
        
        float top, bottom, left, middle, right;
        
        float health_bar_height = tobedrawn->health_bar_height;
        float health_bar_width = tobedrawn->health_bar_width;
        float health_bar_length = tobedrawn->health_bar_length;
        float health = tobedrawn->health;
        float max_health = tobedrawn->max_health;
        
        
        bottom = sc.x - health_bar_height;
        top = bottom - health_bar_width;
        left = sc.x - health_bar_length/2.0;
        middle = left + health_bar_length * (health*1.0) / max_health;
        right = left + health_bar_length;
        
        sfVertex points[4];

        points[0].color = sfRed;
        points[1].color = sfRed;
        points[2].color = sfRed;
        points[3].color = sfRed;
        
        
        points[0].position.x = left;
        points[0].position.y = top;
        points[1].position.x = right;
        points[1].position.y = top;
        points[2].position.x = left;
        points[2].position.y = bottom;
        points[3].position.x = right;
        points[3].position.y = bottom;
        
        sfRenderTexture_drawPrimitives 	( 	bbViewport_healthbars,
		                                &points[0],
		                                4,
		                                sfTrianglesStrip,
		                                NULL//const sfRenderStates *  	states 
	                                );
	                                
	                                

        points[0].color = sfGreen;
        points[1].color = sfGreen;
        points[2].color = sfGreen;
        points[3].color = sfGreen;
        
        
        points[0].position.x = left;
        points[0].position.y = top;
        points[1].position.x = middle;
        points[1].position.y = top;
        points[2].position.x = middle;
        points[2].position.y = bottom;
        points[3].position.x = right;
        points[3].position.y = bottom;
        
        sfRenderTexture_drawPrimitives 	( 	bbViewport_healthbars,
		                                &points[0],
		                                4,
		                                sfTrianglesStrip,
		                                NULL//const sfRenderStates *  	states 
	                                );
        
    }
    
    
    
   return 0;

}

int bbDrawable_plotPool(void){

    int tobedrawn_int = bbDrawable_Pool_In_Use_Head;
    
    if ( tobedrawn_int == -1) return 0;
    
    while (tobedrawn_int != -1){
        bbDrawable* tobedrawn = bbDrawable_Pool_Lookup(tobedrawn_int);
        bbDrawable_plot(tobedrawn);
        tobedrawn_int = tobedrawn->Pool_Next;
        
    }
    return 0;
}

int basicdraw(bbDrawable* tobedrawn, int i){
    int animation = tobedrawn->animation[i];
    int angle = tobedrawn->angle[i];
    int frame = tobedrawn->frame[i];
    
    bbMapCoords mc = tobedrawn->location;
    bbScreenCoords sc = bbMapCoords_getScreenCoords_centre(mc);
    
    //printf("x=%f, y=%f\n", sc.x, sc.y); 
    
    sfVector2f position;
    position.x = sc.x;
    position.y = sc.y;
    
    sfSprite* sprite = bbAnimation_getSprite(animation,angle,frame);
    
    sfSprite_setPosition(sprite, position);
    sfRenderTexture_drawSprite(bbViewport_main, sprite, NULL);
    
    
    
    return 0;
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
    
    return 0;
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
    int framerate = anim->framerate;
    
    tobedrawn->frame[i] = (frame+1)%(frames*framerate);
    
    return 0;
}

int balloondraw(bbDrawable* tobedrawn, int i){

    bbMapCoords mc = tobedrawn->location;
    
    

    int animation = tobedrawn->animation[i];
    int angle = tobedrawn->angle[i];
    int frame = tobedrawn->frame[i];
    
    bbAnimation* anim = bbAnimation_vtable[animation];
    
    int frames = anim->frames;
    int framerate = anim->framerate;
    
    
    mc = tobedrawn->location;
    mc.k += 160 + 160 * sin(2.0*M_PI*frame/(frames*framerate));
    bbScreenCoords sc = bbMapCoords_getScreenCoords_centre(mc);
    
    sfVector2f position;
    position.x = sc.x;
    position.y = sc.y;
    
    sfSprite* sprite = bbAnimation_getSprite(animation,angle,frame);
    
    sfSprite_setPosition(sprite, position);
    sfRenderTexture_drawSprite(bbViewport_main, sprite, NULL);
    
    
    tobedrawn->frame[i] = (frame+1)%(frames*framerate);
    
    return 0;
}

int shadowdraw(bbDrawable* tobedrawn, int i){
    int animation = tobedrawn->animation[i];
    int angle = tobedrawn->angle[i];
    int frame = tobedrawn->frame[i];
    sfSprite* sprite = bbAnimation_getSprite(animation,angle,frame);
    
    bbMapCoords mc = tobedrawn->location;
    bbSquareCoords sc = bbMapCoords_getSquareCoords (mc);
    if (sc.i < 0 || sc.j < 0 || sc.i >= SQUARES_PER_MAP || sc.j >= SQUARES_PER_MAP) return 0;
    bbGroundSurface* Ground_Surface = &bbGroundSurface_grid[sc.i][sc.j];
    
    int pointi = mc.i - sc.i*POINTS_PER_PIXEL * PIXELS_PER_TILE * TILES_PER_SQUARE;
    int pointj = mc.j - sc.j*POINTS_PER_PIXEL * PIXELS_PER_TILE * TILES_PER_SQUARE;
    
    float pixeli = (1.0 * pointi) / POINTS_PER_PIXEL;
    float pixelj = (1.0 * pointj) / POINTS_PER_PIXEL;
    
    sfVector2f position;
    position.x = pixelj;
    position.y = pixeli;
    
    sfSprite_setPosition(sprite, position);
    
    
    sfRenderTexture* Footprint = Ground_Surface->Footprints_Render_Texture;
    sfRenderTexture_drawSprite(Footprint, sprite, NULL);
    
    
    
    
    


}

int bbDrawfunction_initAll(){

    Circle = sfCircleShape_create();
    sfCircleShape_setFillColor(Circle, sfTransparent);
    sfCircleShape_setOutlineColor(Circle, sfCyan);
    sfCircleShape_setOutlineThickness(Circle, 2.0);
    
    

    bbDrawfunction_vtable[DRAW_BASIC] = basicdraw;
    bbDrawfunction_vtable[DRAW_NULL] = nulldraw;
    bbDrawfunction_vtable[DRAW_REPEAT] = repeatdraw;
    bbDrawfunction_vtable[DRAW_BALLOON] = balloondraw;
    bbDrawfunction_vtable[DRAW_SHADOW] = shadowdraw;
    
    return 0;
}


