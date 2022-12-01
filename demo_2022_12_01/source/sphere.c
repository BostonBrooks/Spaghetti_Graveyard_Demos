
#define SPHERE
//-----------------------------INCLUDES----------------------------//

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

#ifndef MEDIA
#define MEDIA
#include "../headers/media.h"
#endif

#ifndef VIEWPORT
#define VIEWPORT
#include "../headers/viewport.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif


#ifndef GROUNDSURFACE
#define GROUNDSURFACE
#include "../headers/groundsurface.h"
#endif


//-----------------------------CODE----------------------------//
//TODO - move this functionality to drawfunctions.
int draw_sprite_to_ground(bbMapCoords mc, int sprite){

    bbSquareCoords sc;
    sc = bbMapCoords_getSquareCoords (mc);
    if (sc.i < 0 || sc.j < 0 || sc.i >= SQUARES_PER_MAP || sc.j >= SQUARES_PER_MAP) return 0;
    
    bbGroundSurface* Ground_Surface = &bbGroundSurface_grid[sc.i][sc.j];
    
    int pointi = mc.i - sc.i*POINTS_PER_PIXEL * PIXELS_PER_TILE * TILES_PER_SQUARE;
    int pointj = mc.j - sc.j*POINTS_PER_PIXEL * PIXELS_PER_TILE * TILES_PER_SQUARE;
    
    int pixeli = floordiv(pointi, POINTS_PER_PIXEL);
    int pixelj = floordiv(pointj, POINTS_PER_PIXEL);
    
    sfVector2f position;
    position.x = pixelj;
    position.y = pixeli;
    
    sfSprite_setPosition(sfSprite_vtable[sprite], position);
    
    
    sfRenderTexture* Footprint = Ground_Surface->Footprints_Render_Texture;
    sfRenderTexture_drawSprite(Footprint, sfSprite_vtable[sprite], NULL);
    
    
    
    

}


int bbSphere_draw ( bbMapCoords mc){

    bbScreenCoords sc;
    
    sc = bbMapCoords_getScreenCoords_centre(mc);
    
    sfVector2f position;
    position.x = sc.x;
    position.y = sc.y;
    
    sfSprite_setPosition(sfSprite_vtable[29], position);
    
    sfRenderTexture_drawSprite(bbViewport_main, sfSprite_vtable[29], NULL);
    
    draw_sprite_to_ground(mc, 40);
    
    

}
