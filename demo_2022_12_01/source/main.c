#define MAIN

//-----------------------------INCLUDES----------------------------//

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

#ifndef DRAWABLES
#define DRAWABLES
#include "../headers/drawables.h"
#endif

#ifndef DRAWFUNCTIONS
#define DRAWFUNCTIONS
#include "../headers/drawfunctions.h"
#endif

#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

#ifndef GROUNDSURFACE
#define GROUNDSURFACE
#include "../headers/groundsurface.h"
#endif

#ifndef INTERPOLATION
#define INTERPOLATION
#include "../headers/interpolation.h"
#endif

#ifndef MEDIA
#define MEDIA
#include "../headers/media.h"
#endif

#ifndef POOLS
#define POOLS
#include "../headers/pools.h"
#endif

#ifndef SPHERE
#define SPHERE
#include "../headers/sphere.h"
#endif

#ifndef TERRAINSQUARES
#define TERRAINSQUARES
#include "../headers/terrain_squares.h"
#endif

#ifndef VIEWPORT
#define VIEWPORT
#include "../headers/viewport.h"
#endif

#ifndef WINDOW
#define WINDOW
#include "../headers/window.h"
#endif


//-----------------------------CONSTANTS----------------------------//

//-----------------------------STRUCURES------------------------------//

//-----------------------------GLOBALS----------------------------//

//-----------------------------FUNCTIONS--------------------------//

int main(void){

    LightIncedence.i = 1;
    LightIncedence.j = 1;
    LightIncedence.k = 1;
    
    viewpoint.i = 2000;
    viewpoint.j = 3000;
    viewpoint.k = 0;
    
    Load_Media();
    bbElevations_load();
    Init_Window();
    
    sfRenderWindow_drawSprite(window, sfSprite_vtable[39],NULL);
    sfRenderWindow_display(window);
    
    bbViewport_init();
    bbGroundSurface_initAll();
    bbDrawfunction_initAll();
    
    bbDrawable* PineTree = PineTree_new(viewpoint);
    
    bbDrawable *PineTrees[32];
    
    bbMapCoords mc;
    int i;
    for (i=0;i<32;i++){
    
        mc.i = rand()%30000;
        mc.j = rand()%30000;
        bbMapCoords_updateElevation(&mc);
        PineTrees[i] = PineTree_new(mc);
    }
    
        
    sfRenderTexture_drawSprite(viewport, sfSprite_vtable[28], NULL); //viewport background
    
    Create_Ground_Shaders();
    
        
    while(1){
    
	
	
        for (i=0;i<32;i++){
    
	    bbDrawable_plot(PineTrees[i]);
        }
	
	bbGroundSurface_drawVisible();  
        


        bbMapCoords mc;
        
        sfEvent event;
        bbScreenCoords point;
        
        while(sfRenderWindow_pollEvent(window, &event)){
            
        
        
            if (event.type == sfEvtMouseMoved){
                point.x = event.mouseMove.x;
                point.y = event.mouseMove.y;
                
                point.x = point.x -  VIEWPORT_LEFT;
                point.y = point.y -  VIEWPORT_TOP;
            } else if (event.type == sfEvtMouseButtonPressed){
            
                point.x = event.mouseButton.x -  VIEWPORT_LEFT;
                point.y = event.mouseButton.y -  VIEWPORT_TOP;
                mc = bbScreenCoords_getMapCoords (point);
                viewpoint = mc;
            }
        
        }
    
        mc = bbScreenCoords_getMapCoords (point);
        
        mc.k = 512;
        bbSphere_draw (mc);
        sfRenderTexture_display(viewport);
        
        sfVector2f position;
        
        position.x = 100;    position.y = 100;
        sfSprite_setPosition(sfSprite_vtable[30], position);
        sfRenderTexture_drawSprite(bbViewport_background, sfSprite_vtable[30], NULL);
        
        position.x = 100;    position.y = 200;
        sfSprite_setPosition(sfSprite_vtable[31], position);
        sfRenderTexture_drawSprite(bbViewport_ground, sfSprite_vtable[31], NULL);
        
        position.x = 100;    position.y = 300;
        sfSprite_setPosition(sfSprite_vtable[32], position);
        sfRenderTexture_drawSprite(bbViewport_main, sfSprite_vtable[32], NULL);
        
        position.x = 100;    position.y = 400;
        sfSprite_setPosition(sfSprite_vtable[33], position);
        sfRenderTexture_drawSprite(bbViewport_highlights, sfSprite_vtable[33], NULL);
        
        position.x = 100;    position.y = 500;
        sfSprite_setPosition(sfSprite_vtable[34], position);
        sfRenderTexture_drawSprite(bbViewport_healthbars, sfSprite_vtable[34], NULL);
        
        sfRenderTexture_display(bbViewport_background);
        sfRenderTexture_display(bbViewport_ground);
        sfRenderTexture_display(bbViewport_main);
        sfRenderTexture_display(bbViewport_highlights);
        sfRenderTexture_display(bbViewport_healthbars);
        
        
        sfRenderWindow_drawSprite(window, viewport_sprite, &bbViewport_renderer);
        Display_Decal();
        
        //sfRenderWindow_drawSprite(window, viewport_drawable_sprite, NULL);
        
        sfRenderWindow_display(window);
        
        sfRenderTexture_clear(bbViewport_background, sfBlue);
        sfRenderTexture_clear(bbViewport_ground, sfTransparent);
        sfRenderTexture_clear(bbViewport_main, sfTransparent);
        sfRenderTexture_clear(bbViewport_highlights, sfTransparent);
        sfRenderTexture_clear(bbViewport_healthbars, sfTransparent);
    
    }



}

