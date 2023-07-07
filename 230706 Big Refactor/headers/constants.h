/**
 * @file
 *
 * @ingroup 
 * @brief This header file contains all of the numerical constants used by the game, all in one place
 */

#define CONSTANTS

//-----------------------------COMPILATION------------------------//


#define SHOWTIMING
#define VERBOSE
#define DEBUG

//#define FASTLOAD
//skips slow rendering of hillshading to speed up loading of game




//-----------------------------PLACEHOLDERS----------------------------//

#define TBA                    16
#define TBA_LARGE              800

//-----------------------------WINDOW----------------------------//

#define WINDOW_HEIGHT          1080
#define WINDOW_WIDTH           1920

#define VIEWPORT_TOP           21
#define VIEWPORT_LEFT          21
#define VIEWPORT_WIDTH         1281
#define VIEWPORT_HEIGHT        1038

//-----------------------------GEOMETRY----------------------------//

#define POINTS_PER_PIXEL      8   //was 8
#define PIXELS_PER_TILE       16
#define POINTS_PER_TILE       (POINTS_PER_PIXEL*PIXELS_PER_TILE)
#define TILES_PER_SQUARE      32
#define PIXELS_PER_SQUARE     (POINTS_PER_TILE * TILES_PER_SQUARE)
#define POINTS_PER_SQUARE     (POINTS_PER_PIXEL * PIXELS_PER_TILE * TILES_PER_SQUARE)
#define SQUARES_PER_MAP	      5
#define TILES_PER_MAP         (TILES_PER_SQUARE * SQUARES_PER_MAP)
#define PIXELS_PER_MAP        (PIXELS_PER_TILE * TILES_PER_MAP)
#define POINTS_PER_MAP        (POINTS_PER_PIXEL * PIXELS_PER_MAP)

#define HEIGHT_MAP_PADDING    16
#define HEIGHT_MAP_SIZE       (TILES_PER_MAP + 1 + 2 * HEIGHT_MAP_PADDING)

#define ISOMETRIC_FACTOR      2.0

#define ELEVATION_SCALE       4
// The elevation in POINTS is given by the pixel value of the height map * ELEVATION_SCALE
// This is to circumvent the fact that we have 8 bits or 2^8 possible values on the height map

#define PIXEL_VALUE_MAX     256
// this is = 2^8 as in the above comment

#define ELEVATION_MAX       (ELEVATION_SCALE * PIXEL_VALUE_MAX)

//-----------------------------DRAWABLES---------------------------------//

#define DRAWABLE_POOL_LEVEL_1 TBA_LARGE
#define DRAWABLE_POOL_LEVEL_2 TBA_LARGE

#define ANIMATIONS_PER_DRAWABLE TBA

//-----------------------------DRAWFUNCTIONS---------------------------------//

#define NUMBER_OF_DRAW_FUNCTIONS TBA

//Drawfunction types:

#define DRAW_BASIC      0
#define DRAW_NULL       1
#define DRAW_REPEAT     2
#define DRAW_BALLOON    3
#define DRAW_SHADOW     4


//-----------------------------AICONTROLLERS---------------------------------//

#define AICONTROLLER_POOL_LEVEL_1 TBA_LARGE
#define AICONTROLLER_POOL_LEVEL_2 TBA_LARGE

#define DRAWABLES_PER_AI           TBA

#define RETHUNK      1   //flag message/ai handler to re-process message/ai function
#define NO_RETHUNK   0

//-----------------------------AI TYPES (EXTERNAL STATE)----------------------------------//


#define NUMBER_OF_EXTERNAL_STATES      TBA

#define AI_NULL                     0
#define AI_FOX                      1
#define AI_COW                      2
#define AI_JELLY                    3
#define AI_YETTI                    4

#define PLAYER_BOW_IDLE             5
#define PLAYER_BOW_MOVING	    6
#define PLAYER_BOW_APPROACHING      7
#define PLAYER_BOW_QUESTION         8
#define PLAYER_BOW_WAIT             9   
#define PLAYER_BOW_SHOOT            10

//-----------------------------AI STATES (INTERNAL STATE)-------------------------------//

#define NUMBER_OF_INNER_STATES     TBA

#define COW_IDLE                   0
#define COW_APPROACHING            1
#define COW_ATTACKING              2


//-----------------------------MESSAGES---------------------------------//

#define MESSAGE_POOL_LEVEL_1       TBA_LARGE
#define MESSAGE_POOL_LEVEL_2       TBA_LARGE
#define NUMBER_OF_MESSAGE_TYPES    16

//-----------------------------MESSAGES TYPES-------------------------------//

#define MESSAGE_MOVE		   0

//----------------------------LOCAL AVOIDANCE -----------------------------//

#define AVOIDANCE_METHOD           0 //use 0 for Nelder-Mead

#define NUMBER_OF_AVOIDANCE_SHAPES  TBA

#define AVOIDANCE_NULL              -1
#define AVOIDANCE_CIRCLULAR         0
#define AVOIDANCE_SQUARE            1
#define AVOIDANCE_LINE              2
#define AVOIDANCE_RECTANGLE         3
#define AVOIDANCE_POLYGON           4

//-----------------------------PROMPT---------------------------------//

#define PROMPT_TOP             762
#define PROMPT_LEFT            1324
#define INPUT_TOP              690 // the y coord of input text, comment out to hide input

#define PROMPT_ROWS            50
#define PROMPT_COLLUMNS        16
#define PROMPT_FONT_SIZE       20
#define PROMPT_BUFFER_SIZE     (5 * TBA_LARGE)


//-----------------------------COLOUR SCHEME---------------------------------//

#define COLOUR_BEIGE           {246, 224, 201, 255}
#define COLOUR_BROWN           {101, 67,  32,  255}
#define COLOUR_DARKBROWN       {60,  40,  19,  255}
#define COLOUR_BACKGROUND_NULL sfBlue
#define COLOUR_VIEWPORT_NULL   sfMagenta
#define COLOUR_GROUND_NULL     {104, 151, 100, 255}
