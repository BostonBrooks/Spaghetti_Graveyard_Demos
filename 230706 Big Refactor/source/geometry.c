#define GEOMETRY

//-----------------------------INCLUDES----------------------------//

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif



//-----------------------------STRUCTS------------------------------//

typedef struct {
    float x;         //x axis is the number of pixels to the right of top left
    float y;         //y axis is down number of pixels down from top left
} bbScreenCoords;

typedef struct {
    int i;           //i axis points down and to the right, in units of points
    int j;           //j axis points up and to the right, in units of points
    int k;           //k axis points up, in units of points, negative indicates invalid
} bbMapCoords;

typedef struct {
    int i;           //i axis points down and to the right, in units of tiles
    int j;           //j axis points up and to the right, in units of tiles
    int k;           //k axis points up, in units of points, negative indicates invalid
} bbTileCoords;

typedef struct {
    int i;           //i axis points down and to the right, in units of tiles
    int j;           //j axis points up and to the right, in units of tiles
    int k;           //k axis points up, in units of points, negative indicates invalid
} bbSquareCoords;


typedef struct {
    float i;           //i axis points down and to the right, in whatever units
    float j;           //j axis points up and to the right, in whatever units
    float k;           //k axis points up, whatever units
} bbFloat3D;


typedef struct {       //used to store the hit box of a drawable relative to the drawable's location on screen.

    float top;
    float left;
    float height;
    float width;
    

} bbFloatRect;



//-----------------------------GLOBALS------------------------------//

int bbElevations[HEIGHT_MAP_SIZE][HEIGHT_MAP_SIZE];
//all of the vertices of map tiles plus a buffer defined by HEIGHT_MAP_PADDING

bbMapCoords viewpoint;

bbFloat3D LightIncedence;

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

int bbScreenCoords_print(bbScreenCoords sc){
    printf("x = %f, y = %f\n", sc.x, sc.y);
}

int bbMapCoords_print(bbMapCoords mc){
    printf("i = %d, j = %d, k = %d\n", mc.i, mc.j, mc.k);
}

int bbElevations_load(){

    sfImage*   height_map;
    sfVector2u height_map_size;
    sfUint8*   height_map_values;
    
    height_map = sfImage_createFromFile("./media/HeightMap.bmp");
    assert (height_map != NULL);
    height_map_size = sfImage_getSize (height_map);
    height_map_values = sfImage_getPixelsPtr(height_map);
    
    int i, j;
    
    for (i = 0; i < HEIGHT_MAP_SIZE; i++){
        for (j = 0; j < HEIGHT_MAP_SIZE; j++) {
            if (i >= height_map_size.x || j >= height_map_size.y){
                bbElevations[i][j] = 0;
            } else {
                bbElevations[i][j] = ELEVATION_SCALE * height_map_values[(j + i * height_map_size.x) * 4];
                //*4 because we want the R coodinate of RGBA values.
                //y -> i, x -> j
            }
        }
    }
    
    sfImage_destroy (height_map);

}

bbFloat3D bbFloat3D_normalise (bbFloat3D foo) {

    float absolute_value = sqrt(foo.i*foo.i + foo.j*foo.j + foo.k*foo.k);

    foo.i /= absolute_value;
    foo.j /= absolute_value;
    foo.k /= absolute_value;

    return foo;
}

int bbTileCoords_getElevation(int i, int j){
    i = i + HEIGHT_MAP_PADDING;
    j = j + HEIGHT_MAP_PADDING;
    
    
    if (i < 0 || j < 0 || i > HEIGHT_MAP_SIZE || j > HEIGHT_MAP_SIZE){ 
        return 0;
        //elevation out of bounds
    }
    return bbElevations[i][j];
}

int floordiv (int a , int b){

    if (b == 0) printf("divide by 0\n");

    int c = a / b;
    
    if (a % b == 0) return c;
    
    if ((a < 0 && b > 0) || (a > 0 && b < 0)) return (c - 1);
    
    return c;

}

bbTileCoords bbMapCoords_getTileCoords (bbMapCoords map){
//we want to divide by tile size in points, always rounding down.

    bbTileCoords tile;
    
    tile.i = floordiv (map.i, POINTS_PER_TILE);
    tile.j = floordiv (map.j, POINTS_PER_TILE);
    tile.k = map.k;
    
    return tile;
}
bbSquareCoords bbMapCoords_getSquareCoords (bbMapCoords map){
//we want to divide by tile size in points, always rounding down.

    bbSquareCoords square;
    
    square.i = floordiv (map.i, POINTS_PER_SQUARE);
    square.j = floordiv (map.j, POINTS_PER_SQUARE);
    square.k = map.k;
    
    return square;
}


bbScreenCoords bbMapCoords_getScreenCoords_topleft (bbMapCoords mc){

    bbScreenCoords sc;
    sc.x = (mc.i + mc.j) / (1.0 * POINTS_PER_PIXEL);
    sc.y = (mc.i - mc.j) / (1.0 * ISOMETRIC_FACTOR * POINTS_PER_PIXEL)
           - mc.k / (1.0 * POINTS_PER_PIXEL);
           
    return sc;
}

bbScreenCoords bbMapCoords_getScreenCoords_centre (bbMapCoords mc){

    bbScreenCoords sc;
    sc.x = VIEWPORT_WIDTH/2.0
           + (mc.i - viewpoint.i) / (1.0 * POINTS_PER_PIXEL)
           + (mc.j - viewpoint.j) / (1.0 * POINTS_PER_PIXEL);
           
    sc.y = VIEWPORT_HEIGHT/2.0
           + (mc.i - viewpoint.i) / (1.0 * ISOMETRIC_FACTOR * POINTS_PER_PIXEL)
           - (mc.j - viewpoint.j) / (1.0 * ISOMETRIC_FACTOR * POINTS_PER_PIXEL)
           - (mc.k - viewpoint.k) / (1.0 * POINTS_PER_PIXEL);
           
    return sc;
}

bbScreenCoords bbTileCoords_getScreenCoords_centre (bbTileCoords TC){

    
    
    int mci = TC.i * POINTS_PER_TILE;
    int mcj = TC.j * POINTS_PER_TILE;
    int mck = TC.k;
    
    bbScreenCoords sc;
    sc.x = VIEWPORT_WIDTH/2.0
           + (mci - viewpoint.i) / (1.0 * POINTS_PER_PIXEL)
           + (mcj - viewpoint.j) / (1.0 * POINTS_PER_PIXEL);
           
    sc.y = VIEWPORT_HEIGHT/2.0
           + (mci - viewpoint.i) / (1.0 * ISOMETRIC_FACTOR * POINTS_PER_PIXEL)
           - (mcj - viewpoint.j) / (1.0 * ISOMETRIC_FACTOR * POINTS_PER_PIXEL)
           - (mck - viewpoint.k) / (1.0 * POINTS_PER_PIXEL);
           
    return sc;
}

bbScreenCoords bbGetVertex(int tile_i, int tile_j, int square_i, int square_j){

    int k = bbTileCoords_getElevation(tile_i + square_i * TILES_PER_SQUARE, tile_j + square_j * TILES_PER_SQUARE);
    
    int i, j;
    
    i = tile_i * PIXELS_PER_TILE;
    j = tile_j * PIXELS_PER_TILE;
    
    bbScreenCoords SC;
    
    SC.x = i + j;
    SC.y = i / ISOMETRIC_FACTOR - j / ISOMETRIC_FACTOR - k / (1.0 * POINTS_PER_PIXEL);
    
    return SC;
   
}

//In the following code, we have a lot of /2's , what would happen if we made them /2.0's

bbMapCoords bbScreenCoords_getMapCoords_k0 (bbScreenCoords sc) {

    bbMapCoords mc;
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

bbMapCoords bbScreenCoords_getMapCoords_kmax (bbScreenCoords sc) {

    bbMapCoords mc;
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

int bbElevations_interpolate ( int i, int j){
//calculates the elevation given i and j coordinates

    int tile_i, tile_j;
    int residual_i, residual_j;
    float normalised_residual_i, normalised_residual_j;
    

    
    tile_i = i / (POINTS_PER_TILE);
    tile_j = j / (POINTS_PER_TILE);
    
    residual_i = i % (POINTS_PER_TILE);
    residual_j = j % (POINTS_PER_TILE);
    
    if (residual_i == 0 && residual_j == 0) {
        return bbTileCoords_getElevation(tile_i, tile_j);
    }
    
    normalised_residual_i = 
         residual_i / (1.0 * (POINTS_PER_TILE));
         
    normalised_residual_j = 
         residual_j / (1.0 * (POINTS_PER_TILE));
         
    int left_elevation = bbTileCoords_getElevation(tile_i, tile_j);
    int middle_elevation;
    int right_elevation = bbTileCoords_getElevation(tile_i + 1, tile_j);
    int elevation;
         
    if (residual_i <= residual_j){  //Upper triangle of tile
    
        middle_elevation = bbTileCoords_getElevation(tile_i, tile_j + 1);
        
        elevation =
            (right_elevation - middle_elevation) * normalised_residual_i
            + (middle_elevation - left_elevation) * normalised_residual_j
            + left_elevation;
            
        return elevation;
        
    } else { //Lower triangle of tile
    
        middle_elevation = bbTileCoords_getElevation(tile_i + 1, tile_j);
                
        elevation =
            (middle_elevation - left_elevation) * normalised_residual_i
            + (right_elevation - middle_elevation) * normalised_residual_j
            + left_elevation;
            
        return elevation;
        
    }
}

int bbMapCoords_updateElevation(bbMapCoords* map_coords){

    map_coords->k = bbElevations_interpolate(map_coords->i, map_coords->j);
    return 1;
}

float signed_area (bbScreenCoords p1, bbScreenCoords p2, bbScreenCoords p3) {
//positive is clockwise

    float x1, y1, x2, y2;
    
    x1 = p2.x - p1.x;
    y1 = p2.y - p1.y;
    
    x2 = p3.x - p2.x;
    y2 = p3.y - p2.y;
    
    float area = (x1 * y2 - x2 * y1) / 2;
    
    return area;
}

int bbScreenCoords_withinTriangle_screen (bbScreenCoords point, bbScreenCoords vertex1, bbScreenCoords vertex2, bbScreenCoords vertex3){

    float area = signed_area (vertex1, vertex2, vertex3);

    if (area == 0) {
    
        #ifdef VERBOSE
        printf("Possible Error\n"); //May lead to exception
        #endif
    
        return 0;
    }

    //if (vertex1, vertex2, vertex3) are clockwise and point is within (vertex1, vertex2, vertex3), 
    // (vertex1, vertex2, point) will be clockwise
    // (vertex2, vertex3, point) will be clockwise
    // (xertex3, vertex1, point) will be clockwise


    //if (vertex1, vertex2, vertex3) are counter-clockwise and point is within (vertex1, vertex2, vertex3), 
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

int bbScreenCoords_withinTriangle_map (bbScreenCoords p, bbMapCoords vertex1, bbMapCoords vertex2, bbMapCoords vertex3){

    bbScreenCoords v1, v2, v3;
    
    v1 = bbMapCoords_getScreenCoords_centre(vertex1);
    v2 = bbMapCoords_getScreenCoords_centre(vertex2);
    v3 = bbMapCoords_getScreenCoords_centre(vertex3);

    return bbScreenCoords_withinTriangle_screen(p, v1, v2, v3);
}

bbMapCoords bbScreenCoords_interpolateMapCoords (bbScreenCoords p, bbMapCoords vertex1, bbMapCoords vertex2, bbMapCoords vertex3){

    bbScreenCoords v1, v2, v3;
    
    v1 = bbMapCoords_getScreenCoords_centre(vertex1);
    v2 = bbMapCoords_getScreenCoords_centre(vertex2);
    v3 = bbMapCoords_getScreenCoords_centre(vertex3);
    
    bbMapCoords point;

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

int bbScreenCoords_withinTile (bbTileCoords tile, bbScreenCoords p) {
    // is the point in the northern or southern half of the tile being examined,
    // or neither.

    bbMapCoords left_vertex, top_vertex, right_vertex, bottom_vertex;
    
    left_vertex.i = tile.i * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    left_vertex.j = tile.j * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    left_vertex.k = bbTileCoords_getElevation(tile.i, tile.j);
    
    top_vertex.i = tile.i * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    top_vertex.j = (tile.j + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    top_vertex.k = bbTileCoords_getElevation(tile.i, tile.j + 1);
    
    right_vertex.i = (tile.i + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    right_vertex.j = (tile.j + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    right_vertex.k = bbTileCoords_getElevation(tile.i + 1, tile.j + 1);
    
    bottom_vertex.i = (tile.i + 1) * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    bottom_vertex.j = tile.j * POINTS_PER_PIXEL * PIXELS_PER_TILE;
    bottom_vertex.k = bbTileCoords_getElevation(tile.i + 1, tile.j);



    if (bbScreenCoords_withinTriangle_map (p, left_vertex, top_vertex, right_vertex) != 0){
        return 1; //Top 
    }

    if (bbScreenCoords_withinTriangle_map (p, left_vertex, bottom_vertex, right_vertex ) != 0){
        return 2; //Bottom
    }

    #ifdef VERBOSE
    printf("Possible Error\n");
    #endif
    
    return 0;
}

int bbMapCoords_withinTile_LR (bbMapCoords point) {

    //TODO how does % operator work with negative values of point.i
    int residual_i = point.i % (POINTS_PER_TILE);
    int residual_j = point.j % (POINTS_PER_TILE);
    
    if (residual_i + residual_j < POINTS_PER_TILE){
        return 1; // Left
    } else {
        return 0; // Right
    }
}

bbMapCoords bbScreenCoords_getMapCoords (bbScreenCoords point){

    bbMapCoords point_max_k = bbScreenCoords_getMapCoords_kmax(point);
    // the highest point on the map that could correspond to the point on the screen

    bbMapCoords point_0_k = bbScreenCoords_getMapCoords_k0(point);
    // the lowest point on the map that could correspond to the point on the screen
    
    int point_left_or_right = bbMapCoords_withinTile_LR (point_max_k);
    // is the point to the left or right of the middle of the first tile examined
    
    int point_top_or_bottom;
    // is the point in the northern or southern half of the tile being examined,
    // or neither.
    
    bbTileCoords tile_point_0_k = bbMapCoords_getTileCoords (point_0_k);
    bbTileCoords tile =  bbMapCoords_getTileCoords (point_max_k);

    if (point_left_or_right == 1){
    //point is on the left side of the first tile we look at;
   
        while (tile.j <= tile_point_0_k.j){

            point_top_or_bottom = bbScreenCoords_withinTile(tile, point);


            if (point_top_or_bottom != 0) break;
            
            tile.i--;
        
            point_top_or_bottom = bbScreenCoords_withinTile(tile, point);
            
            if (point_top_or_bottom != 0) break;
            
            tile.j++;
        
        }
    
    } else {
    //point is on the right side of the first tile we look at;
   
        while (tile.j <= tile_point_0_k.j){
        

      
            point_top_or_bottom = bbScreenCoords_withinTile(tile, point);

           
            if (point_top_or_bottom != 0) break;
            
            tile.j++;
        
            point_top_or_bottom = bbScreenCoords_withinTile(tile, point);
            
            if (point_top_or_bottom != 0) break;
            
            tile.i--;
        
        }
    
    }
    //assert (point_top_or_bottom != 0);
    if (point_top_or_bottom == 0){
        #ifdef VERBOSE
        printf("culprit for many crashes\n");
        #endif
        
        bbMapCoords foo;
        foo.i = -1;
        foo.j = -1;
        foo.k=  -1;
    
        return foo;
    
    }

    bbMapCoords left_vertex, middle_vertex, right_vertex;
    
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
    
    bbMapCoords_updateElevation(&left_vertex);
    bbMapCoords_updateElevation(&middle_vertex);
    bbMapCoords_updateElevation(&right_vertex);
    
    bbMapCoords foo; 
    foo = bbScreenCoords_interpolateMapCoords (point, left_vertex, middle_vertex, right_vertex);
    return foo;
}


int bbMapCoords_isCloser(bbMapCoords a, bbMapCoords b){
//return 1 if a is closer to the screen than b , 0 otherwise

    int foo = a.i - a.j;
    int bar = b.i - b.j;
    
    if(foo > bar) return 1;
    return 0;
}

float bbMapCoords_getDistance(bbMapCoords a, bbMapCoords b){

    return sqrt((a.i - b.i)*(a.i - b.i)+(a.j - b.j)*(a.j - b.j));
}

