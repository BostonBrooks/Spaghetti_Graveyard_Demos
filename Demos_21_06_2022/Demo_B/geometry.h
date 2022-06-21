#define GEOMETRY

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

#ifndef CONSTANTS
#include "constants.h"
#endif


/*
#define POINTS_PER_PIXEL      8
#define PIXELS_PER_TILE       16
#define TILES_PER_SQUARE      32
#define SQUARES_PER_MAP       8
#define TILES_PER_MAP         (TILES_PER_SQUARE*SQUARES_PER_MAP)  // == 256
#define HEIGHT_MAP_SIZE       (TILES_PER_MAP + 3)

#define ISOMETRIC_FACTOR      2.0

#define VIEWPORT_WIDTH        800
#define VIEWPORT_HEIGHT       600

#define ELEVATION_SCALE       8
// The elevation in POINTS is given by the pixel value of the height map * ELEVATION_SCALE

#define PIXEL_VALUE_MAX     256
#define ELEVATION_MAX       (ELEVATION_SCALE * PIXEL_VALUE_MAX)

// There is no #define BORDER, screen coordinates are relative to subwindow.
*/
//-----------------------------STRUCTS------------------------------//

typedef struct {
    float x;         //x axis is the number of pixels to the right of top left
    float y;         //y axis is down number of pixels down from top left
} Screen_Coords;

typedef struct {
    int i;           //i axis points down and to the right, in units of points
    int j;           //j axis points up and to the right, in units of points
    int k;           //k axis points up, in units of points, negative indicates invalid
} Map_Coords;

typedef struct {
    int i;           //i axis points down and to the right, in units of tiles
    int j;           //j axis points up and to the right, in units of tiles
    int k;           //k axis points up, in units of points, negative indicates invalid
} Tile_Coords;

typedef struct {
    float i;           //i axis points down and to the right, in whatever units
    float j;           //j axis points up and to the right, in whatever units
    float k;           //k axis points up, whatever units
} Float_3d;





//-----------------------------GLOBALS------------------------------//

int elevations[HEIGHT_MAP_SIZE][HEIGHT_MAP_SIZE];
//all of the vertices of map tiles plus a 1 tile buffer.

//Bicubic Interpolation Coefficients.
float bicubic_coefficients[4][4];

int bicubic_coefficients_tile_i;
int bicubic_coefficients_tile_j;

Map_Coords viewpoint;

Float_3d LightIncedence;

//-----------------------------FUNCTIONS------------------------------//

int matrix_multiply (float A[4][4], float B[4][4], float C[4][4]){

    // A B = C
    
    int i, j, k;
    
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            C[i][j] = 0;
        }
    }
    
    for (i = 0; i < 4; i++){
        for (j = 0; j < 4; j++){
            for (k = 0; k < 4; k++){
                C[i][j] += A[i][k]* B[k][j];
            }
        }
    }
    
}

int print_matrix(float A[4][4]){

    printf("%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n\n",
          A[0][0],A[0][1],A[0][2],A[0][3],
          A[1][0],A[1][1],A[1][2],A[1][3],
          A[2][0],A[2][1],A[2][2],A[2][3],
          A[3][0],A[3][1],A[3][2],A[3][3]);
}

int print_screen_coords(Screen_Coords sc){
    printf("x = %f, y = %f\n", sc.x, sc.y);
}

int print_map_coords(Map_Coords mc){
    printf("i = %d, j = %d, k = %d\n", mc.i, mc.j, mc.k);
}

int load_elevations(){

    sfImage*   height_map;
    sfVector2u height_map_size;
    sfUint8*   height_map_values;
    
    height_map = sfImage_createFromFile("HeightMap.bmp");
    assert (height_map != NULL);
    height_map_size = sfImage_getSize (height_map);
    height_map_values = sfImage_getPixelsPtr(height_map);
    
    int i, j;
    
    for (i = 0; i < HEIGHT_MAP_SIZE; i++){
        for (j = 0; j < HEIGHT_MAP_SIZE; j++) {
            if (i >= height_map_size.x || j >= height_map_size.y){
                elevations[i][j] = 0;
            } else {
                elevations[i][j] = ELEVATION_SCALE * height_map_values[(j + i * height_map_size.x) * 4];
                //*4 because we want the R coodinate of RGBA values.
                //y -> i, x -> j
            }
        }
    }
    
    sfImage_destroy (height_map);

}

Float_3d Normalise_Float_3d (Float_3d foo) {

    float absolute_value = sqrt(foo.i*foo.i + foo.j*foo.j + foo.k*foo.k);

    foo.i /= absolute_value;
    foo.j /= absolute_value;
    foo.k /= absolute_value;

    return foo;
}

int tile_coords_get_elevation(int i, int j){
    i = i + 1;
    j = j + 1;
    
    
    if (i < 0 || j < 0 || i > HEIGHT_MAP_SIZE || j > HEIGHT_MAP_SIZE){ //TODO > or >=
        printf("You're not supposed to be here\n");
        return 0;
    }
    return elevations[i][j];
}

int floordiv (int a , int b){

if (b == 0) printf("div 0\n");

    int c = a / b;
    
    if (a % b == 0) return c;
    
    if ((a < 0 && b > 0) || (a > 0 && b < 0)) return (c - 1);
    
    return c;

}

Tile_Coords map_to_tile (Map_Coords map){
//we want to divide by tile size in points, always rounding down.

    Tile_Coords tile;
    
    tile.i = floordiv (map.i, POINTS_PER_PIXEL * PIXELS_PER_TILE);
    tile.j = floordiv (map.j, POINTS_PER_PIXEL * PIXELS_PER_TILE);
    tile.k = map.k;
    
    return tile;
}

Screen_Coords map_to_screen_top_left (Map_Coords mc){

    Screen_Coords sc;
    sc.x = (mc.i + mc.j) / (1.0 * POINTS_PER_PIXEL);
    sc.y = (mc.i - mc.j) / (ISOMETRIC_FACTOR * POINTS_PER_PIXEL)
           - mc.k / (1.0 * POINTS_PER_PIXEL);
           
    return sc;
}

Screen_Coords map_to_screen_centre_viewpoint (Map_Coords mc){

    Screen_Coords sc;
    sc.x = VIEWPORT_WIDTH/2.0
           + (mc.i - viewpoint.i) / (1.0 * POINTS_PER_PIXEL)
           + (mc.j - viewpoint.j) / (1.0 * POINTS_PER_PIXEL);
           
    sc.y = VIEWPORT_HEIGHT/2.0
           + (mc.i - viewpoint.i) / (ISOMETRIC_FACTOR * POINTS_PER_PIXEL)
           - (mc.j - viewpoint.j) / (ISOMETRIC_FACTOR * POINTS_PER_PIXEL)
           - (mc.k - viewpoint.k) / (1.0 * POINTS_PER_PIXEL);
           
    return sc;
}

Screen_Coords tile_to_screen_centre_viewpoint (Tile_Coords TC){

    
    
    int mci = TC.i * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    int mcj = TC.j * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    int mck = TC.k;
    
    Screen_Coords sc;
    sc.x = VIEWPORT_WIDTH/2.0
           + (mci - viewpoint.i) / (1.0 * POINTS_PER_PIXEL)
           + (mcj - viewpoint.j) / (1.0 * POINTS_PER_PIXEL);
           
    sc.y = VIEWPORT_HEIGHT/2.0
           + (mci - viewpoint.i) / (ISOMETRIC_FACTOR * POINTS_PER_PIXEL)
           - (mcj - viewpoint.j) / (ISOMETRIC_FACTOR * POINTS_PER_PIXEL)
           - (mck - viewpoint.k) / (1.0 * POINTS_PER_PIXEL);
           
    return sc;
}

Screen_Coords subtile_to_screen_centre_square(int tile_i, int tile_j, int square_i, int square_j){

    int k = elevations[tile_i + square_i * TILES_PER_SQUARE + 1]
                              [tile_j + square_j * TILES_PER_SQUARE + 1];
    //printf("k = %d\n", k);
                              
    int i, j;
    
    i = tile_i * PIXELS_PER_TILE;
    j = tile_j * PIXELS_PER_TILE;
    
    Screen_Coords SC;
    
    SC.x = i + j;
    SC.y = i / ISOMETRIC_FACTOR - j / ISOMETRIC_FACTOR - k / (1.0 * POINTS_PER_PIXEL);
    
    return SC;
   
}



Map_Coords screen_to_map_k_0 (Screen_Coords sc) {

    Map_Coords mc;
    mc.k = 0;
     
    mc.i = (sc.x - VIEWPORT_WIDTH/2.0) * POINTS_PER_PIXEL / 2 
         + (sc.y - VIEWPORT_HEIGHT/2.0) * POINTS_PER_PIXEL * ISOMETRIC_FACTOR / 2
         + (mc.k - viewpoint.k) * ISOMETRIC_FACTOR / 2
         + viewpoint.i;
     
    mc.j = (sc.x - VIEWPORT_WIDTH/2.0) * POINTS_PER_PIXEL / 2
         - (sc.y - VIEWPORT_HEIGHT/2.0) * POINTS_PER_PIXEL * ISOMETRIC_FACTOR / 2
         - (mc.k - viewpoint.k) * ISOMETRIC_FACTOR / 2
         + viewpoint.j;
          
    return mc;
}

Map_Coords screen_to_map_k_max (Screen_Coords sc) {

    Map_Coords mc;
    mc.k = ELEVATION_MAX;
     
    mc.i = (sc.x - VIEWPORT_WIDTH/2.0) * POINTS_PER_PIXEL / 2 
         + (sc.y - VIEWPORT_HEIGHT/2.0) * POINTS_PER_PIXEL * ISOMETRIC_FACTOR / 2
         + (mc.k - viewpoint.k) * ISOMETRIC_FACTOR / 2
         + viewpoint.i;
     
    mc.j = (sc.x - VIEWPORT_WIDTH/2.0) * POINTS_PER_PIXEL / 2
         - (sc.y - VIEWPORT_HEIGHT/2.0) * POINTS_PER_PIXEL * ISOMETRIC_FACTOR / 2
         - (mc.k - viewpoint.k) * ISOMETRIC_FACTOR / 2
         + viewpoint.j;
          
    return mc;
}

int interpolate_elevation ( int i, int j){

    int tile_i, tile_j;
    int residual_i, residual_j;
    float normalised_residual_i, normalised_residual_j;
    

    
    tile_i = i / (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    tile_j = j / (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    
    residual_i = i % (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    residual_j = j % (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    
    if (residual_i == 0 && residual_j == 0) {
        return tile_coords_get_elevation(tile_i, tile_j);
    }
    
    normalised_residual_i = 
         residual_i / (1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));
         
    normalised_residual_j = 
         residual_j / (1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));
         
    int left_elevation = tile_coords_get_elevation(tile_i, tile_j);
    int middle_elevation;
    int right_elevation = tile_coords_get_elevation(tile_i + 1, tile_j);
    int elevation;
         
    if (residual_i <= residual_j){  //Upper triangle of tile
    
        middle_elevation = tile_coords_get_elevation(tile_i, tile_j + 1);
        
        elevation =
            (right_elevation - middle_elevation) * normalised_residual_i
            + (middle_elevation - left_elevation) * normalised_residual_j
            + left_elevation;
            
        return elevation;
        
    } else { //Lower triangle of tile
    
        middle_elevation = tile_coords_get_elevation(tile_i + 1, tile_j);
                
        elevation =
            (middle_elevation - left_elevation) * normalised_residual_i
            + (right_elevation - middle_elevation) * normalised_residual_j
            + left_elevation;
            
        return elevation;
        
    }
}

int map_coords_update_elevations(Map_Coords* map_coords){

    map_coords->k = interpolate_elevation(map_coords->i, map_coords->j);
    return 1;
}

float signed_area (Screen_Coords p1, Screen_Coords p2, Screen_Coords p3) {
//positive is clockwise

    float x1, y1, x2, y2;
    
    x1 = p2.x - p1.x;
    y1 = p2.y - p1.y;
    
    x2 = p3.x - p2.x;
    y2 = p3.y - p2.y;
    
    float area = (x1 * y2 - x2 * y1) / 2;
    
    return area;
}

int point_within_triangle_screen_coords (Screen_Coords point, Screen_Coords vertex1, Screen_Coords vertex2, Screen_Coords vertex3){

    float area = signed_area (vertex1, vertex2, vertex3);

    if (area == 0) return 0; //Its a surprise tool that will help us later

    //if (vertex1, vertex2, vertex3) are clockwise and point is within
    // (vertex1, vertex2, vertex3), 
    // (vertex1, vertex2, point) will be clockwise
    // (vertex2, vertex3, point) will be clockwise
    // (xertex3, vertex1, point) will be clockwise


    //if (vertex1, vertex2, vertex3) are counter-clockwise and point is within
    // (vertex1, vertex2, vertex3), 
    // (vertex1, vertex2, point) will be counter-clockwise
    // (vertex2, vertex3, point) will be counter-clockwise
    // (xertex3, vertex1, point) will be counter-clockwise


    float trig1 = signed_area (vertex1, vertex2, point);
    float trig2 = signed_area (vertex2, vertex3, point);
    float trig3 = signed_area (vertex3, vertex1, point);

    if (trig1 * area >= 0 && trig2 * area >= 0 && trig3 * area >= 0) {
        return 1;
    }

    return 0;
}

int point_within_triange_map_coords (Screen_Coords p, Map_Coords vertex1, Map_Coords vertex2, Map_Coords vertex3){

    Screen_Coords v1, v2, v3;
    
    v1 = map_to_screen_centre_viewpoint(vertex1);
    v2 = map_to_screen_centre_viewpoint(vertex2);
    v3 = map_to_screen_centre_viewpoint(vertex3);

    return point_within_triangle_screen_coords(p, v1, v2, v3);
}

Map_Coords interpolate_map_coords (Screen_Coords p, Map_Coords vertex1, Map_Coords vertex2, Map_Coords vertex3){

    Screen_Coords v1, v2, v3;
    
    v1 = map_to_screen_centre_viewpoint(vertex1);
    v2 = map_to_screen_centre_viewpoint(vertex2);
    v3 = map_to_screen_centre_viewpoint(vertex3);
    
    Map_Coords point;

    float det1, det2, det3, det4;
    
    det1 = vertex1.i * v2.y + vertex2.i * v3.y + vertex3.i * v1.y
         - vertex1.i * v3.y - vertex2.i * v1.y - vertex3.i * v2.y;
    
    det2 = v1.x * vertex2.i + v2.x * vertex3.i + v3.x * vertex1.i
         - v1.x * vertex3.i - v2.x * vertex1.i - v3.x * vertex2.i;
         
    det3 = v1.x * v2.y * vertex3.i + v2.x * v3.y * vertex1.i + v3.x * v1.y * vertex2.i
         - v1.x * v3.y * vertex2.i - v2.x * v1.y * vertex3.i - v3.x * v2.y * vertex1.i   ;  
         
    
    det4 = v1.x * v2.y + v2.x * v3.y + v3.x * v1.y
         - v1.x * v3.y - v2.x * v1.y - v3.x * v2.y;
         
    assert(det4 != 0);
    
    point.i = (det1 * p.x + det2 * p.y + det3) / det4;
    
    
    
    det1 = vertex1.j * v2.y + vertex2.j * v3.y + vertex3.j * v1.y
         - vertex1.j * v3.y - vertex2.j * v1.y - vertex3.j * v2.y;
    
    det2 = v1.x * vertex2.j + v2.x * vertex3.j + v3.x * vertex1.j
         - v1.x * vertex3.j - v2.x * vertex1.j - v3.x * vertex2.j;
         
    det3 = v1.x * v2.y * vertex3.j + v2.x * v3.y * vertex1.j + v3.x * v1.y * vertex2.j
         - v1.x * v3.y * vertex2.j - v2.x * v1.y * vertex3.j - v3.x * v2.y * vertex1.j;
         
    
    //det4 = v1.x * v2.y + v2.x * v3.y + v3.x * v1.y
    //     - v1.x * v3.y - v2.x * v1.y - v3.x * v2.y;
         
    //assert(det4 != 0);
    
    point.j = (det1 * p.x + det2 * p.y + det3) / det4;
    
    
    
    det1 = vertex1.k * v2.y + vertex2.k * v3.y + vertex3.k * v1.y
         - vertex1.k * v3.y - vertex2.k * v1.y - vertex3.k * v2.y;
    
    det2 = v1.x * vertex2.k + v2.x * vertex3.k + v3.x * vertex1.k
         - v1.x * vertex3.k - v2.x * vertex1.k - v3.x * vertex2.k;
         
    det3 = v1.x * v2.y * vertex3.k + v2.x * v3.y * vertex1.k + v3.x * v1.y * vertex2.k
         - v1.x * v3.y * vertex2.k - v2.x * v1.y * vertex3.k - v3.x * v2.y * vertex1.k;     
         
    
    //det4 = v1.x * v2.y + v2.x * v3.y + v3.x * v1.y
    //     - v1.x * v3.y - v2.x * v1.y - v3.x * v2.y;
         
    //assert(det4 != 0);
    
    point.k = (det1 * p.x + det2 * p.y + det3) / det4;
    
    return point;
}

int point_within_tile (Tile_Coords tile, Screen_Coords p) {
    // is the point in the northern or southern half of the tile being examined,
    // or neither.

    Map_Coords left_vertex, top_vertex, right_vertex, bottom_vertex;
    
    left_vertex.i = tile.i * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    left_vertex.j = tile.j * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    left_vertex.k = tile_coords_get_elevation(tile.i, tile.j);
    
    top_vertex.i = tile.i * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    top_vertex.j = (tile.j + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    top_vertex.k = tile_coords_get_elevation(tile.i, tile.j + 1);
    
    right_vertex.i = (tile.i + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    right_vertex.j = (tile.j + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    right_vertex.k = tile_coords_get_elevation(tile.i + 1, tile.j + 1);
    
    bottom_vertex.i = (tile.i + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    bottom_vertex.j = tile.j * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    bottom_vertex.k = tile_coords_get_elevation(tile.i + 1, tile.j);


    if (point_within_triange_map_coords (p, left_vertex, top_vertex, right_vertex) != 0){
        return 1; //Top
    }

    if (point_within_triange_map_coords (p, left_vertex, bottom_vertex, right_vertex ) != 0){
        return 2; //Bottom
    }


    
    return 0;
}

int left_or_right_of_tile (Map_Coords point) {

    int residual_i = point.i % (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    int residual_j = point.j % (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    
    if (residual_i + residual_j < POINTS_PER_PIXEL * PIXELS_PER_TILE){
        return 1; // Left
    } else {
        return 0; // Right
    }
}

Map_Coords screen_to_map_centre_viewpoint (Screen_Coords point){

    Map_Coords point_max_k = screen_to_map_k_max(point);
    // the highest point on the map that could correspond to the point on the screen

    Map_Coords point_0_k = screen_to_map_k_0(point);
    // the lowest point on the map that could correspond to the point on the screen
    
    int point_left_or_right = left_or_right_of_tile (point_max_k);
    // is the point to the left or right of the middle of the first tile examined
    
    int point_top_or_bottom;
    // is the point in the northern or southern half of the tile being examined,
    // or neither.
    
    Tile_Coords tile_point_0_k = map_to_tile (point_0_k);
    Tile_Coords tile =  map_to_tile (point_max_k);

    if (point_left_or_right == 1){
    //point is on the left side of the first tile we look at;
   
        while (tile.j <= tile_point_0_k.j){

            point_top_or_bottom = point_within_tile(tile, point);


            if (point_top_or_bottom != 0) break;
            
            tile.i--;
        
            point_top_or_bottom = point_within_tile(tile, point);
            
            if (point_top_or_bottom != 0) break;
            
            tile.j++;
        
        }
    
    } else {
    //point is on the right side of the first tile we look at;
   
        while (tile.j <= tile_point_0_k.j){
        

      
            point_top_or_bottom = point_within_tile(tile, point);

           
            if (point_top_or_bottom != 0) break;
            
            tile.j++;
        
            point_top_or_bottom = point_within_tile(tile, point);
            
            if (point_top_or_bottom != 0) break;
            
            tile.i--;
        
        }
    
    }
    assert (point_top_or_bottom != 0);

    Map_Coords left_vertex, middle_vertex, right_vertex;
    
    left_vertex.i = tile.i * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    left_vertex.j = tile.j * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    
    right_vertex.i = (tile.i + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    right_vertex.j = (tile.j + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    

    if (point_top_or_bottom == 1){ //Top
    
        middle_vertex.i = tile.i * POINTS_PER_PIXEL * PIXELS_PER_TILE;
        middle_vertex.j = (tile.j + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    
    } else { //bottom
        
        middle_vertex.i = (tile.i + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
        middle_vertex.j = tile.j * POINTS_PER_PIXEL * PIXELS_PER_TILE;
       
    }
    
    map_coords_update_elevations(&left_vertex);
    map_coords_update_elevations(&middle_vertex);
    map_coords_update_elevations(&right_vertex);
    
    Map_Coords foo; 
    foo = interpolate_map_coords (point, left_vertex, middle_vertex, right_vertex);
    return foo;
}

int update_bicubic_coefficients(Tile_Coords tile){  //TODO untested

    bicubic_coefficients_tile_i = tile.i;
    bicubic_coefficients_tile_j = tile.j;
    
    
    float height_values[4][4];
    
    height_values[0][0] = tile_coords_get_elevation(tile.i - 1, tile.j - 1);
    height_values[0][1] = tile_coords_get_elevation(tile.i - 1, tile.j);
    height_values[0][2] = tile_coords_get_elevation(tile.i - 1, tile.j + 1);
    height_values[0][3] = tile_coords_get_elevation(tile.i - 1, tile.j + 2);
    
    height_values[1][0] = tile_coords_get_elevation(tile.i, tile.j - 1);
    height_values[1][1] = tile_coords_get_elevation(tile.i, tile.j);
    height_values[1][2] = tile_coords_get_elevation(tile.i, tile.j + 1);
    height_values[1][3] = tile_coords_get_elevation(tile.i, tile.j + 2);
    
    height_values[2][0] = tile_coords_get_elevation(tile.i + 1, tile.j - 1);
    height_values[2][1] = tile_coords_get_elevation(tile.i + 1, tile.j);
    height_values[2][2] = tile_coords_get_elevation(tile.i + 1, tile.j + 1);
    height_values[2][3] = tile_coords_get_elevation(tile.i + 1, tile.j + 2);
    
    height_values[3][0] = tile_coords_get_elevation(tile.i + 2, tile.j - 1);
    height_values[3][1] = tile_coords_get_elevation(tile.i + 2, tile.j);
    height_values[3][2] = tile_coords_get_elevation(tile.i + 2, tile.j + 1);
    height_values[3][3] = tile_coords_get_elevation(tile.i + 2, tile.j + 2);
    
    
    
    float left_matrix[4][4];
    
    left_matrix[0][0] = 0.0;
    left_matrix[0][1] = 1.0;
    left_matrix[0][2] = 0.0;
    left_matrix[0][3] = 0.0;
    
    left_matrix[1][0] = -1.0/3.0;
    left_matrix[1][1] = -1.0/2.0;
    left_matrix[1][2] =  1.0;
    left_matrix[1][3] = -1.0/6.0;
    
    left_matrix[2][0] =  1.0/2.0;
    left_matrix[2][1] = -1.0;
    left_matrix[2][2] =  1.0/2.0;
    left_matrix[2][3] =  0.0;
    
    left_matrix[3][0] = -1.0/6.0;
    left_matrix[3][1] =  1.0/2.0;
    left_matrix[3][2] = -1.0/2.0;
    left_matrix[3][3] =  1.0/6.0;
    
    float right_matrix[4][4];
    
    right_matrix[0][0] =  0.0;
    right_matrix[0][1] = -1.0/3.0;
    right_matrix[0][2] =  1.0/2.0;
    right_matrix[0][3] = -1.0/6.0;
    
    right_matrix[1][0] =  1.0;
    right_matrix[1][1] = -1.0/2.0;
    right_matrix[1][2] = -1.0;
    right_matrix[1][3] =  1.0/2.0;
    
    right_matrix[2][0] =  0.0;
    right_matrix[2][1] =  1.0;
    right_matrix[2][2] =  1.0/2.0;
    right_matrix[2][3] = -1.0/2.0;
    
    right_matrix[3][0] =  0.0;
    right_matrix[3][1] = -1.0/6.0;
    right_matrix[3][2] =  0.0;
    right_matrix[3][3] =  1.0/6.0;

    float intermediate_matrix[4][4];
    
    
    matrix_multiply(left_matrix, height_values, intermediate_matrix);
    
    
    matrix_multiply(intermediate_matrix, right_matrix, bicubic_coefficients);
    
    //printf("\n height values\n");
    //print_matrix(height_values);
    //printf("\n bicubic coefficients\n");
    //print_matrix(bicubic_coefficients);
           
}

int map_coords_get_elevation_bicubic(Map_Coords mc){  //TODO untested

    Tile_Coords tc = map_to_tile (mc);
    
    assert(tc.i >= 0 && tc.j >= 0);
    

    
    
    int residual_i, residual_j;
    float normalised_residual_i, normalised_residual_j;
    
    residual_i = mc.i % (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    residual_j = mc.j % (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    
    if (residual_i == 0 && residual_j == 0) {
        return tile_coords_get_elevation(tc.i, tc.j);
    }
    
    if (bicubic_coefficients_tile_i != tc.i
     || bicubic_coefficients_tile_j != tc.j){
        update_bicubic_coefficients(tc);
    }
    
    normalised_residual_i = 
         (residual_i * 1.0) / (1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));
         
    normalised_residual_j = 
         (residual_j * 1.0) / (1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));
         
    float x = normalised_residual_i;
    float x2 = x * x;
    float x3 = x2 * x;
    
    float y = normalised_residual_j;
    float y2 = y * y;
    float y3 = y2 * y;
    

    return (
           bicubic_coefficients[0][0]
         + bicubic_coefficients[0][1] * y
         + bicubic_coefficients[0][2] * y2
         + bicubic_coefficients[0][3] * y3
         
         
         + (bicubic_coefficients[1][0]
          + bicubic_coefficients[1][1] * y 
          + bicubic_coefficients[1][2] * y2 
          + bicubic_coefficients[1][3] * y3) * x
          
         + (bicubic_coefficients[2][0] 
         + bicubic_coefficients[2][1] * y 
         + bicubic_coefficients[2][2] * y2 
         + bicubic_coefficients[2][3] * y3) * x2
         
         + (bicubic_coefficients[3][0]
         + bicubic_coefficients[3][1] * y 
         + bicubic_coefficients[3][2] * y2 
         + bicubic_coefficients[3][3] * y3) * x3
         
         );

}

Float_3d map_coords_get_normal_bicubic(Map_Coords mc){  //TODO untested

    Tile_Coords tc = map_to_tile (mc);
    
    assert(tc.i >= 0 && tc.j >= 0);
    
    if (bicubic_coefficients_tile_i != tc.i
     || bicubic_coefficients_tile_j != tc.j){
//printf("tile_i = %d, tile_j = %d\n", tc.i, tc.j);
        update_bicubic_coefficients(tc);
    }
    
    
    int residual_i, residual_j;
    float normalised_residual_i, normalised_residual_j;
    
    residual_i = mc.i % (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    residual_j = mc.j % (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    
    normalised_residual_i = 
         residual_i / (1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));
    
    normalised_residual_j = 
         residual_j / (1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));
         
    float x = normalised_residual_i;
    float x2 = x * x;
    float x3 = x2 * x;
    
    float y = normalised_residual_j;
    float y2 = y * y;
    float y3 = y2 * y;
    
    float partialx = (
          (bicubic_coefficients[0][0]
         + bicubic_coefficients[0][1] * y
         + bicubic_coefficients[0][2] * y2
         + bicubic_coefficients[0][3] * y3) * 0.0
         
         
         +(bicubic_coefficients[1][0]
         + bicubic_coefficients[1][1] * y 
         + bicubic_coefficients[1][2] * y2 
         + bicubic_coefficients[1][3] * y3) * 1.0
          
         +(bicubic_coefficients[2][0] 
         + bicubic_coefficients[2][1] * y 
         + bicubic_coefficients[2][2] * y2 
         + bicubic_coefficients[2][3] * y3) * 2.0 * x
         
         +(bicubic_coefficients[3][0]
         + bicubic_coefficients[3][1] * y 
         + bicubic_coefficients[3][2] * y2 
         + bicubic_coefficients[3][3] * y3) * 3.0 * x2
         
         ) / (1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));
         //TODO May wish to multiply by a constant because scales and angles are not exactly isometric
    
    float partialy = (
          (bicubic_coefficients[0][0] * 0.0
         + bicubic_coefficients[0][1] * 1.0
         + bicubic_coefficients[0][2] * 2.0 * y
         + bicubic_coefficients[0][3] * 3.0 * y2)
         
         
         +(bicubic_coefficients[1][0] * 0.0
         + bicubic_coefficients[1][1] * 1.0 
         + bicubic_coefficients[1][2] * 2.0 * y
         + bicubic_coefficients[1][3] * 3.0 * y2) * x
          
         +(bicubic_coefficients[2][0] * 0.0
         + bicubic_coefficients[2][1] * 1.0 
         + bicubic_coefficients[2][2] * 2.0 * y 
         + bicubic_coefficients[2][3] * 3.0 * y2) * x2
         
         +(bicubic_coefficients[3][0] * 0.0
         + bicubic_coefficients[3][1] * 1.0 
         + bicubic_coefficients[3][2] * 2.0 * y 
         + bicubic_coefficients[3][3] * 3.0 * y2) * x3
         
         ) / (1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));
         //TODO May wish to multiply by a constant because scales and angles are not exactly isometric
         
         Float_3d surface_normal;
         
         surface_normal.i = -partialx; //-df/di
         surface_normal.j = -partialy; //-df/dj
         surface_normal.k = 1;
         
//printf("surface_normal (i, j, k) = (%f, %f, %f)\n", surface_normal.i, surface_normal.j, surface_normal.k);
         
         return (Normalise_Float_3d(surface_normal));

}
