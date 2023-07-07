/**
 * @file
 *
 * @ingroup 
 * @brief Load media from disk
 */

#define MEDIA

//-----------------------------INCLUDES----------------------------//

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif


//-----------------------------STRUCURES------------------------------//

typedef struct {

int Vdrawfunction;
int angles;
int frames;
int framerate;
int sprites[];

} bbAnimation;

//-----------------------------GLOBALS----------------------------//

extern sfTexture** sfTexture_vtable;
extern sfSprite** sfSprite_vtable;
extern bbAnimation** bbAnimation_vtable;

//-----------------------------FUNCTIONS--------------------------//

/** for testing purposes */
int print_next_line(FILE* Media);

/** create vtable for sfTextures */
int sfTexture_vtable_alloc(int number);

/** create vtable for sfSprites */
int sfSprite_vtable_alloc(int number);

/** create vtable for bbAnimations */
int bbAnimation_vtable_alloc(int number);


/** Load texture for disk */
int sfTexture_to_vtable(int VAddress, char File[], sfBool smooth);

/** Create sprite from texture */
int sfSprite_to_vtable(int VAddress, int TextureVAddress, int left, int top, int width,
    int height, float originx, float originy, float scalex, float scaley);

/** Create animation from sprites */
int bbAnimation_to_vtable(int VAddress, int Vdrawfunction, int angles, int frames, int framerate, int sprites[]);

/** Load media from disk */
int Load_Media();

/** Lookup sprite with intention of drawing to screen */
sfSprite* bbAnimation_getSprite(int animation_int, int angle_int, int frame_int);

