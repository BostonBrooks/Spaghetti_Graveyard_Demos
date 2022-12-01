#define MEDIA

//-----------------------------INCLUDES----------------------------//

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif



//-----------------------------STRUCURES------------------------------//

typedef struct {

int Vdrawfunction;
int angles;
int frames;
int sprites[];

} bbAnimation;

//-----------------------------GLOBALS----------------------------//

sfTexture** sfTexture_vtable;
sfSprite** sfSprite_vtable;
bbAnimation** bbAnimation_vtable;

//-----------------------------FUNCTIONS--------------------------//

int sfTexture_vtable_alloc(int number) {
    sfTexture_vtable = (sfTexture**) malloc (sizeof(sfTexture*) * number);
}

int sfSprite_vtable_alloc(int number) {
    sfSprite_vtable = (sfSprite**) malloc (sizeof(sfSprite*) * number);
}

int bbAnimation_vtable_alloc(int number) {
    bbAnimation_vtable = (bbAnimation**) malloc (sizeof(bbAnimation*) * number);
}

int sfTexture_to_vtable(int VAddress, char File[], sfBool smooth){

    sfTexture* New_Texture = sfTexture_createFromFile(File, NULL);
    assert(New_Texture != NULL);
    
    sfTexture_setSmooth(New_Texture, smooth);
    
    sfTexture_vtable[VAddress] = New_Texture;
}

int sfSprite_to_vtable(int VAddress, int TextureVAddress, int left, int top, int width,
    int height, float originx, float originy, float scalex, float scaley){

    sfSprite* New_Sprite = sfSprite_create();
    
    sfSprite_setTexture(New_Sprite, sfTexture_vtable[TextureVAddress], sfFalse);
    
    sfIntRect rectangle;
    
    rectangle.left = left;
    rectangle.top = top;
    rectangle.width = width;
    rectangle.height = height;
    
    sfSprite_setTextureRect(New_Sprite, rectangle);
    
    sfVector2f origin;
    
    origin.x = originx;
    origin.y = originy;
    
    sfSprite_setOrigin(New_Sprite, origin);
    
    sfVector2f scale;
    scale.x = scalex;
    scale.y = scaley;
    
    sfSprite_setScale(New_Sprite, scale);
    
    sfSprite_vtable[VAddress] = New_Sprite;
   
}

int bbAnimation_to_vtable(int VAddress, int Vdrawfunction, int angles, int frames, int sprites[]){

    bbAnimation* animation = malloc (sizeof(bbAnimation) + sizeof(int) * angles * frames);
    
    animation->Vdrawfunction = Vdrawfunction;
    animation->angles = angles;
    animation->frames = frames;
    
    //printf("%d, %d, %d\n", animation->Vdrawfunction,animation->angles,animation->frames);
    
    int i;
    
    for (i = 0; i < angles*frames; i++){
    
        animation->sprites[i] = sprites[i];
    
    }

    bbAnimation_vtable[VAddress] = animation; 

}

int Load_Media(){

// Input file

    FILE* Media = fopen("./media/media.txt", "r");
    assert(Media != NULL);
    
    int return_value;
    int number;
    
// Textures
    
    return_value = fscanf(Media, "sfTexture:\n\n");
    assert (return_value == 0);
    
    return_value = fscanf(Media, "Number of Textures: %d\n\n", &number);
    assert (return_value == 1);
    
    sfTexture_vtable_alloc(number);
    
    return_value = fscanf(Media, "VAddress, File, Smooth:\n\n");
    assert (return_value == 0);
    
    int VAddress;
    char File_Name[100];
    char Smooth;
    
    
    while((return_value = fscanf(Media, "%d, \"%99[^\"]\", %c\n", &VAddress, &File_Name[0], &Smooth)) == 3){
        
        
        //printf("\n\n%s\n\n", File_Name);    //debug
        
        if (Smooth == 'T') {
            sfTexture_to_vtable(VAddress, File_Name, sfTrue);
        } else  {
            sfTexture_to_vtable(VAddress, File_Name, sfFalse);
        }
         
    }

// Sprites

    return_value = fscanf(Media, "\nsfSprite:\n\n");
    assert (return_value == 0);
    
    
    return_value = fscanf(Media, "Number of Sprites: %d\n\n", &number);
    assert (return_value == 1);
    
    sfSprite_vtable_alloc(number);
    
    return_value = fscanf(Media, "label, VAddress, sfTexture, left, top, width, height, originx, originy, scalex, scaley\n\n");
    assert (return_value == 0);
    
    char Label[100];
    //int VAddress;
    int TextureVAddress;
    int left, top, width, height;
    float originx, originy;
    float scalex, scaley;
    
    while((return_value = fscanf(Media, "\"%99[^\"]\", %d, %d, %d, %d, %d, %d, %f, %f, %f, %f\n",
     &Label[0], &VAddress, &TextureVAddress, &left, &top, &width, &height,
      &originx, &originy, &scalex, &scaley)) == 11){

        //printf("\n\n%s\n\n", Label);    //debug
        
        sfSprite_to_vtable(VAddress, TextureVAddress, left, top, width, height,
         originx, originy, scalex, scaley);
        
         
    }
   
// Animations    

    //char Label[100]
    //int VAddress;
    int Vdrawfunction;
    int angles;
    int frames;
    int sprites[256];

    return_value = fscanf(Media, "\nAnimation:\n\n");
    assert (return_value == 0);
    
    return_value = fscanf(Media, "Number of Animations: %d\n\n", &number);
    
    
    assert (return_value == 1);
    
    bbAnimation_vtable_alloc(number);
    
    return_value = fscanf(Media, "label, VAddress, Vdrawfunction, angles, frames, sprites[]\n\n");
    assert (return_value == 0);
    
    int i;
    
    while((return_value = fscanf(Media, "\"%99[^\"]\", %d, %d, %d, %d",
     &Label[0], &VAddress, &Vdrawfunction, &angles, &frames)) == 5){
     
        for (i = 0; i < angles * frames; i++){
            fscanf(Media, ", %d", &sprites[i]);
        }
        fscanf(Media, "\n");
        //printf("\n\n%s\n\n", Label);    //debug
        bbAnimation_to_vtable(VAddress, Vdrawfunction, angles, frames, sprites);
    
    }
    
    return_value = fscanf(Media, "\n\nEnd:");
    assert (return_value == 0);
    
    printf("Media loaded successfully!\n");
    
    fclose(Media);
    
}


sfSprite* bbAnimation_getSprite(int animation_int, int angle_int, int frame_int){
//TODO test for non-negative inputs
    bbAnimation* animation = bbAnimation_vtable[animation_int];
    int sprite_int = (animation->frames)*angle_int + frame_int;
    int sprite_num = animation->sprites[sprite_int];
    return sfSprite_vtable[sprite_num];
    
}

