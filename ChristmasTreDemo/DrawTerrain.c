//-----------------------------INCLUDES----------------------------//

#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

//-----------------------------CONSTANTS----------------------------//

#define TEXTURESQUARESIZE    512   //Each Medium Square relates to 256 by 256 pixel square of texture data.
#define MAPSQUARESIZE        512     //Each Medium Square relates to a 256 by 256 point square on the map.
#define SUBDIVISIONS         32        //Medium square is made up of an 8 by 8 array of triangles.
#define MAPDIMSQUARES        8     //Size of the map in terms of terrain squares.
#define TEXTURETILESIZE      (TEXTURESQUARESIZE/SUBDIVISIONS)
#define MAPTILESIZE          (MAPSQUARESIZE/SUBDIVISIONS)
#define SCREEN_HEIGHT        600
#define SCREEN_WIDTH         800

#define XSCALE               /1
#define YSCALE               /2
#define ZSCALE               /1

#define XSCALE_INV           *1
#define YSCALE_INV           *2
#define ZSCALE_INV           *1

#define ELEVATIONSCALE       *2
#define ELEVATIONRANGE       1024

#define BORDER               0

//-----------------------------STRUCTURES----------------------------//



typedef sfVector2i XY_Point; //Point on screen. x axis is right, y axis is down, from the top left. 
typedef sfVector2f XY_Float;

typedef struct {
    int i;          //i axis is down and to the right
    int j;          //j axis is up and to the right
    int k;          //k axis is up
} IJK_Point;

typedef struct {
    float i;          //i axis is down and to the right
    float j;          //j axis is up and to the right
    float k;          //k axis is up
} IJK_Float;

typedef struct {
    void* previous;
    void* next;
} LinkedList;

typedef struct {
    LinkedList global;
    LinkedList local;
    LinkedList visible;
    IJK_Point location;
} ForestTree;

typedef enum {
    matte,
    luminous,
} SlideType;

typedef struct {
    SlideType type;
    int angles;
    int frames;
    int framerate;
    sfSprite** sprite;
} Slide;

typedef struct {
    LinkedList global;
    LinkedList local;
    LinkedList visible;
    IJK_Point location;

    Slide* slide[8];
    int angle[8];
    int frame[8];
} Drawable;

typedef struct {
    XY_Point Location;        //location of Terrain Square on map.
                     // Ie leftmost square is at (0,0),
                     // the first TS down and to the right is at (1,0)
                     //the first TS up and to the right is at (0,1)
    int ElevationData[SUBDIVISIONS+3][SUBDIVISIONS+3];
                     //made up of SUBDIVISIONS*SUBDIVISIONS grid of upper and lower triangles.
                     //Meaning a SUBDIVISIONS+1*SUBDIVISIONS+1 grid of vertices.
                     //Plus a 1 vertex buffer of vertices used to calculate terrain shaddows.
    sfTexture* GroundSurface;
    sfImage* CPULightMap;
    sfUint8* LightMapPtr;
    sfTexture* GPULightMap;
    sfVertexArray* GPU_Elevations;
    Drawable* Drawable_Head;
    Drawable* Drawable_Tail;

} TerrainSquare;




//-----------------------------GLOBALS----------------------------//


Drawable* Drawable_Head = NULL;
Drawable* Drawable_Tail = NULL;

Drawable* VisibleDrawable_Head = NULL;
Drawable* VisibleDrawable_Tail = NULL;


TerrainSquare TerrainSquares[MAPDIMSQUARES][MAPDIMSQUARES];

IJK_Point ViewPoint;

sfImage* HeightMap;
sfUint8* HeightMapPtr;
sfVector2u HeightMapSize;

sfVideoMode mode = {SCREEN_WIDTH + 2*BORDER, SCREEN_HEIGHT+2*BORDER, 32};
sfRenderWindow* window;

sfRenderTexture* SpriteBuffer;
sfTexture* SpriteBufferTexture;
sfSprite* SpriteBufferSprite;

sfTexture* GroundSurface;

IJK_Float LightIncedence;

//Bilinear Interpolation Coefficients.
	float a00, a01, a02, a03;
	float a10, a11, a12, a13;
	float a20, a21, a22, a23;
	float a30, a31, a32, a33;


sfRenderStates GroundRenderer; 
sfShader* GroundShader;


sfRenderStates Drawable_Renderer; 
sfShader* Drawable_Shader;

Slide Drawable_Slide;
Slide LightsSlide;

//-----------------------------COORDINATE-TRANSFORM-FUNCTIONS----------------------------//

XY_Point IJKtoXYzero ( IJK_Point a ) //x is to the right, y is down
{
    XY_Point b;
    b.x = (a.i + a.j) XSCALE;

    b.y = (a.i - a.j) YSCALE - a.k ZSCALE;

    return b;
}

XY_Point IJKtoXYrelative ( IJK_Point a ) {
    XY_Point b;
    b.x = SCREEN_WIDTH/2 + ((a.i - ViewPoint.i) + (a.j - ViewPoint.j)) XSCALE + BORDER;
    
    b.y = SCREEN_HEIGHT/2 + ((a.i - ViewPoint.i)
                - (a.j - ViewPoint.j) + 4096) YSCALE - 4095 YSCALE - (a.k - ViewPoint.k) ZSCALE + BORDER ;
//We introduce '- 4095 YSCALE' so that '((a.i - ViewPoint.i) - (a.j - ViewPoint.j) + 4096) YSCALE'
//is always positive, such that when the non-ineger value is rounded towards zero, is always rounds down
    return b;
}

XY_Float IJKtoXYfloat ( IJK_Point a ) {
    XY_Float b;
    b.x = SCREEN_WIDTH/2.0 + ((a.i - ViewPoint.i) + (a.j - ViewPoint.j)) * 1.0 XSCALE + BORDER;
    
    b.y = SCREEN_HEIGHT/2.0 + ((a.i - ViewPoint.i)
                - (a.j - ViewPoint.j)) *1.0 YSCALE - (a.k - ViewPoint.k) *1.0 ZSCALE + BORDER;
//We introduce '- 4095 YSCALE' so that '((a.i - ViewPoint.i) - (a.j - ViewPoint.j) + 4096) YSCALE'
//is always positive, such that when the non-ineger value is rounded towards zero, is always rounds down
    return b;
}

IJK_Point XYtoIJ0relative ( XY_Point a ) {
    IJK_Point b;
    b.k = 0;
    
    b.i = ((a.x - SCREEN_WIDTH/2) XSCALE_INV + (a.y - ViewPoint.k ZSCALE - SCREEN_HEIGHT/2) YSCALE_INV)/2 + ViewPoint.i;

    b.j = ((a.x - SCREEN_WIDTH/2) XSCALE_INV - (a.y - ViewPoint.k ZSCALE - SCREEN_HEIGHT/2) YSCALE_INV)/2 + ViewPoint.j;

    
    return b;
}


IJK_Float Normalise_IJK_Float (IJK_Float z){

    float normaliser = sqrt(z.i*z.i + z.j*z.j + z.k*z.k);

    z.i /= normaliser;
    z.j /= normaliser;
    z.k /= normaliser;

    return z;
}

IJK_Point IJ0toIJKGetElevations(IJK_Point P){

    int SquareM, SquareN;
    int TileM, TileN;
    int ResidualM, ResidualN;
    int ResidualI, ResidualJ;

    int Det1, Det2, Det3, Det4;

    SquareM = P.i / MAPSQUARESIZE;
    SquareN = P.j / MAPSQUARESIZE;

    ResidualM = P.i % MAPSQUARESIZE;
    ResidualN = P.j % MAPSQUARESIZE;

    TileM = ResidualM / MAPTILESIZE;
    TileN = ResidualN / MAPTILESIZE;

    ResidualI = ResidualM % MAPTILESIZE; //x
    ResidualJ = ResidualN % MAPTILESIZE; //y



    //        | z0  y0  1 |         | x0  z0  1 |         | x0  y0  z0 |      | x0  y0  1 |
    //P.k = ( | z1  y1  1 | * x   + | x1  z1  1 | * y   + | x1  y1  z1 |  ) / | x1  y1  1 |
    //        | z2  y2  1 |         | x2  z2  1 |         | x2  y2  z2 |      | x2  y2  1 |


//Det 1
    // | z0  y0  1 | = -y0 z1 + y0 z2 + y1 z0 - y1 z2 - y2 z0 + y2 z1
    // | z1  y1  1 |
    // | z2  y2  1 |

//Det 2
    // | x0  z0  1 | = -z0 x1 + z0 x2 + z1 x0 - z1 x2 - z2 x0 + z2 x1
    // | x1  z1  1 |
    // | x2  z2  1 |

//Det 3
    // | x0  y0  z0 | = x0 y1 z2 - x0 y2 z1 - x1 y0 z2 + x1 y2 z0 + x2 y0 z1 - x2 y1 z0 
    // | x1  y1  z1 |
    // | x2  y2  z2 |

//Det 4
    // | x0  y0  1 | = -y0 x1 + y0 x2 + y1 x0 - y1 x2 - y2 x0 + y2 x1
    // | x1  y1  1 |
    // | x2  y2  1 |

// Det 4 != 0 so no glitches there...
//x0, y0 = 0...

    int Left_Pointi = 0;           //x0
    int Left_Pointj = 0;           //y0
    int Left_Pointk = TerrainSquares[SquareM][SquareN].ElevationData[TileM+1][TileN+1];  //z0

    int Middle_Pointi; //x2
    int Middle_Pointj; //y2
    int Middle_Pointk; //z2

    int Right_Pointi = MAPTILESIZE; //x1
    int Right_Pointj = MAPTILESIZE; //y1
    int Right_Pointk = TerrainSquares[SquareM][SquareN].ElevationData[TileM+2][TileN+2]; //z1

    if (ResidualI < ResidualJ) {
        //Upper triangle

        Middle_Pointi = 0; //x2
        Middle_Pointj = MAPTILESIZE; //y2
        Middle_Pointk = TerrainSquares[SquareM][SquareN].ElevationData[TileM+1][TileN+2]; //z2

        Det1 = Right_Pointj * Left_Pointk - Right_Pointj * Middle_Pointk - Middle_Pointj * Left_Pointk + Middle_Pointj * Right_Pointk ;
        Det2 = -Left_Pointk * Right_Pointi + Middle_Pointk *  Right_Pointi;
        Det3 =  Right_Pointi * Middle_Pointj * Left_Pointk + Middle_Pointi * Right_Pointj * Left_Pointk; 
        Det4 =  Middle_Pointj * Right_Pointi;

    } else {
        //Lower triangle

        Middle_Pointi = MAPTILESIZE; //x2
        Middle_Pointj = 0; //y2
        Middle_Pointk = TerrainSquares[SquareM][SquareN].ElevationData[TileM+2][TileN+1]; //z2

        Det1 = Right_Pointj * Left_Pointk - Right_Pointj * Middle_Pointk;
        Det2 = -Left_Pointk * Right_Pointi + Left_Pointk * Middle_Pointi  - Right_Pointk * Middle_Pointi  + Middle_Pointk * Right_Pointi;
        Det3 =  - Middle_Pointi * Right_Pointj * Left_Pointk;
        Det4 = - Right_Pointj * Middle_Pointi ;


    }

    P.k = (Det1 * ResidualI + Det2 * ResidualJ + Det3)/Det4;

    return P;
}

//-----------------------------BICUBIC-INTERPOLATION-FUNCTIONS----------------------------//




int UpdateCoefficients (int SquareM, int SquareN, int TileM, int TileN) {

    //TerrainSquares[SquareM][SquareN].ElevationData[TileM, TileN]


    float p[4][4] =
    { { TerrainSquares[SquareM][SquareN].ElevationData[TileM][TileN], 
        TerrainSquares[SquareM][SquareN].ElevationData[TileM][TileN+1],
        TerrainSquares[SquareM][SquareN].ElevationData[TileM][TileN+2],
        TerrainSquares[SquareM][SquareN].ElevationData[TileM][TileN+3] },

      { TerrainSquares[SquareM][SquareN].ElevationData[TileM+1][TileN], 
        TerrainSquares[SquareM][SquareN].ElevationData[TileM+1][TileN+1],
        TerrainSquares[SquareM][SquareN].ElevationData[TileM+1][TileN+2],
        TerrainSquares[SquareM][SquareN].ElevationData[TileM+1][TileN+3] },

      { TerrainSquares[SquareM][SquareN].ElevationData[TileM+2][TileN], 
        TerrainSquares[SquareM][SquareN].ElevationData[TileM+2][TileN+1],
        TerrainSquares[SquareM][SquareN].ElevationData[TileM+2][TileN+2],
        TerrainSquares[SquareM][SquareN].ElevationData[TileM+2][TileN+3] },

      { TerrainSquares[SquareM][SquareN].ElevationData[TileM+3][TileN], 
        TerrainSquares[SquareM][SquareN].ElevationData[TileM+3][TileN+1],
        TerrainSquares[SquareM][SquareN].ElevationData[TileM+3][TileN+2],
        TerrainSquares[SquareM][SquareN].ElevationData[TileM+3][TileN+3] }};

    a00 = p[1][1];
    a01 = -0.5*p[1][0] + 0.5*p[1][2];
    a02 = p[1][0] - 2.5*p[1][1] + 2*p[1][2] - 0.5*p[1][3];
    a03 = -0.5*p[1][0] + 1.5*p[1][1] - 1.5*p[1][2] + 0.5*p[1][3];
    a10 = -0.5*p[0][1] + 0.5*p[2][1];
    a11 = 0.25*p[0][0] - 0.25*p[0][2] - 0.25*p[2][0] + 0.25*p[2][2];
    a12 = -0.5*p[0][0] + 1.25*p[0][1] - p[0][2] + 0.25*p[0][3] 
            + 0.5*p[2][0] - 1.25*p[2][1] + p[2][2] - 0.25*p[2][3];

    a13 = 0.25*p[0][0] - 0.75*p[0][1] + 0.75*p[0][2] - 0.25*p[0][3]
            - 0.25*p[2][0] + 0.75*p[2][1] - 0.75*p[2][2] + 0.25*p[2][3];

    a20 = p[0][1] - 2.5*p[1][1] + 2*p[2][1] - 0.5*p[3][1];

    a21 = -0.5*p[0][0] + 0.5*p[0][2] + 1.25*p[1][0] - 1.25*p[1][2]
            - p[2][0] + p[2][2] + 0.25*p[3][0] - 0.25*p[3][2];

    a22 = p[0][0] - 2.5*p[0][1] + 2*p[0][2] - 0.5*p[0][3] - 2.5*p[1][0]
            + 6.25*p[1][1] - 5*p[1][2] + 1.25*p[1][3] + 2*p[2][0] - 5*p[2][1] + 4*p[2][2]
            - p[2][3] - 0.5*p[3][0] + 1.25*p[3][1] - p[3][2] + 0.25*p[3][3];
  
    a23 = -0.5*p[0][0] + 1.5*p[0][1] - 1.5*p[0][2] + 0.5*p[0][3]
            + 1.25*p[1][0] - 3.75*p[1][1] + 3.75*p[1][2] - 1.25*p[1][3] - p[2][0] + 3*p[2][1] 
            - 3*p[2][2] + p[2][3] + 0.25*p[3][0] - 0.75*p[3][1] + 0.75*p[3][2] - 0.25*p[3][3];

    a30 = -0.5*p[0][1] + 1.5*p[1][1] - 1.5*p[2][1] + 0.5*p[3][1];

    a31 = 0.25*p[0][0] - 0.25*p[0][2] - 0.75*p[1][0] + 0.75*p[1][2] 
            + 0.75*p[2][0] - 0.75*p[2][2] - 0.25*p[3][0] + 0.25*p[3][2];

    a32 = -0.5*p[0][0] + 1.25*p[0][1] - p[0][2] + 0.25*p[0][3]
            + 1.5*p[1][0] - 3.75*p[1][1] + 3*p[1][2] - 0.75*p[1][3] - 1.5*p[2][0] + 3.75*p[2][1]
            - 3*p[2][2] + 0.75*p[2][3] + 0.5*p[3][0] - 1.25*p[3][1] + p[3][2] - 0.25*p[3][3];

    a33 = 0.25*p[0][0] - 0.75*p[0][1] + 0.75*p[0][2] - 0.25*p[0][3] 
            - 0.75*p[1][0] + 2.25*p[1][1] - 2.25*p[1][2] + 0.75*p[1][3] + 0.75*p[2][0]
            - 2.25*p[2][1] + 2.25*p[2][2] - 0.75*p[2][3] - 0.25*p[3][0] + 0.75*p[3][1]
            - 0.75*p[3][2] + 0.25*p[3][3];
}


float GetValue (int i, int j) {

    float x = (i*1.0)/TEXTURETILESIZE;
    float y = (j*1.0)/TEXTURETILESIZE;
    float x2 = x * x;
    float x3 = x2 * x;
    float y2 = y * y;
    float y3 = y2 * y;

    return (a00 + a01 * y + a02 * y2 + a03 * y3) +
           (a10 + a11 * y + a12 * y2 + a13 * y3) * x +
           (a20 + a21 * y + a22 * y2 + a23 * y3) * x2 +
           (a30 + a31 * y + a32 * y2 + a33 * y3) * x3;
}


IJK_Float GetNormal (int i, int j) {

    float x = (i*1.0)/TEXTURETILESIZE;
    float y = (j*1.0)/TEXTURETILESIZE;
    float x2 = x * x;
    float x3 = x2 * x;
    float y2 = y * y;
    float y3 = y2 * y;

    float partialx = (
         (a00 + a01 * y + a02 * y2 + a03 * y3)*0 +
           (a10 + a11 * y + a12 * y2 + a13 * y3) * 1 +
           (a20 + a21 * y + a22 * y2 + a23 * y3) * 2 * x +
           (a30 + a31 * y + a32 * y2 + a33 * y3) * 3 * x2)

           /TEXTURETILESIZE;


    float partialy = (
           (a00 * 0 + a01 * 1 + a02 * 2*y + a03 * 3*y2) +
           (a10 * 0 + a11 * 1 + a12 * 2*y + a13 * 3*y2) * x +
           (a20 * 0 + a21 * 1 + a22 * 2*y + a23 * 3*y2) * x2 +
           (a30 * 0 + a31 * 1 + a32 * 2*y + a33 * 3*y2) * x3)

           /TEXTURETILESIZE;

    IJK_Float SurfaceNormal;

    SurfaceNormal.i = -partialx; //-df/di
    SurfaceNormal.j = -partialy; //-df/dj
    SurfaceNormal.k = 1;

    return Normalise_IJK_Float (SurfaceNormal);
}


//-----------------------------HILL-SHADING-FUNCTIONS----------------------------//

int CalcHillShading(int SquareM, int SquareN){

   
    int TileM, TileN;

    int i,j;


    IJK_Float LightInc = Normalise_IJK_Float (LightIncedence);   //points TOWARD the sun


    for (TileM = 0; TileM < SUBDIVISIONS; TileM++){

        for (TileN = 0; TileN < SUBDIVISIONS; TileN++){

            UpdateCoefficients (SquareM, SquareN, TileM, TileN);

            for (i = 0; i < TEXTURETILESIZE; i++) {

                for (j = 0; j < TEXTURETILESIZE; j++){ 



                     IJK_Float SurfaceNormal = GetNormal (i, j);

                     float DotProduct = SurfaceNormal.i * LightInc.i
                        + SurfaceNormal.j * LightInc.j + SurfaceNormal.k * LightInc.k;

                     if (DotProduct < 0) DotProduct = 0;

                     DotProduct = 0.1 + 0.9 * DotProduct;


                     int TextureI = TileM * TEXTURETILESIZE + i;

                     int TextureJ = TileN * TEXTURETILESIZE + j;

                     int DestCoord = (TextureI * TEXTURESQUARESIZE + TextureJ) * 4;

                     

                     TerrainSquares[SquareM][SquareN].LightMapPtr[DestCoord+0] =  DotProduct * 255;

                     TerrainSquares[SquareM][SquareN].LightMapPtr[DestCoord+1] = DotProduct * 255;

                     TerrainSquares[SquareM][SquareN].LightMapPtr[DestCoord+2] = DotProduct * 255;

                     TerrainSquares[SquareM][SquareN].LightMapPtr[DestCoord+3] = 255;

                     
                }

            }


        }

    }
    sfIntRect area;
    area.left = 0 ;
    area.top = 0 ;
    area.width = TEXTURESQUARESIZE;
    area.height = TEXTURESQUARESIZE;
    TerrainSquares[SquareM][SquareN].GPULightMap = sfTexture_createFromImage(TerrainSquares[SquareM][SquareN].CPULightMap, &area);



}

//-----------------------------SHADER-FUNCTIONS----------------------------//


int CreateShaders(void){

//vertex shader
    char vertShader[] = "\
        uniform vec2 offset;\
        void main()\
        {\
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
        uniform sampler2D light_map;\
        uniform sampler2D ground_surface;\
        \
        void main()\
        {\
            vec4 pixel = texture2D(ground_surface, gl_TexCoord[0].xy);\
        \
            vec4 Light = texture2D(light_map, gl_TexCoord[0].xy);\
            gl_FragColor =  gl_Color * Light * pixel;\
        }\
    ";


    GroundShader = sfShader_createFromMemory(vertShader, fragShader);
    assert (GroundShader != NULL);
    GroundRenderer.shader = GroundShader;
    GroundRenderer.blendMode = sfBlendAlpha;
    GroundRenderer.transform = sfTransform_Identity;
    GroundRenderer.texture = NULL;


//Fragment Shader
    char fragDrawable_Shader[] = "\
        uniform sampler2D texture;\
        uniform vec4 Shade;\
        \
        void main()\
        {\
            vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);\
        \
            gl_FragColor =  gl_Color * (Shade * pixel);\
        }\
    ";


    Drawable_Shader = sfShader_createFromMemory(NULL, fragDrawable_Shader);
    assert (Drawable_Shader != NULL);
    Drawable_Renderer.shader = Drawable_Shader;
    Drawable_Renderer.blendMode = sfBlendAlpha;
    Drawable_Renderer.transform = sfTransform_Identity;
    Drawable_Renderer.texture = NULL;


}


//-----------------------------UNSORTED-FUNCTIONS----------------------------//




int LoadElevations(int m, int n){
    //What area of Heightmap is covered by TerrainSquares[m][n]?
    int m1,n1;  // Coordinates of tile within terrain square.
    for (m1 = 0; m1 < SUBDIVISIONS+3; m1++) {
        for (n1 = 0; n1 < SUBDIVISIONS+3; n1++) {

            int m2 = m * SUBDIVISIONS;   //map tile coordinates of leftmost corner of terrain square.
            int n2 = n * SUBDIVISIONS;

            int m3 = m2 + m1;             //map tile coordinates of elevation point
            int n3 = n2 + n1;

            int HeightmapCoord = (m3 * HeightMapSize.x + n3) * 4;
            TerrainSquares[m][n].ElevationData[m1][n1] = HeightMapPtr[HeightmapCoord] ELEVATIONSCALE;

            //Note that (m1, n1) = (0, 0) refers to the elevation at a point one tile to the
            //left of what is drawn to the screen, used for calculating terrain shaddows of a tile.
        }
    }
}

int SetGroundSurface(int m, int n){
    TerrainSquares[m][n].GroundSurface = GroundSurface;

}
int CreateVertexArray(int m, int n){
    TerrainSquares[m][n].GPU_Elevations = sfVertexArray_create();
    sfVertexArray_setPrimitiveType(TerrainSquares[m][n].GPU_Elevations, sfTriangles);
    sfVertex topVertex, bottomVertex, leftVertex, rightVertex;
    
    IJK_Point ThreeDVertices[SUBDIVISIONS+1][SUBDIVISIONS+1];
    XY_Point TwoDTextureVertices[SUBDIVISIONS+1][SUBDIVISIONS+1];
    XY_Point TwoDScreenVertices[SUBDIVISIONS+1][SUBDIVISIONS+1];    

    int m1, n1; //M is right (x), n is down (y)

    for (m1 = 0; m1 < SUBDIVISIONS+1; m1++) {     
        for (n1 = 0; n1 < SUBDIVISIONS+1; n1++) {

            ThreeDVertices[m1][n1].i = MAPSQUARESIZE/SUBDIVISIONS * m1;  //down -> down and to the right
            ThreeDVertices[m1][n1].j = MAPSQUARESIZE/SUBDIVISIONS * n1;  //right -> up to the right
            ThreeDVertices[m1][n1].k = TerrainSquares[m][n].ElevationData[m1+1][n1+1];
                      //First and last elevation points are discarded.
                      //They will be used later when calculating terrain shaddows.

            TwoDTextureVertices[m1][n1].x = TEXTURESQUARESIZE/SUBDIVISIONS * n1;
            TwoDTextureVertices[m1][n1].y = TEXTURESQUARESIZE/SUBDIVISIONS * m1;

            TwoDScreenVertices[m1][n1] = IJKtoXYzero ( ThreeDVertices[m1][n1] );

        }

    }
    for (m1 = 0; m1 < SUBDIVISIONS; m1++) {
        for (n1 = 0; n1 < SUBDIVISIONS; n1++) {

            //for each subdivision of Medium Square, add upper and lower triangles of the subdivision.


            topVertex.position.x = TwoDScreenVertices[m1][n1+1].x;
            topVertex.position.y = TwoDScreenVertices[m1][n1+1].y;


            topVertex.texCoords.x = TwoDTextureVertices[m1][n1+1].x;
            topVertex.texCoords.y = TwoDTextureVertices[m1][n1+1].y;


            bottomVertex.position.x = TwoDScreenVertices[m1+1][n1].x;
            bottomVertex.position.y = TwoDScreenVertices[m1+1][n1].y;

            bottomVertex.texCoords.x = TwoDTextureVertices[m1+1][n1].x;
            bottomVertex.texCoords.y = TwoDTextureVertices[m1+1][n1].y;


            leftVertex.position.x = TwoDScreenVertices[m1][n1].x;
            leftVertex.position.y = TwoDScreenVertices[m1][n1].y;

            leftVertex.texCoords.x = TwoDTextureVertices[m1][n1].x;
            leftVertex.texCoords.y = TwoDTextureVertices[m1][n1].y;


            rightVertex.position.x = TwoDScreenVertices[m1+1][n1+1].x;
            rightVertex.position.y = TwoDScreenVertices[m1+1][n1+1].y;

            rightVertex.texCoords.x = TwoDTextureVertices[m1+1][n1+1].x;
            rightVertex.texCoords.y = TwoDTextureVertices[m1+1][n1+1].y;


            topVertex.color = sfWhite;
            bottomVertex.color = sfWhite;
            leftVertex.color = sfWhite;
            rightVertex.color = sfWhite;


            sfVertexArray_append(TerrainSquares[m][n].GPU_Elevations, topVertex);
            sfVertexArray_append(TerrainSquares[m][n].GPU_Elevations, rightVertex);
            sfVertexArray_append(TerrainSquares[m][n].GPU_Elevations, leftVertex);

            sfVertexArray_append(TerrainSquares[m][n].GPU_Elevations, leftVertex);
            sfVertexArray_append(TerrainSquares[m][n].GPU_Elevations, rightVertex);
            sfVertexArray_append(TerrainSquares[m][n].GPU_Elevations, bottomVertex);
        }
    }

}


int CreateDrawable(IJK_Point location) { 

    location = IJ0toIJKGetElevations(location);
    Drawable* Drawable_ = malloc(sizeof(Drawable));
    Drawable_->location = location;
    Drawable_->slide[0] = &Drawable_Slide;
    Drawable_->angle[0] = 0;
    Drawable_->frame[0] = 0;

    Drawable_->slide[1] = &LightsSlide;
    Drawable_->angle[1] = 0;
    Drawable_->frame[1] = rand() % 6;

    Drawable_->slide[2] = NULL;
    Drawable_->slide[3] = NULL;
    Drawable_->slide[4] = NULL;
    Drawable_->slide[5] = NULL;
    Drawable_->slide[6] = NULL;
    Drawable_->slide[7] = NULL;
    

//Add to end of global list
    if (Drawable_Head == NULL){
        Drawable_Head = Drawable_;
        Drawable_Tail = Drawable_;
        Drawable_->global.previous = NULL;
        Drawable_->global.next = NULL;
    } else { 
        Drawable_Tail->global.next = Drawable_;
        Drawable_->global.previous = Drawable_Tail;
        Drawable_->global.next = NULL;
        Drawable_Tail = Drawable_;
    }

//Insert into sorted list belonging to TerrainSquares[SquareM][SquareN]
    int SquareM, SquareN;

    SquareM = location.i / MAPSQUARESIZE;
    SquareN = location.j / MAPSQUARESIZE;

    int Drawable_Distance = Drawable_->location.j - Drawable_->location.i;
   
    if (TerrainSquares[SquareM][SquareN].Drawable_Head == NULL) {
        assert(TerrainSquares[SquareM][SquareN].Drawable_Tail == NULL);
        TerrainSquares[SquareM][SquareN].Drawable_Head = Drawable_;
        TerrainSquares[SquareM][SquareN].Drawable_Tail = Drawable_;
        Drawable_->local.previous = NULL;
        Drawable_->local.next = NULL;

//drawable_head closer than Drawable_, put drawable_ behind it
    } else if (Drawable_Distance > TerrainSquares[SquareM][SquareN].Drawable_Head->location.j
                            -TerrainSquares[SquareM][SquareN].Drawable_Head->location.i) {
      
        Drawable_->local.previous = NULL;
        Drawable_->local.next = TerrainSquares[SquareM][SquareN].Drawable_Head;
        TerrainSquares[SquareM][SquareN].Drawable_Head->local.previous = Drawable_;
        TerrainSquares[SquareM][SquareN].Drawable_Head = Drawable_;
          

//drawable_tail further than Drawable_, put drawable_ in front of it
    } else if  (Drawable_Distance < TerrainSquares[SquareM][SquareN].Drawable_Tail->location.j
                            -TerrainSquares[SquareM][SquareN].Drawable_Tail->location.i){

        Drawable_->local.next = NULL;
        Drawable_->local.previous = TerrainSquares[SquareM][SquareN].Drawable_Tail;
        TerrainSquares[SquareM][SquareN].Drawable_Tail->local.next = Drawable_;
        TerrainSquares[SquareM][SquareN].Drawable_Tail = Drawable_;

//while node is further than Drawable_, go to next node
    } else {
    Drawable* node = TerrainSquares[SquareM][SquareN].Drawable_Head;
        
        while (Drawable_Distance < node->location.j - node->location.i) {
            node = node->local.next;
        }
    //Insert Drawable_ before node

        Drawable_->local.next = node;
        Drawable_->local.previous = node->local.previous;
        ((Drawable*)Drawable_->local.previous)->local.next = Drawable_;
        ((Drawable*)Drawable_->local.next)->local.previous = Drawable_;
    }
        

}




int GenDrawables (int SquareM, int SquareN) {
    int TileM = 0, TileN = 0;
            
    for (TileM = 0; TileM < SUBDIVISIONS; TileM += 16) {
        for (TileN = 0; TileN < SUBDIVISIONS; TileN += 16) {

            IJK_Point location;
            location.i = MAPSQUARESIZE * SquareM + MAPTILESIZE * TileM +  rand() % (MAPTILESIZE*8);
            location.j = MAPSQUARESIZE * SquareN + MAPTILESIZE * TileN +  rand() % (MAPTILESIZE*8);
            CreateDrawable(location);

        }
    }

}



int GenSquares(void){

    
    HeightMap = sfImage_createFromFile("HeightMap.bmp");
    assert (HeightMap != NULL);
    HeightMapPtr = sfImage_getPixelsPtr(HeightMap);
    HeightMapSize = sfImage_getSize(HeightMap);
    
    GroundSurface = sfTexture_createFromFile("grass4.bmp", NULL);
    assert(GroundSurface != NULL);

    


    int SquareM,SquareN;        
    for (SquareM = 0; SquareM < MAPDIMSQUARES; SquareM++) {
        for (SquareN = 0; SquareN < MAPDIMSQUARES; SquareN++) {

            TerrainSquares[SquareM][SquareN].CPULightMap = sfImage_create(TEXTURESQUARESIZE,TEXTURESQUARESIZE);
            assert(TerrainSquares[SquareM][SquareN].CPULightMap != NULL);
            TerrainSquares[SquareM][SquareN].LightMapPtr = sfImage_getPixelsPtr(TerrainSquares[SquareM][SquareN].CPULightMap); 

            TerrainSquares[SquareM][SquareN].Location.x = SquareM;
            TerrainSquares[SquareM][SquareN].Location.y = SquareN;
            TerrainSquares[SquareM][SquareN].Drawable_Head = NULL;
            TerrainSquares[SquareM][SquareN].Drawable_Tail = NULL;
            LoadElevations(SquareM,SquareN);
            SetGroundSurface(SquareM,SquareN);
            CreateVertexArray(SquareM,SquareN);
            CalcHillShading(SquareM,SquareN);                     //TODO Move this to DrawSquare(), Lazy evaluation rather than greedy evaluation.
            GenDrawables(SquareM,SquareN);     
                       
        }
    }
}



int DrawSquare(int SquareM, int SquareN, XY_Float offset){   
    sfVector2f offsetv;
    offsetv.x = offset.x;
    offsetv.y = offset.y;
    sfShader_setVector2Parameter (GroundShader, "offset", offsetv); 
    sfShader_setTextureParameter(GroundShader,"ground_surface", TerrainSquares[SquareM][SquareN].GroundSurface);
    sfShader_setTextureParameter(GroundShader,"light_map", TerrainSquares[SquareM][SquareN].GPULightMap);
    GroundRenderer.texture = TerrainSquares[0][0].GPULightMap;  //This line does nothing, but without it the code doesnt work :P
    sfRenderWindow_drawVertexArray (window, TerrainSquares[SquareM][SquareN].GPU_Elevations, &GroundRenderer);
}



int DrawSquareZ(int m, int n){

    IJK_Point Location;
    Location.i = TerrainSquares[m][n].Location.x * MAPSQUARESIZE;
    Location.j = TerrainSquares[m][n].Location.y * MAPSQUARESIZE;
    Location.k = 0;
    
    XY_Float Screen_Location = IJKtoXYfloat ( Location );

    DrawSquare(m, n, Screen_Location);
}


int DrawVisibleSquares (void){

    XY_Point a;
    //Top Left Corner

    a.x = 0;
    a.y = -ViewPoint.k;

    IJK_Point TL = XYtoIJ0relative (a);

    //Top Right Corner

    a.x = SCREEN_WIDTH;
    a.y = -ViewPoint.k;

    IJK_Point TR = XYtoIJ0relative (a);

    //Bottom Left Corner

    a.x = 0;
    a.y = SCREEN_HEIGHT + ELEVATIONRANGE - ViewPoint.k;

    IJK_Point BL = XYtoIJ0relative (a);

    //Bottom Right Corner

    a.x = SCREEN_WIDTH;
    a.y = SCREEN_HEIGHT + ELEVATIONRANGE - ViewPoint.k;

    IJK_Point BR = XYtoIJ0relative (a);

    //Print squares in the diamond enclosed by TL, TR, BL and BR

    IJK_Point LeftCorner;            //Corners of the square containing the screen rectangle 
    IJK_Point RightCorner;

    LeftCorner.i = TL.i / MAPSQUARESIZE;
    LeftCorner.j = BL.j / MAPSQUARESIZE;

    if (LeftCorner.i < 0) LeftCorner.i = 0;
    if (LeftCorner.j < 0) LeftCorner.j = 0;

    RightCorner.i = BR.i / MAPSQUARESIZE;
    RightCorner.j = TR.j / MAPSQUARESIZE;

    if (RightCorner.i > MAPDIMSQUARES - 1) RightCorner.i = MAPDIMSQUARES - 1;
    if (RightCorner.j > MAPDIMSQUARES - 1) RightCorner.j = MAPDIMSQUARES - 1;
    //minus 1 because index of array goes from 0 to  MAPDIMSQUARES - 1
    int m,n;

    for (n = RightCorner.j; n >= LeftCorner.j; n--){
        for (m = LeftCorner.i; m <= RightCorner.i; m++){
            DrawSquareZ(m,n);
        }
    }
}


sfColor GetHillShading (IJK_Point P) {

    int SquareM = P.i / MAPSQUARESIZE;
    int SquareN = P.j / MAPSQUARESIZE;

    int ResidualI = P.i % MAPSQUARESIZE;
    int ResidualJ = P.j % MAPSQUARESIZE;

    int DestCoord = (ResidualI * TEXTURESQUARESIZE + ResidualJ) * 4;

    sfColor Shade;
    Shade.r = TerrainSquares[SquareM][SquareN].LightMapPtr[DestCoord+0];
    Shade.g = TerrainSquares[SquareM][SquareN].LightMapPtr[DestCoord+1];
    Shade.b = TerrainSquares[SquareM][SquareN].LightMapPtr[DestCoord+2];
    Shade.a = TerrainSquares[SquareM][SquareN].LightMapPtr[DestCoord+3];


    return Shade;
}

int MergeInSquare(int SquareM, int SquareN){

    Drawable* FromList = TerrainSquares[SquareM][SquareN].Drawable_Head;
    if (FromList != NULL) {
    Drawable* IntoList = VisibleDrawable_Head;

    if ( VisibleDrawable_Head == NULL) {
        assert(VisibleDrawable_Tail == NULL);
        VisibleDrawable_Head = FromList;
        VisibleDrawable_Tail = FromList;
        FromList->visible.next = NULL;
        FromList->visible.previous = NULL;
        IntoList = FromList;
        FromList = FromList->local.next;
    }
    
    

    int A, B;

    
    while (FromList != NULL) {
        if (1) {//FromList node on screen

      

            A = (FromList->location.i - FromList->location.j > IntoList->location.i - IntoList->location.j );
            B = IntoList->visible.next != NULL;
            
            while (A && B) {
                IntoList = IntoList->visible.next;
                A = (FromList->location.i - FromList->location.j > IntoList->location.i - IntoList->location.j );
                B = IntoList->visible.next != NULL;
            }
    
            if (IntoList->visible.next == NULL) {
                assert (VisibleDrawable_Tail == IntoList);
                IntoList->visible.next = FromList;
                FromList->visible.previous = IntoList;
                FromList->visible.next = NULL;
                VisibleDrawable_Tail = FromList;
                IntoList = FromList;
            } else {

                FromList->visible.previous = IntoList;
                FromList->visible.next = IntoList->visible.next;
                ((Drawable*)FromList->visible.previous)->visible.next = FromList;
                ((Drawable*)FromList->visible.next)->visible.previous = FromList;
            }
        }
        FromList = FromList->local.next;
    }
    assert(VisibleDrawable_Head != NULL);
    }
}

    sfTexture* TreeTexture[1];
    sfSprite* TreeSprite[1];

    sfTexture* LightsTexture[6];
    sfSprite* LightsSprites[6];


int CreateSprites(){

    sfVector2f vector;
    vector.x = 158;
    vector.y = 444;




    TreeTexture[0] = sfTexture_createFromFile("Tree.png", NULL);
    sfTexture_setSmooth(TreeTexture[0], sfTrue);
    TreeSprite[0] = sfSprite_create();
    sfSprite_setTexture(TreeSprite[0], TreeTexture[0], sfTrue);
    sfSprite_setOrigin (TreeSprite[0], vector);

    LightsTexture[0] = sfTexture_createFromFile("Lights0.png", NULL);
    sfTexture_setSmooth(LightsTexture[0], sfTrue);
    LightsSprites[0] = sfSprite_create();
    sfSprite_setTexture(LightsSprites[0], LightsTexture[0], sfTrue);
    sfSprite_setOrigin (LightsSprites[0], vector);

    LightsTexture[1] = sfTexture_createFromFile("Lights1.png", NULL);
    sfTexture_setSmooth(LightsTexture[1], sfTrue);
    LightsSprites[1] = sfSprite_create();
    sfSprite_setTexture(LightsSprites[1], LightsTexture[1], sfTrue);
    sfSprite_setOrigin (LightsSprites[1], vector);

    LightsTexture[2] = sfTexture_createFromFile("Lights2.png", NULL);
    sfTexture_setSmooth(LightsTexture[2], sfTrue);
    LightsSprites[2] = sfSprite_create();
    sfSprite_setTexture(LightsSprites[2], LightsTexture[2], sfTrue);
    sfSprite_setOrigin (LightsSprites[2], vector);

    LightsTexture[3] = sfTexture_createFromFile("Lights3.png", NULL);
    sfTexture_setSmooth(LightsTexture[3], sfTrue);
    LightsSprites[3] = sfSprite_create();
    sfSprite_setTexture(LightsSprites[3], LightsTexture[3], sfTrue);
    sfSprite_setOrigin (LightsSprites[3], vector);

    LightsTexture[4] = sfTexture_createFromFile("Lights4.png", NULL);
    sfTexture_setSmooth(LightsTexture[4], sfTrue);
    LightsSprites[4] = sfSprite_create();
    sfSprite_setTexture(LightsSprites[4], LightsTexture[4], sfTrue);
    sfSprite_setOrigin (LightsSprites[4], vector);

    LightsTexture[5] = sfTexture_createFromFile("Lights5.png", NULL);
    sfTexture_setSmooth(LightsTexture[5], sfTrue);
    LightsSprites[5] = sfSprite_create();
    sfSprite_setTexture(LightsSprites[5], LightsTexture[5], sfTrue);
    sfSprite_setOrigin (LightsSprites[5], vector);

    //vector.x = 1;
    //vector.y = 1;
    //sfSprite_setScale (TreeSprite, vector);

}


int PlotDrawable(Drawable* Drawable_){

    assert(Drawable_ != NULL);

    sfColor Shade = GetHillShading (Drawable_->location);
    Shade.r = Shade.r / 2 + 128;
    Shade.g = Shade.g / 2 + 128;
    Shade.b = Shade.b / 2 + 128;
    

    XY_Float Location = IJKtoXYfloat ( Drawable_->location );
    sfVector2f loc;
    loc.x = Location.x;
    loc.y = Location.y;

    int i;
    for (i = 0; i < 8; i++) {
        if (Drawable_->slide[i] != NULL) {
            if (Drawable_->slide[i]->type == matte){
            sfSprite_setPosition (Drawable_->slide[i]->sprite[Drawable_->frame[i]/Drawable_->slide[i]->framerate], loc);
            sfShader_setColorParameter (Drawable_Shader, "Shade", Shade);
            sfRenderTexture_drawSprite(SpriteBuffer, Drawable_->slide[i]->sprite[Drawable_->frame[i]/Drawable_->slide[i]->framerate], &Drawable_Renderer);
            Drawable_->frame[i]  = (Drawable_->frame[i] + 1) % (Drawable_->slide[i]->frames * Drawable_->slide[i]->framerate);
            } else if (Drawable_->slide[i]->type == luminous){
            sfSprite_setPosition (Drawable_->slide[i]->sprite[Drawable_->frame[i]/Drawable_->slide[i]->framerate], loc);
            sfShader_setColorParameter (Drawable_Shader, "Shade", sfWhite);
            sfRenderTexture_drawSprite(SpriteBuffer, Drawable_->slide[i]->sprite[Drawable_->frame[i]/Drawable_->slide[i]->framerate], &Drawable_Renderer);
            Drawable_->frame[i]  = (Drawable_->frame[i] + 1) % (Drawable_->slide[i]->frames * Drawable_->slide[i]->framerate);
            }
        }
    }
}

int PlotVisibleDrawables(){

    Drawable* CurrentDrawable_ = VisibleDrawable_Head; //Why is this null?
    assert (CurrentDrawable_ != NULL);
    while (CurrentDrawable_ != NULL) {
        PlotDrawable(CurrentDrawable_);
        CurrentDrawable_ = CurrentDrawable_->visible.next;
    }

}


int DrawVisibleDrawable_s(void){

    VisibleDrawable_Head = NULL;
    VisibleDrawable_Tail = NULL;


    XY_Point a;
    //Top Left Corner

    a.x = 0;
    a.y = -ViewPoint.k;

    IJK_Point TL = XYtoIJ0relative (a);
    
    //Top Right Corner

    a.x = SCREEN_WIDTH;
    a.y = -ViewPoint.k;

    IJK_Point TR = XYtoIJ0relative (a);
    
    //Bottom Left Corner

    a.x = 0;
    a.y = SCREEN_HEIGHT + ELEVATIONRANGE - ViewPoint.k;

    IJK_Point BL = XYtoIJ0relative (a);

    //Bottom Right Corner

    a.x = SCREEN_WIDTH;
    a.y = SCREEN_HEIGHT + ELEVATIONRANGE - ViewPoint.k;

    IJK_Point BR = XYtoIJ0relative (a);

    //Print squares in the diamond enclosed by TL, TR, BL and BR

    IJK_Point LeftCorner;            //Corners of the square containing the screen rectangle 
    IJK_Point RightCorner;

    LeftCorner.i = TL.i / MAPSQUARESIZE;
    LeftCorner.j = BL.j / MAPSQUARESIZE;

    if (LeftCorner.i < 0) LeftCorner.i = 0;
    if (LeftCorner.j < 0) LeftCorner.j = 0;

    RightCorner.i = BR.i / MAPSQUARESIZE;
    RightCorner.j = TR.j / MAPSQUARESIZE;

    if (RightCorner.i > MAPDIMSQUARES - 1) RightCorner.i = MAPDIMSQUARES -1;
    if (RightCorner.j > MAPDIMSQUARES - 1) RightCorner.j = MAPDIMSQUARES -1;

    int m,n;

    for (n = RightCorner.j; n >= LeftCorner.j; n--){
        for (m = LeftCorner.i; m <= RightCorner.i; m++){ 
            MergeInSquare(m,n);
        }
    }

    PlotVisibleDrawables();
}

int CreateSpriteBuffer(void) {

    SpriteBuffer = sfRenderTexture_create (SCREEN_WIDTH + 2*BORDER, SCREEN_HEIGHT+2*BORDER, sfFalse);
    SpriteBufferTexture = sfRenderTexture_getTexture(SpriteBuffer);
    SpriteBufferSprite = sfSprite_create();
    sfSprite_setTexture (SpriteBufferSprite, SpriteBufferTexture, sfTrue);
}

int DrawSpriteBuffer(){

    sfRenderTexture_display(SpriteBuffer);
    sfRenderWindow_drawSprite (window, SpriteBufferSprite, NULL);
    sfRenderTexture_clear (SpriteBuffer, sfTransparent);
}




int CreateSlides() {

    Drawable_Slide.type = matte;
    Drawable_Slide.angles = 1;
    Drawable_Slide.frames = 1;
    Drawable_Slide.framerate = 1;
    Drawable_Slide.sprite = TreeSprite;   

    LightsSlide.type = luminous;
    LightsSlide.angles = 1;
    LightsSlide.frames = 6;
    LightsSlide.framerate = 4;
    LightsSlide.sprite = LightsSprites;


}


int main(void) {
    window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);
    assert (window != NULL);

    CreateSpriteBuffer();

    CreateSprites();

    CreateSlides();

    LightIncedence.i = 1;
    LightIncedence.j = 1;
    LightIncedence.k = 1;

    sfRectangleShape* rectangle= sfRectangleShape_create();//DEBUG
    assert (rectangle != NULL);

    sfVector2f position;//DEBUG

    position.x = SCREEN_WIDTH;//DEBUG
    position.y = SCREEN_HEIGHT;//DEBUG
    sfRectangleShape_setSize (rectangle, position);
    position.x = BORDER;//DEBUG
    position.y = BORDER;//DEBUG
    sfRectangleShape_setPosition(rectangle, position);//DEBUG
    sfRectangleShape_setFillColor(rectangle, sfTransparent);//DEBUG
    sfRectangleShape_setOutlineColor(rectangle, sfBlack);//DEBUG
    sfRectangleShape_setOutlineThickness (rectangle, 1.0);

    GenSquares();
    CreateShaders();

    ViewPoint.i = 700;
    ViewPoint.j = 700;
    ViewPoint.k = 0; 

    sfRenderWindow_setFramerateLimit (window, 60);

        int i = 1000; int c = 1;
    while(1){
        ViewPoint = IJ0toIJKGetElevations(ViewPoint);

        DrawVisibleSquares();
        DrawVisibleDrawable_s();
        DrawSpriteBuffer();

        

        sfRenderWindow_drawRectangleShape (window, rectangle, NULL);
        sfRenderWindow_display(window);

        sfRenderWindow_clear (window, sfColor_fromRGB (255,255,255));

        
        

        i--;
        if (i == 0){
            i = 1000;
            c++;
        }
        if(c == 5) c = 1; 

        if (c == 1){
            ViewPoint.i += 1;
        }else if (c == 3){
            ViewPoint.i -= 1;
        }else if (c == 2){
            ViewPoint.j += 1;  
        }else if (c == 4){
            ViewPoint.j -= 1;
        }else if (c == 0){
            ViewPoint.k += 1;
        }else if (c == 0){
            ViewPoint.k -= 1;
        }else if (c == 0){
            exit(1);
        }

        
    
    }



sleep(5);
}

//Destroy all humans
