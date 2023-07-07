 /**
 * @file
 *
 * @ingroup 
 * @brief Terrain squares list all of drawables within a sqaure on the map
 * It should be called bbMapSquare
 */

#define BBTERRAINSQUARE

//-----------------------------INCLUDES----------------------------//


#ifndef BBDRAWABLE
#define BBDRAWABLE
#include "../headers/bbDrawable.h"
#endif

#ifndef BBDRAWABLE_PLOT
#define BBDRAWABLE_PLOT
#include "../headers/bbDrawable_plot.h"
#endif


//-----------------------------CONSTANTS----------------------------//


//-----------------------------STRUCTS------------------------------//

typedef struct {

    int i_Coord; //what terrain square are we looking at?
    int j_Coord;
    
    int bbDrawable_head;
    int bbDrawable_tail;
    
} bbTerrainSquare;


extern bbTerrainSquare bbTerrainSquare_grid[SQUARES_PER_MAP][SQUARES_PER_MAP];

extern int bbDrawable_lost_head;
extern int bbDrawable_lost_tail;


//-----------------------------FUNCTIONS--------------------------//

/** Plot drawables within square */
int bbTerrainSquare_draw(int Square_i, int Square_j);

/** Plot drawables on screen */
int bbTerrainSquare_drawVisible(void);

/** Initilise terrain square grid */
int bbTerrainSquare_initAll(void);
