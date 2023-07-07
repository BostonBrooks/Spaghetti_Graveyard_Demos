#define BBGROUNDSURFACE

//-----------------------------INCLUDES----------------------------//

#ifndef ALL_INCLUDES
#define ALL_INCLUDES
#include "../headers/all_includes.h"
#endif

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

#ifndef WINDOW
#define WINDOW
#include "../headers/window.h"
#endif

#ifndef INTERPOLATION
#define INTERPOLATION
#include "../headers/interpolation.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

#ifndef MEDIA
#define MEDIA
#include "../headers/media.h"
#endif

#ifndef VIEWPORT
#define VIEWPORT
#include "../headers/viewport.h"
#endif

//-----------------------------CONSTANTS----------------------------//

/*
#define DEFAULT_COLOR_R 104
#define DEFAULT_COLOR_G 151
#define DEFAULT_COLOR_B 100
#define DEFAULT_COLOR_A 255
*/
//-----------------------------STRUCTS------------------------------//

typedef struct {
    int i_Coord; //what terrain square are we looking at?
    int j_Coord;
    
    sfRenderTexture*  Base_Render_Texture;
    sfTexture*        Base_Texture;
    
    
    sfImage*          Hill_Shading_CPU;
    sfUint8*          Hill_Shading_Data;
    sfRenderTexture*  Hill_Shading_Render_Texture;
    sfTexture*        Hill_Shading_Texture;
    
    sfRenderTexture*  Footprints_Render_Texture;
    sfTexture*        Footprints_Texture;
    
    sfRenderTexture*  Auras_Render_Texture;
    sfTexture*        Auras_Texture;
    
    sfRenderTexture*  Circles_Render_Texture;
    sfTexture*        Circles_Texture;
    
    sfVertexArray*    Vertex_Array;
} bbGroundSurface;



//-----------------------------GLOBALS----------------------------//


bbGroundSurface bbGroundSurface_grid[SQUARES_PER_MAP][SQUARES_PER_MAP];


sfRenderStates GroundRenderer; 
sfShader* GroundShader;

sfRenderTexture* null_render_texture;
sfTexture* null_texture;

//-----------------------------FUNCTIONS--------------------------//

int bbGroundSurface_initVertexArray(int square_i, int square_j){

    sfVertexArray* VA = bbGroundSurface_grid[square_i][square_j].Vertex_Array;
    sfVertexArray_setPrimitiveType(VA, sfTriangles);
    
    
    sfVertex topVertex, bottomVertex, leftVertex, rightVertex;
    bbScreenCoords SC;
    
    int i, j;


    for (i = 0; i < TILES_PER_SQUARE; i++){
    
        for (j = TILES_PER_SQUARE - 1; j >= 0; j--){
    
            // calculate vertices
            //x axis is the number of pixels to the right of top left
    //y axis is down number of pixels down from top left
    //i axis points down and to the right, in units of points
    //j axis points up and to the right, in units of points
            
            SC = bbGetVertex(i, j + 1, square_i, square_j);
            
            topVertex.position.x = SC.x;
            topVertex.position.y = SC.y;



            //y -> i, x -> j, as defined in globals.h
            topVertex.texCoords.y = i * PIXELS_PER_TILE;
            topVertex.texCoords.x = (j+1) * PIXELS_PER_TILE;


            SC = bbGetVertex(i + 1, j, square_i, square_j);
            
            bottomVertex.position.x = SC.x;
            bottomVertex.position.y = SC.y;

            bottomVertex.texCoords.y = (i + 1) * PIXELS_PER_TILE;
            bottomVertex.texCoords.x = j * PIXELS_PER_TILE;


            SC = bbGetVertex(i, j, square_i, square_j);
            
            leftVertex.position.x = SC.x;
            leftVertex.position.y = SC.y;

            leftVertex.texCoords.y = i * PIXELS_PER_TILE;
            leftVertex.texCoords.x = j * PIXELS_PER_TILE;


            SC = bbGetVertex(i + 1, j + 1, square_i, square_j);
            
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
// Used for temporary storage of hill shading data, not thread safe!!! //
sfUint8 Hill_Shading_Data[PIXELS_PER_SQUARE * PIXELS_PER_SQUARE * 4];

int bbGroundSurface_calcHillShading(int Square_i, int Square_j){
    
    //#ifdef DEBUG  
    //printf("Executing bbGroundSurface_calcHillShading(%d, %d);\n", Square_i, Square_j);
    //#endif
    
    int Tile_i, Tile_j;
    int i, j;
    
    bbFloat3D LightInc = bbFloat3D_normalise (LightIncedence);

    //#ifdef DEBUG  
    //printf("LightInc (i, j, k) = (%f, %f, %f)\n", LightInc.i, LightInc.j, LightInc.k);
    //#endif
    
    for (Tile_i = 0; Tile_i < TILES_PER_SQUARE; Tile_i++){
    
        for (Tile_j = 0; Tile_j < TILES_PER_SQUARE; Tile_j++){
    
            for (i = 0; i < PIXELS_PER_TILE; i++){
    
                for (j = 0; j < PIXELS_PER_TILE; j++){
    
                    bbMapCoords mc;
                    mc.i = (i + (Tile_i + Square_i * TILES_PER_SQUARE) * PIXELS_PER_TILE)*POINTS_PER_PIXEL;
                    mc.j = (j + (Tile_j + Square_j * TILES_PER_SQUARE) * PIXELS_PER_TILE)*POINTS_PER_PIXEL;
    
                    
                    float Dot_Product;


#ifndef FASTLOAD
                    bbFloat3D normal = lanczos_getNormal(mc);
                    Dot_Product = LightInc.i * normal.i + LightInc.j * normal.j + LightInc.k * normal.k;
 
#else
                    Dot_Product = 0.8;
#endif
                    if (Dot_Product < 0) Dot_Product = 0;
                    
                    
                    
                    int Texture_i = Tile_i * PIXELS_PER_TILE + i;
                    int Texture_j = Tile_j * PIXELS_PER_TILE + j;
                    
                    int Dest_Coord = (Texture_j + PIXELS_PER_SQUARE * Texture_i) * 4;
                    
                    
                    Hill_Shading_Data[Dest_Coord+0] = Dot_Product * 255;
                    Hill_Shading_Data[Dest_Coord+1] = Dot_Product * 255;
                    Hill_Shading_Data[Dest_Coord+2] = Dot_Product * 255;
                    Hill_Shading_Data[Dest_Coord+3] = 255;
                    
                }
    
            }
    
        }
    
    }

    bbGroundSurface_grid[Square_i][Square_j].Hill_Shading_CPU 
         = sfImage_createFromPixels (PIXELS_PER_SQUARE,
             PIXELS_PER_SQUARE, Hill_Shading_Data);
    return 0;
} 

int bbGroundSurface_initAll(void){

    int i, j;
    sfColor Default_Colour = COLOUR_GROUND_NULL;
    
    for (i = 0; i < SQUARES_PER_MAP; i++){
        for (j = 0; j < SQUARES_PER_MAP; j++){
            bbGroundSurface* Ground_Surface = &bbGroundSurface_grid[i][j];
            
            Ground_Surface->i_Coord = i;
            Ground_Surface->j_Coord = j;

            
            Ground_Surface->Base_Render_Texture = 
            sfRenderTexture_create (PIXELS_PER_TILE * TILES_PER_SQUARE, PIXELS_PER_TILE * TILES_PER_SQUARE, sfFalse);             
            sfRenderTexture_clear(Ground_Surface->Base_Render_Texture, Default_Colour);
            Ground_Surface->Base_Texture = sfRenderTexture_getTexture(Ground_Surface->Base_Render_Texture);
            
            Ground_Surface->Hill_Shading_Render_Texture = 
            sfRenderTexture_create (PIXELS_PER_TILE * TILES_PER_SQUARE, PIXELS_PER_TILE * TILES_PER_SQUARE, sfFalse);             
            sfRenderTexture_clear(Ground_Surface->Hill_Shading_Render_Texture, Default_Colour);
            Ground_Surface->Hill_Shading_Texture = sfRenderTexture_getTexture(Ground_Surface->Hill_Shading_Render_Texture);
            
            Ground_Surface->Footprints_Render_Texture = 
            sfRenderTexture_create (PIXELS_PER_TILE * TILES_PER_SQUARE, PIXELS_PER_TILE * TILES_PER_SQUARE, sfFalse);             
            sfRenderTexture_clear(Ground_Surface->Footprints_Render_Texture, Default_Colour);
            Ground_Surface->Footprints_Texture = sfRenderTexture_getTexture(Ground_Surface->Footprints_Render_Texture);
            
            Ground_Surface->Auras_Render_Texture = 
            sfRenderTexture_create (PIXELS_PER_TILE * TILES_PER_SQUARE, PIXELS_PER_TILE * TILES_PER_SQUARE, sfFalse);             
            sfRenderTexture_clear(Ground_Surface->Auras_Render_Texture, Default_Colour);
            Ground_Surface->Auras_Texture = sfRenderTexture_getTexture(Ground_Surface->Auras_Render_Texture);
            
            Ground_Surface->Circles_Render_Texture = 
            sfRenderTexture_create (PIXELS_PER_TILE * TILES_PER_SQUARE, PIXELS_PER_TILE * TILES_PER_SQUARE, sfFalse);             
            sfRenderTexture_clear(Ground_Surface->Circles_Render_Texture, Default_Colour);
            Ground_Surface->Circles_Texture = sfRenderTexture_getTexture(Ground_Surface->Circles_Render_Texture);
            
                
    sfRenderTexture* Base = Ground_Surface->Base_Render_Texture;
    //sfRenderTexture* Footprints = Ground_Surface->Footprints_Render_Texture;
    //sfRenderTexture* Auras = Ground_Surface->Auras_Render_Texture;
    
    sfVector2f position;
    
    position.x = 00;    position.y = 00;
    //sfSprite_setPosition(sfSprite_vtable[35], position);
    //sfRenderTexture_drawSprite(Auras, sfSprite_vtable[35], NULL);
    //sfRenderTexture_display(Auras);
    
    sfSprite_setPosition(sfSprite_vtable[37], position);
    sfRenderTexture_drawSprite(Base, sfSprite_vtable[37], NULL);
    //sfRenderTexture_drawSprite(Base, sfSprite_vtable[36], NULL);
    sfRenderTexture_display(Base);
    

    
    
    sfRenderTexture_display(bbGroundSurface_grid[0][0].Base_Render_Texture); 
                
            //#ifdef DEBUG  
            //printf("Entering bbGroundSurface_calcHillShading(%d, %d);\n", i, j);
            //#endif    
                
            bbGroundSurface_calcHillShading(i, j);
            

                
            sfIntRect area;
            area.left = 0;
            area.top = 0;
            area.width = PIXELS_PER_SQUARE;
            area.height = PIXELS_PER_SQUARE;
            
            
            sfTexture* temp_hill_shading;
            sfSprite* temp_hill_shading_sprite;
                
            temp_hill_shading = sfTexture_createFromImage(Ground_Surface->Hill_Shading_CPU, &area);
            temp_hill_shading_sprite = sfSprite_create();
            sfSprite_setTexture(temp_hill_shading_sprite,temp_hill_shading,sfTrue);
            sfRenderTexture_drawSprite(Ground_Surface->Hill_Shading_Render_Texture, temp_hill_shading_sprite, NULL);
            sfRenderTexture_display(Ground_Surface->Hill_Shading_Render_Texture);
            
            sfSprite_destroy(temp_hill_shading_sprite);
            sfTexture_destroy(temp_hill_shading);
            

            
            Ground_Surface->Vertex_Array = sfVertexArray_create();
            
            bbGroundSurface_initVertexArray(i, j);
            
    
        }
    
    }

}

//TODO mix hill shading with footprints then multiply base?

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
        uniform sampler2D Footprints_Texture;\
        uniform sampler2D Auras_Texture;\
        uniform sampler2D Circles_Texture;\
        \
        void main()\
        {\
            vec4 base = texture2D(Base_Texture, gl_TexCoord[0].xy);\
            vec4 hill_shading = texture2D(Hill_Shading_Texture, gl_TexCoord[0].xy);\
            vec4 footprints = texture2D(Footprints_Texture, gl_TexCoord[0].xy);\
            vec4 auras = texture2D(Auras_Texture, gl_TexCoord[0].xy);\
            vec4 circles = texture2D(Circles_Texture, gl_TexCoord[0].xy);\
        \
            vec4 mix1 = base * hill_shading;\
            vec4 mix2 = mix(mix1, footprints, footprints.a);\
            vec4 mix3 = mix(mix2, auras, auras.a);\
            vec4 mix4 = mix(mix3, circles, circles.a);\
            gl_FragColor = mix4;\
        }\
    ";

//            gl_FragColor =  gl_Color * Light * pixel;\

    null_render_texture = sfRenderTexture_create (PIXELS_PER_TILE * TILES_PER_SQUARE, PIXELS_PER_TILE * TILES_PER_SQUARE, sfFalse);
    sfRenderTexture_drawSprite(null_render_texture, sfSprite_vtable[12], NULL);
    sfRenderTexture_display(null_render_texture);
    null_texture = sfRenderTexture_getTexture(null_render_texture);
    
    
    
    GroundShader = sfShader_createFromMemory(vertShader, NULL, fragShader);
    printf("ground shader problem?\n");
    assert (GroundShader != NULL);
    GroundRenderer.shader = GroundShader;
    GroundRenderer.blendMode = sfBlendAlpha;
    GroundRenderer.transform = sfTransform_Identity;
    GroundRenderer.texture = null_texture;
    
    printf("No ground shader problem.\n");
    


}


int bbGroundSurface_draw(int Square_i, int Square_j){

    bbGroundSurface* Ground_Surface = &bbGroundSurface_grid[Square_i][Square_j];
    
    //sfRenderTexture_display(Ground_Surface->Base_Texture);
    //sfRenderTexture_display(Ground_Surface->Hill_Shading_Texture);
    //sfRenderTexture_display(Ground_Surface->Footprints_Texture);
    //sfRenderTexture_display(Ground_Surface->Auras_Texture);
    //sfRenderTexture_display(Ground_Surface->Circles_Texture);
    
    bbTileCoords TC;
    TC.i = Square_i * TILES_PER_SQUARE;
    TC.j = Square_j * TILES_PER_SQUARE;
    TC.k = 0;

    bbScreenCoords left_corner;
    left_corner = bbTileCoords_getScreenCoords_centre(TC);
    sfVector2f sf_left_corner;
    sf_left_corner.x = left_corner.x;
    sf_left_corner.y = left_corner.y;
    
    sfVertexArray* VA = Ground_Surface->Vertex_Array;
    
    sfShader_setVec2Uniform (GroundShader, "offset", sf_left_corner);
    
    
    sfShader_setTextureUniform(GroundShader,"Base_Texture", Ground_Surface->Base_Texture);
    sfShader_setTextureUniform(GroundShader,"Hill_Shading_Texture", Ground_Surface->Hill_Shading_Texture);
    sfShader_setTextureUniform(GroundShader,"Footprints_Texture", Ground_Surface->Footprints_Texture);
    sfShader_setTextureUniform(GroundShader,"Auras_Texture", Ground_Surface->Auras_Texture);
    sfShader_setTextureUniform(GroundShader,"Circles_Texture", Ground_Surface->Circles_Texture);
    
    sfRenderTexture_drawVertexArray (bbViewport_ground, VA, &GroundRenderer);
    
    sfRenderTexture_clear(Ground_Surface->Footprints_Render_Texture, sfTransparent);
    sfRenderTexture_clear(Ground_Surface->Auras_Render_Texture, sfTransparent);
    sfRenderTexture_clear(Ground_Surface->Circles_Render_Texture, sfTransparent);
    
    
}
    

int bbGroundSurface_drawVisible(){
    
    bbScreenCoords sc;
    bbMapCoords TopLeft, TopRight, BottomLeft, BottomRight;
    
    //not sure if I should add VIEWPORT_TOP and VIEWPORT_LEFT 
    //Im going to say no
    sc.x = 0;
    sc.y = -viewpoint.k;
        
    TopLeft = bbScreenCoords_getMapCoords_k0(sc);
    
    sc.x = VIEWPORT_WIDTH;
    sc.y = -viewpoint.k;
        
    TopRight = bbScreenCoords_getMapCoords_k0(sc);
    
    sc.x = 0;
    sc.y = VIEWPORT_HEIGHT + ELEVATION_MAX -viewpoint.k;
        
    BottomLeft = bbScreenCoords_getMapCoords_k0(sc);
    
    sc.x = VIEWPORT_WIDTH;
    sc.y = VIEWPORT_HEIGHT + ELEVATION_MAX -viewpoint.k;
        
    BottomRight = bbScreenCoords_getMapCoords_k0(sc);
    
    
    bbSquareCoords LeftCorner;
    bbSquareCoords RightCorner;
    
    LeftCorner.i = floordiv(TopLeft.i,POINTS_PER_SQUARE);
    LeftCorner.j = floordiv(BottomLeft.j,POINTS_PER_SQUARE);
    
    if (LeftCorner.i < 0) LeftCorner.i = 0;
    if (LeftCorner.j < 0) LeftCorner.j = 0;
    
    RightCorner.i = floordiv(BottomRight.i,POINTS_PER_SQUARE);
    RightCorner.j = floordiv(TopRight.j,POINTS_PER_SQUARE);
    
    if (RightCorner.i > SQUARES_PER_MAP-1) RightCorner.i = SQUARES_PER_MAP-1;
    if (RightCorner.j > SQUARES_PER_MAP-1) RightCorner.j = SQUARES_PER_MAP-1;
    //indexing starts at zero
    
    int m,n;
    
    for (n = RightCorner.j; n >= LeftCorner.j; n--){
        for (m = LeftCorner.i; m <= RightCorner.i; m++){
            bbGroundSurface_draw(m,n);
        }
    }
}
