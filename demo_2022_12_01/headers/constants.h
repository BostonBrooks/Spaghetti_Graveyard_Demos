#define CONSTANTS

//-----------------------------WINDOW----------------------------//

#define WINDOW_HEIGHT          1080
#define WINDOW_WIDTH           1920

#define VIEWPORT_TOP           21
#define VIEWPORT_LEFT          21
#define VIEWPORT_WIDTH         1281
#define VIEWPORT_HEIGHT        1038

#define HEALTHBAR_TOP          127
#define HEALTHBAR_LEFT         67
#define HEALTHBAR_WIDTH        20
#define HEALTHBAR_HEIGHT       675



//-----------------------------GEOMETRY----------------------------//

#define POINTS_PER_PIXEL      8
#define PIXELS_PER_TILE       16
#define TILES_PER_SQUARE      32
#define SQUARES_PER_MAP       5
#define PIXELS_PER_SQUARE     (PIXELS_PER_TILE*TILES_PER_SQUARE)  // == 512
#define TILES_PER_MAP         (TILES_PER_SQUARE*SQUARES_PER_MAP)  // == 256
#define POINTS_PER_SQUARE     (POINTS_PER_PIXEL*PIXELS_PER_TILE*TILES_PER_SQUARE)
#define HEIGHT_MAP_PADDING    16
#define HEIGHT_MAP_SIZE       (TILES_PER_MAP + 1 + 2 * 16)

#define ISOMETRIC_FACTOR      2.0


#define ELEVATION_SCALE       4
// The elevation in POINTS is given by the pixel value of the height map * ELEVATION_SCALE

#define PIXEL_VALUE_MAX     256
#define ELEVATION_MAX       (ELEVATION_SCALE * PIXEL_VALUE_MAX)

//-----------------------------GROUNDSURFACE----------------------------//

#define DEFAULT_COLOR_R 104
#define DEFAULT_COLOR_G 151
#define DEFAULT_COLOR_B 100
#define DEFAULT_COLOR_A 255

//-----------------------------DRAWABLES---------------------------------//

#define DRAWABLE_POOL_LEVEL_1 1024
#define DRAWABLE_POOL_LEVEL_2 1024

#define ANIMATIONS_PER_DRAWABLE 16

//-----------------------------DRAWFUNCTIONS---------------------------------//

#define NUMBER_OF_DRAW_FUNCTIONS 3
