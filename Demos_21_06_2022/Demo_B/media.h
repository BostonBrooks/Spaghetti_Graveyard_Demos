#define MEDIA

//-----------------------------INCLUDES----------------------------//
/*
#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

*/

//-----------------------------CONSTANTS----------------------------//

//#ifndef CONSTANTS
//#include "constants.h"
//#endif



//-----------------------------STRUCURES------------------------------//

typedef struct {

int Vdrawfunction;
int angles;
int frames;
int sprites[];

} Animation;

//-----------------------------GLOBALS----------------------------//

sfTexture** Textures;
sfSprite** Sprites;
Animation** Animations;

//-----------------------------FUNCTIONS--------------------------//

int Declare_Textures(int number) {
    Textures = (sfTexture**) malloc (sizeof(sfTexture*) * number);
}

int Declare_Sprites(int number) {
    Sprites = (sfSprite**) malloc (sizeof(sfSprite*) * number);
}

int Declare_Animations(int number) {
    Animations = (Animation**) malloc (sizeof(Animation*) * number);
}

int Create_Texture(int VAddress, char File[], sfBool smooth){

    sfTexture* New_Texture = sfTexture_createFromFile(File, NULL);
    
    sfTexture_setSmooth(New_Texture, smooth);
    
    Textures[VAddress] = New_Texture;
}

int Create_Sprite(int VAddress, int TextureVAddress, int left, int top, int width,
    int height, float originx, float originy, float scalex, float scaley){

    sfSprite* New_Sprite = sfSprite_create();
    
    sfSprite_setTexture(New_Sprite, Textures[TextureVAddress], sfFalse);
    
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
    
    Sprites[VAddress] = New_Sprite;
   
}

int Create_Animation(int VAddress, int Vdrawfunction, int angles, int frames, int sprites[]){

    Animation* animation = malloc (sizeof(Animation) + sizeof(int) * angles * frames);
    
    animation->Vdrawfunction = Vdrawfunction;
    animation->angles = angles;
    animation->frames = frames;
    
    int i;
    
    for (i = 0; i < angles*frames; i++){
    
        animation->sprites[i] = sprites[i];
    
    }

    Animations[VAddress] = animation; 

}

int Load_Media(){

// Input file

    FILE* Media = fopen("media.txt", "r");
    assert(Media != NULL);
    
    int return_value;
    int number;
    
// Textures
    
    return_value = fscanf(Media, "sfTexture:\n\n");
    assert (return_value == 0);
    
    return_value = fscanf(Media, "Number of Textures: %d\n\n", &number);
    assert (return_value == 1);
    
    Declare_Textures(number);
    
    return_value = fscanf(Media, "VAddress, File, Smooth:\n\n");
    assert (return_value == 0);
    
    int VAddress;
    char File_Name[100];
    char Smooth;
    
    
    while((return_value = fscanf(Media, "%d, \"%99[^\"]\", %c\n", &VAddress, &File_Name[0], &Smooth)) == 3){
        
        
        //printf("\n\n%s\n\n", File_Name);    //debug
        
        if (Smooth == 'T') {
            Create_Texture(VAddress, File_Name, sfTrue);
        } else  {
            Create_Texture(VAddress, File_Name, sfFalse);
        }
         
    }

// Sprites

    return_value = fscanf(Media, "\nsfSprite:\n\n");
    assert (return_value == 0);
    
    
    return_value = fscanf(Media, "Number of Sprites: %d\n\n", &number);
    assert (return_value == 1);
    
    Declare_Sprites(number);
    
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
        
        Create_Sprite(VAddress, TextureVAddress, left, top, width, height,
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
    
    //printf("\n\n%d\n\n", number);    //debug
    
    assert (return_value == 1);
    
    Declare_Animations(number);
    
    return_value = fscanf(Media, "label, VAddress, Vdrawfunction angles, frames, sprites[]\n\n");
    assert (return_value == 0);
    
    int i;
    
    
    while((return_value = fscanf(Media, "\"%99[^\"]\",%d, %d, %d, %d",
     &Label[0], &VAddress, &Vdrawfunction, &angles, &frames)) == 4){
     
        for (i = 0; i < angles * frames; i++){
            fscanf(Media, ", %d", &sprites[i]);
        }
        
        Create_Animation(VAddress, Vdrawfunction, angles, frames, sprites);
        //Create_Animation(int VAddress, int Vdrawfunction, int angles, int frames, int sprites[])
    
    }
    
    return_value = fscanf(Media, "\n\nEnd:");
    assert (return_value == 0);
    
    printf("Media loaded successfully!\n");
    
    fclose(Media);
    
}
