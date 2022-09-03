#define MAIN

//-----------------------------SYSTEM FILES----------------------------//

#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

int main(void){

    sfTexture* Blank_Texture;
    sfTexture* Raw_Texture;
    sfTexture* Rendered_Texture;
    sfSprite* Sprite;
    sfSprite* Sprite2;
    sfRenderTexture* Render_Texture;
    sfVertexArray* Vertex_Array;
    sfRenderStates Render_States;
    sfShader* Shader;
    sfRenderWindow* Window;
    
    sfVideoMode Video_Mode = {1200, 600, 32};
    Window = sfRenderWindow_create (Video_Mode, "RenderTexture Test", sfResize | sfClose, NULL);
    sfRenderWindow_clear(Window, sfWhite);
    sfRenderWindow_display(Window);
    
    Blank_Texture = sfTexture_createFromFile("./NULL.bmp", NULL);
    Raw_Texture = sfTexture_createFromFile("./Texture.bmp", NULL);
    Sprite = sfSprite_create();
    sfSprite_setTexture(Sprite, Raw_Texture, sfFalse);
    
    
    sfRenderWindow_drawSprite(Window, Sprite, NULL);
    
    
    
    Render_Texture = sfRenderTexture_create(512, 512, sfFalse);
    sfRenderTexture_drawSprite(Render_Texture, Sprite, NULL);
    
    Rendered_Texture = sfRenderTexture_getTexture(Render_Texture);
    
    Sprite2 = sfSprite_create();
    sfSprite_setTexture(Sprite2, Rendered_Texture, sfFalse);
    sfVector2f Position;
    Position.x = 512;
    Position.y = 0;
    
    sfSprite_setPosition(Sprite2, Position);
    
    sfRenderWindow_drawSprite(Window, Sprite2, NULL);
    sfRenderWindow_display(Window);
    
    sfSleep(sfSeconds(60));
}
