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
int sprites[];

} bbAnimation;

//-----------------------------GLOBALS----------------------------//

extern sfTexture** sfTexture_vtable;
extern sfSprite** sfSprite_vtable;
extern bbAnimation** bbAnimation_vtable;

//-----------------------------FUNCTIONS--------------------------//

int sfTexture_vtable_alloc(int number);

int sfSprite_vtable_alloc(int number);

int bbAnimation_vtable_alloc(int number);

int sfTexture_to_vtable(int VAddress, char File[], sfBool smooth);

int sfSprite_to_vtable(int VAddress, int TextureVAddress, int left, int top, int width,
    int height, float originx, float originy, float scalex, float scaley);

int bbAnimation_to_vtable(int VAddress, int Vdrawfunction, int angles, int frames, int sprites[]);

int Load_Media();


sfSprite* bbAnimation_getSprite(int animation_int, int angle_int, int frame_int);
