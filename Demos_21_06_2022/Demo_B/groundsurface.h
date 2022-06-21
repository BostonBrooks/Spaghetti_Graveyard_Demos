#define GROUNDSURFACE

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

#ifndef GEOMETRY
#include "geometry.h"
#endif

//TODO window contains sfRender_Texture ViewPort;
#ifndef WINDOW
#include "window.h"
#endif


//-----------------------------CONSTANTS----------------------------//

#ifndef CONSTANTS
#include "constants.h"
#endif
/*
#define DEFAULT_COLOR_R 104
#define DEFAULT_COLOR_G 151
#define DEFAULT_COLOR_B 100
#define DEFAULT_COLOR_A 255
*/
//-----------------------------STRUCTS------------------------------//

typedef struct {
    int i_Coord;
    int j_Coord;
    
    sfRenderTexture*  Base_Render_Texture;
    sfTexture*        Base_Texture;
    
    
    sfImage*          Hill_Shading_CPU;
    sfUint8*          Hill_Shading_Data;
    //sfRenderTexture*  Hill_Shading_Render_Texture;
    sfTexture*        Hill_Shading_Texture;
    
    sfRenderTexture*  Shadows_Render_Texture;
    sfTexture*        Shadows_Texture;
    
    sfRenderTexture*  Auras_Render_Texture;
    sfTexture*        Auras_Texture;
    
    sfVertexArray*    Vertex_Array;
} Square_Ground_Surface;



//-----------------------------GLOBALS----------------------------//

Square_Ground_Surface Square_Ground_Surfaces[SQUARES_PER_MAP][SQUARES_PER_MAP];


sfRenderStates GroundRenderer; 
sfShader* GroundShader;



//-----------------------------FUNCTIONS--------------------------//

int Init_Vertex_Array(int square_i, int square_j){

    sfVertexArray* VA = Square_Ground_Surfaces[square_i][square_j].Vertex_Array;
    sfVertexArray_setPrimitiveType(VA, sfTriangles);
    
    
    sfVertex topVertex, bottomVertex, leftVertex, rightVertex;
    Screen_Coords SC;
    
    int i, j;


    for (i = 0; i < TILES_PER_SQUARE; i++){
    
        for (j = TILES_PER_SQUARE - 1; j >= 0; j--){
    
            // calculate vertices
            //x axis is the number of pixels to the right of top left
    //y axis is down number of pixels down from top left
    //i axis points down and to the right, in units of points
    //j axis points up and to the right, in units of points
            
            SC = subtile_to_screen_centre_square(i, j + 1, square_i, square_j);
            
            topVertex.position.x = SC.x;
            topVertex.position.y = SC.y;



            //y -> i, x -> j, as defined in globals.h
            topVertex.texCoords.y = i * PIXELS_PER_TILE;
            topVertex.texCoords.x = (j+1) * PIXELS_PER_TILE;


            SC = subtile_to_screen_centre_square(i + 1, j, square_i, square_j);
            
            bottomVertex.position.x = SC.x;
            bottomVertex.position.y = SC.y;

            bottomVertex.texCoords.y = (i + 1) * PIXELS_PER_TILE;
            bottomVertex.texCoords.x = j * PIXELS_PER_TILE;


            SC = subtile_to_screen_centre_square(i, j, square_i, square_j);
            
            leftVertex.position.x = SC.x;
            leftVertex.position.y = SC.y;

            leftVertex.texCoords.y = i * PIXELS_PER_TILE;
            leftVertex.texCoords.x = j * PIXELS_PER_TILE;


            SC = subtile_to_screen_centre_square(i + 1, j + 1, square_i, square_j);
            
            rightVertex.position.x = SC.x;
            rightVertex.position.y = SC.y;

            rightVertex.texCoords.y = (i + 1) * PIXELS_PER_TILE;
            rightVertex.texCoords.x = (j + 1) * PIXELS_PER_TILE;
    
                        
            topVertex.color = sfWhite;
            bottomVertex.color = sfWhite;
            leftVertex.color = sfWhite;
            rightVertex.color = sfWhite;


            sfVertexArray_append(VA, topVertex);
            sfVertexArray_append(VA, rightVertex);
            sfVertexArray_append(VA, leftVertex);

            sfVertexArray_append(VA, leftVertex);
            sfVertexArray_append(VA, rightVertex);
            sfVertexArray_append(VA, bottomVertex);
                
        }
    }

}


int Calc_Hill_Shading(int Square_i, int Square_j){

    int Tile_i, Tile_j;
    int i, j;
    
    Float_3d LightInc = Normalise_Float_3d (LightIncedence);
    
//printf("lightInc (i, j, k) = (%f, %f, %f)\n", LightInc.i, LightInc.j, LightInc.k);
    
    sfUint8 Hill_Shading_Data[PIXELS_PER_SQUARE * PIXELS_PER_SQUARE * 4];
    
    for (Tile_i = 0; Tile_i < TILES_PER_SQUARE; Tile_i++){
    
        for (Tile_j = 0; Tile_j < TILES_PER_SQUARE; Tile_j++){
    
            for (i = 0; i < PIXELS_PER_TILE; i++){
    
                for (j = 0; j < PIXELS_PER_TILE; j++){
    
                    Map_Coords mc;
                    mc.i = (i + (Tile_i + Square_i * TILES_PER_SQUARE) * PIXELS_PER_TILE)*POINTS_PER_PIXEL;
                    mc.j = (j + (Tile_j + Square_j * TILES_PER_SQUARE) * PIXELS_PER_TILE)*POINTS_PER_PIXEL;
                    
                    ////shade based on interpolated slope
                    Float_3d normal = map_coords_get_normal_bicubic(mc);
                    float Dot_Product = LightInc.i * normal.i + LightInc.j * normal.j + LightInc.k * normal.k;
                    if (Dot_Product < 0) Dot_Product = 0;
                    
                    
                    ////shade based on elevation
                    //float Dot_Product = map_coords_get_elevation_bicubic(mc) / (1.0 * ELEVATION_MAX);
                    //if (Dot_Product < 0) Dot_Product = 0;
                    //if (Dot_Product > 1) Dot_Product = 1;
                    
                    
                    ////One colour per tile
                    //float Dot_Product = tile_coords_get_elevation(Tile_i + Square_i * TILES_PER_SQUARE, Tile_j + Square_j * TILES_PER_SQUARE) / (1.0 * ELEVATION_MAX);

                    
                    
                    
                    int Texture_i = Tile_i * PIXELS_PER_TILE + i;
                    int Texture_j = Tile_j * PIXELS_PER_TILE + j;
                    
                    int Dest_Coord = (Texture_j + PIXELS_PER_SQUARE * Texture_i) * 4;
                    
                    //printf("DC = %d\n", Dest_Coord);
                    
                    Hill_Shading_Data[Dest_Coord+0] = Dot_Product * 255;
                    Hill_Shading_Data[Dest_Coord+1] = Dot_Product * 255;
                    Hill_Shading_Data[Dest_Coord+2] = Dot_Product * 255;
                    Hill_Shading_Data[Dest_Coord+3] = 255;
                    
                    
                    //printf("HSD = %d\n", Hill_Shading_Data[Dest_Coord+0]);
                }
    
            }
    
        }
    
    }

    Square_Ground_Surfaces[Square_i][Square_j].Hill_Shading_CPU 
         = sfImage_createFromPixels (PIXELS_PER_SQUARE,
             PIXELS_PER_SQUARE, Hill_Shading_Data);

} 

int Init_Ground_Surfaces(void){

    int i, j;
    sfColor Default_Color;
    
    for (i = 0; i < SQUARES_PER_MAP; i++){
        for (j = 0; j < SQUARES_PER_MAP; j++){
            Square_Ground_Surface* Ground_Surface = &Square_Ground_Surfaces[i][j];
            
            Ground_Surface->i_Coord = i;
            Ground_Surface->j_Coord = j;
            
            Ground_Surface->Base_Render_Texture = 
                sfRenderTexture_create (PIXELS_PER_TILE * TILES_PER_SQUARE, PIXELS_PER_TILE * TILES_PER_SQUARE, sfFalse);
                
            Default_Color.r = DEFAULT_COLOR_R;
            Default_Color.g = DEFAULT_COLOR_G;
            Default_Color.b = DEFAULT_COLOR_B;
            Default_Color.a = DEFAULT_COLOR_A;
            
            sfRenderTexture_clear(Ground_Surface->Base_Render_Texture, Default_Color);
                
            Ground_Surface->Base_Texture = Textures[11];
       

            
                
            Calc_Hill_Shading(i, j);
             

            //sfRenderTexture_clear(Ground_Surface->Hill_Shading_Render_Texture, sfWhite);
                
            sfIntRect area;
            area.left = 0;
            area.top = 0;
            area.width = PIXELS_PER_SQUARE;
            area.height = PIXELS_PER_SQUARE;
                
            Ground_Surface->Hill_Shading_Texture
              = sfTexture_createFromImage(Ground_Surface->Hill_Shading_CPU, &area);
               // sfRenderTexture_getTexture(Ground_Surface->Hill_Shading_Render_Texture);
               
   //
            Ground_Surface->Base_Render_Texture = 
                sfRenderTexture_create (PIXELS_PER_TILE * TILES_PER_SQUARE, PIXELS_PER_TILE * TILES_PER_SQUARE, sfFalse);
                
            sfRenderTexture_clear(Ground_Surface->Base_Render_Texture, sfGreen);
                
            Ground_Surface->Base_Texture =
                sfRenderTexture_getTexture(Ground_Surface->Base_Render_Texture);   
   
   //            
            
            
            Ground_Surface->Shadows_Render_Texture = 
                sfRenderTexture_create (PIXELS_PER_TILE * TILES_PER_SQUARE, PIXELS_PER_TILE * TILES_PER_SQUARE, sfFalse);
                
            sfRenderTexture_clear(Ground_Surface->Shadows_Render_Texture, sfTransparent);
                
            Ground_Surface->Shadows_Texture =
                sfRenderTexture_getTexture(Ground_Surface->Shadows_Render_Texture);
            
            
            Ground_Surface->Auras_Render_Texture = 
                sfRenderTexture_create (PIXELS_PER_TILE * TILES_PER_SQUARE, PIXELS_PER_TILE * TILES_PER_SQUARE, sfFalse);
                
            sfRenderTexture_clear(Ground_Surface->Auras_Render_Texture, sfTransparent);
                
            
            Ground_Surface->Auras_Texture =
                sfRenderTexture_getTexture(Ground_Surface->Auras_Render_Texture);
            
            Ground_Surface->Vertex_Array = sfVertexArray_create();
            
            Init_Vertex_Array(i, j);
            
    
        }
    
    }

}


int Create_Ground_Shaders (void){
//vertex_shader
    char vertShader[] = "\
        uniform vec2 offset;\
        void main()\
        {\
         \
            vec4 offsetvertex = vec4(offset.x, offset.y, 0, 0);\
            gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex + offsetvertex);\
         \
            gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;\
         \
           gl_FrontColor = gl_Color;\
        }\
    ";
    
    //Fragment Shader
    char fragShader[] = "\
        uniform sampler2D Base_Texture;\
        uniform sampler2D Hill_Shading_Texture;\
        uniform sampler2D Shadows_Texture;\
        uniform sampler2D Auras_Texture;\
        \
        void main()\
        {\
            vec4 base = texture2D(Base_Texture, gl_TexCoord[0].xy);\
            vec4 hill_shading = texture2D(Hill_Shading_Texture, gl_TexCoord[0].xy);\
            vec4 shadows = texture2D(Shadows_Texture, gl_TexCoord[0].xy);\
            vec4 auras = texture2D(Auras_Texture, gl_TexCoord[0].xy);\
        \
            vec4 mix1 = base * hill_shading;\
            vec4 mix2 = mix(mix1, shadows, shadows.a);\
            vec4 mix3 = mix(mix2, auras, auras.a);\
            gl_FragColor = mix3;\
        }\
    ";

//            gl_FragColor =  gl_Color * Light * pixel;\

    
    GroundShader = sfShader_createFromMemory(vertShader, NULL, fragShader);
    assert (GroundShader != NULL);
    GroundRenderer.shader = GroundShader;
    GroundRenderer.blendMode = sfBlendAlpha;
    GroundRenderer.transform = sfTransform_Identity;
    GroundRenderer.texture = Textures[12];


}


int Draw_Ground_Square(int Square_i, int Square_j){

    Square_Ground_Surface* Ground_Surface = &Square_Ground_Surfaces[Square_i][Square_j];
    Tile_Coords TC;
    TC.i = Square_i * TILES_PER_SQUARE;
    TC.j = Square_j * TILES_PER_SQUARE;
    TC.k = 0;

    Screen_Coords left_corner;
    left_corner = tile_to_screen_centre_viewpoint(TC);
    sfVector2f sf_left_corner;
    sf_left_corner.x = left_corner.x;
    sf_left_corner.y = left_corner.y;
    
    sfVertexArray* VA = Ground_Surface->Vertex_Array;
    
    sfShader_setVec2Uniform (GroundShader, "offset", sf_left_corner);
    
    sfShader_setTextureUniform(GroundShader,"Base_Texture", Ground_Surface->Base_Texture);
    sfShader_setTextureUniform(GroundShader,"Hill_Shading_Texture", Ground_Surface->Hill_Shading_Texture);
    sfShader_setTextureUniform(GroundShader,"Shadows_Texture", Ground_Surface->Shadows_Texture);
    sfShader_setTextureUniform(GroundShader,"Auras_Texture", Ground_Surface->Auras_Texture);
    
    sfRenderTexture_drawVertexArray (viewport, VA, &GroundRenderer);
    sfRenderTexture_drawVertexArray (viewport_ground, VA, &GroundRenderer);
    //one or the other of the above lines is needed.
    
}
    
    
    
