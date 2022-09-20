#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>


int main(void){

    sfRenderWindow* window;
    
    sfTexture* null_texture;
    sfTexture* raw_texture;
    sfTexture* rendered_texture;
    
    sfSprite* raw_sprite;
    sfSprite* rendered_sprite;
    
    sfRenderTexture* render_texture;
    
    
    null_texture = sfTexture_createFromFile("Null.png", NULL);
    raw_texture = sfTexture_createFromFile("Texture.png", NULL);
    
    raw_sprite = sfSprite_create();
    sfSprite_setTexture(raw_sprite, raw_texture, sfTrue);
    
    render_texture = sfRenderTexture_create(512, 512, sfFalse);
    sfRenderTexture_drawSprite(render_texture, raw_sprite, NULL);
    sfRenderTexture_display(render_texture);
    
    rendered_texture = sfRenderTexture_getTexture(render_texture);
    
    rendered_sprite = sfSprite_create();
    sfSprite_setTexture(rendered_sprite, rendered_texture, sfTrue);
    
    sfVector2f position;
    position.x = 512;
    position.y = 0;
    
    sfSprite_setPosition(rendered_sprite, position);
    
    
    
    sfVideoMode mode = {1024, 1024, 32};
    window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);
    sfRenderWindow_clear(window, sfWhite);
    
    sfRenderWindow_drawSprite(window, raw_sprite, NULL);
    sfRenderWindow_drawSprite(window, rendered_sprite, NULL);
    
    sfRenderStates Renderer;
    sfShader* Shader;
    
    
    char vertShader[] = "\
        uniform vec2 position;\
        void main()\
        {\
         \
            vec4 offsetvertex = vec4(position.x, position.y, 0, 0);\
            gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + offsetvertex);\
         \
            gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\
         \
           gl_FrontColor = gl_Color;\
        }\
    ";
    
    //Fragment Shader
    char fragShader[] = "\
        uniform sampler2D Texture;\
        \
        void main()\
        {\
            gl_FragColor = texture2D(Texture, gl_TexCoord[0].xy);\
        }\
    ";

//            gl_FragColor =  gl_Color * Light * pixel;\

    
    Shader = sfShader_createFromMemory(vertShader, NULL, fragShader);
    Renderer.shader = Shader;
    Renderer.blendMode = sfBlendAlpha;
    Renderer.transform = sfTransform_Identity;
    Renderer.texture = null_texture;
    
    sfVertexArray*    Vertex_Array;
    Vertex_Array = sfVertexArray_create();
    sfVertexArray_setPrimitiveType(Vertex_Array, sfTriangles);
    
    sfVertex topVertex, bottomVertex, leftVertex, rightVertex;
    
    topVertex.position.x = 256;
    topVertex.position.y = -256;
    topVertex.texCoords.x = 512;
    topVertex.texCoords.y = 0;
    
    bottomVertex.position.x = 256;
    bottomVertex.position.y = 256;
    bottomVertex.texCoords.x = 0;
    bottomVertex.texCoords.y = 512;
    
    leftVertex.position.x = 0;
    leftVertex.position.y = 0;
    leftVertex.texCoords.x = 0;
    leftVertex.texCoords.y = 0;
    
    rightVertex.position.x = 512;
    rightVertex.position.y = 0;
    rightVertex.texCoords.x = 512;
    rightVertex.texCoords.y = 512;

    sfVertexArray_append(Vertex_Array, topVertex);
    sfVertexArray_append(Vertex_Array, rightVertex);
    sfVertexArray_append(Vertex_Array, leftVertex);

    sfVertexArray_append(Vertex_Array, leftVertex);
    sfVertexArray_append(Vertex_Array, rightVertex);
    sfVertexArray_append(Vertex_Array, bottomVertex);
    
    position.x = 0;
    position.y = 768;
    
    sfShader_setVec2Uniform(Shader, "position", position);
    sfShader_setTextureUniform(Shader, "Texture", raw_texture);
    
    sfRenderWindow_drawVertexArray (window, Vertex_Array, &Renderer);
    
    
    position.x = 512;
    position.y = 768;
    
    sfShader_setVec2Uniform(Shader, "position", position);
    sfShader_setTextureUniform(Shader, "Texture", rendered_texture);
    
    sfRenderWindow_drawVertexArray (window, Vertex_Array, &Renderer);
    
    
    sfRenderWindow_display(window);
    sfSleep(sfSeconds(60));

}
