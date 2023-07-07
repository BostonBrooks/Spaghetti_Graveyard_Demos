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


bbTerrainSquare bbTerrainSquare_grid[SQUARES_PER_MAP][SQUARES_PER_MAP];

int bbDrawable_lost_head;
int bbDrawable_lost_tail;


//-----------------------------FUNCTIONS--------------------------//

int bbTerrainSquare_draw(int Square_i, int Square_j);
int bbTerrainSquare_drawVisible(void);
int bbTerrainSquare_initAll(void);


int bbTerrainSquare_initAll(void){

    bbDrawable_lost_head = -1;
    bbDrawable_lost_tail = -1;
    int i, j;
    
    for (i=0; i< SQUARES_PER_MAP; i++){
    
        for (j=0; j< SQUARES_PER_MAP; j++){
    
            bbTerrainSquare* TS = &bbTerrainSquare_grid[i][j];
            TS->i_Coord = i;
            TS->j_Coord = j;
            TS->bbDrawable_head = -1;
            TS->bbDrawable_tail = -1;
        }
    }

}

int bbTerrainSquare_draw(int Square_i, int Square_j){

    bbTerrainSquare* TS = &bbTerrainSquare_grid[Square_i][Square_j];

    int tobedrawn_int = TS->bbDrawable_head;
    
    if ( tobedrawn_int == -1) return 0;
    
    while (tobedrawn_int != -1){
        bbDrawable* tobedrawn = bbDrawable_Pool_Lookup(tobedrawn_int);
        bbDrawable_plot(tobedrawn);
        tobedrawn_int = tobedrawn->Square_Next;
        
    }
    return 0;

}

//TODO take a sorted list of drawables per bbTerrainSquare, merge list,
//print all drawables to screen in order 
int bbTerrainSquare_drawVisible(){
    
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
            bbTerrainSquare_draw(m,n);
        }
    }
}
