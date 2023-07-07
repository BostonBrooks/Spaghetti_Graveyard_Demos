#define POTENTIAL_FIELDS


//-----------------------------INCLUDES----------------------------//


#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif

#ifndef BBDRAWABLE
#define BBDRAWABLE
#include "../headers/bbDrawable.h"
#endif

#ifndef BBTERRAINSQUARE
#define BBTERRAINSQUARE
#include "../headers/bbTerrainSquare.h"
#endif

//-----------------------------GLOBALS----------------------------//

float (*potential_shape_vtable[NUMBER_OF_AVOIDANCE_SHAPES])(int drawable_A_int, int drawable_B_int, bbMapCoords test_point);

//-----------------------------CODE----------------------------//

float sigmoid_untuned(float x){

    float return_value;
    if (x > -2){
        return_value = -x / (1+fabs(x)) + 1;
    }else{
        //f(-2) = 1/3
        //f'(x) = -1/9
        //f(x) = (1-x) / 9
        
        return_value = (1-x)/9;
    
    }
    return(return_value);
}

float sigmoid(float distance){

  //Tune-able parameters
    float magnitude = 1;
    float stiffness = 5;
    float offset = 0;
    
    float return_value = magnitude * sigmoid_untuned(distance * stiffness + offset);
    

    return(return_value);

}


float Circular_Potential (int drawable_A_int, int drawable_B_int, bbMapCoords test_point){

//drawable_A is the object being repelled, drawable_B is the object doing the repelling,
//test_point is the hypothetical position where drawable_A is located.

    bbDrawable* drawable_A = bbDrawable_Pool_Lookup(drawable_A_int);
    bbDrawable* drawable_B = bbDrawable_Pool_Lookup(drawable_B_int);

    int drawable_A_i = drawable_A->location.i;
    int drawable_A_j = drawable_A->location.j;
    float drawable_A_radius = drawable_A->avoidance_radius;
    
    
    int drawable_B_i = drawable_B->location.i;
    int drawable_B_j = drawable_B->location.j;
    float drawable_B_radius = drawable_B->avoidance_radius;
    
    //TODO what if avoidance_radius = -1
    
    int delta_i = test_point.i - drawable_B_i;
    int delta_j = test_point.j - drawable_B_j;
    
    float distance =  sqrt(delta_i * delta_i + delta_j*delta_j);
    float gap = distance - drawable_A_radius - drawable_B_radius;
    
    float potential = sigmoid(gap);
    
    return potential;
    
    
}

float Square_Potential (int drawable_A_int, int drawable_B_int, bbMapCoords test_point){

//drawable_A is the object being repelled, drawable_B is the object doing the repelleing,
//test_point is the hypothetical position where drawable_A is located.

    return 0;
    
    
}


float Line_Potential (int drawable_A_int, int drawable_B_int, bbMapCoords test_point){

//drawable_A is the object being repelled, drawable_B is the object doing the repelleing,
//test_point is the hypothetical position where drawable_A is located.

    return 0;
    
    
}

float Rectangle_Potential (int drawable_A_int, int drawable_B_int, bbMapCoords test_point){

//drawable_A is the object being repelled, drawable_B is the object doing the repelleing,
//test_point is the hypothetical position where drawable_A is located.

    return 0;
    
    
}


float Polygon_Potential (int drawable_A_int, int drawable_B_int, bbMapCoords test_point){

//drawable_A is the object being repelled, drawable_B is the object doing the repelling,
//test_point is the hypothetical position where drawable_A is located.

    return 0;
    
    
}

int potential_avoidance_initShapes(void){

    potential_shape_vtable[AVOIDANCE_CIRCLULAR]    = Circular_Potential;
    potential_shape_vtable[AVOIDANCE_SQUARE]       = Square_Potential;
    potential_shape_vtable[AVOIDANCE_LINE]         = Line_Potential;
    potential_shape_vtable[AVOIDANCE_RECTANGLE]    = Rectangle_Potential;
    potential_shape_vtable[AVOIDANCE_POLYGON]      = Polygon_Potential;
    
    return 0;

}

float sum_potential_per_square(int drawable_A_int, bbMapCoords test_point, int Square_i, int Square_j){

//drawable_A is the object being repelled, drawable_B is the object doing the repelling,
//test_point is the hypothetical position where drawable_A is located.

    bbTerrainSquare* TS = &bbTerrainSquare_grid[Square_i][Square_j];
    bbDrawable* drawable_A = bbDrawable_Pool_Lookup(drawable_A_int);
    bbDrawable* drawable_B;
    
    int drawable_B_int =  TS->bbDrawable_head;
    
    float output = 0;
    float temp;
    
    if(drawable_B_int == -1) return output; //if the square is empty, return 0
    
    drawable_B =  bbDrawable_Pool_Lookup(drawable_B_int);
    
    while(drawable_B_int != -1){
    
        drawable_B = bbDrawable_Pool_Lookup(drawable_B_int);
    
        if(drawable_B_int != drawable_A_int){
        
            if(drawable_B->shape != AVOIDANCE_NULL){
            
                temp = potential_shape_vtable[drawable_B->shape]
                           (drawable_A_int, drawable_B_int, test_point);
//printf("temp = %f\n", output);                           
                output += temp;
            
            
            }
        
        
        }
        
        drawable_B_int = drawable_B->Square_Next;
    }
    
//printf("per square potential = %f\n", output);
    return output;
    

}

float sum_potential_Nearby(int drawable_A_int, bbMapCoords test_point){

    float tuneable_mass = 100;

    bbDrawable* drawable_A = bbDrawable_Pool_Lookup(drawable_A_int);
    
    bbSquareCoords SC = bbMapCoords_getSquareCoords(drawable_A->location);
    
    
    int i_min = SC.i-1;
    int j_min = SC.j-1;
    int i_max = SC.i+1;
    int j_max = SC.j+1;
    
    if (i_min < 0) i_min = 0;
    if (j_min < 0) j_min = 0;
    if (i_max >= SQUARES_PER_MAP ) i_max = SQUARES_PER_MAP -1;
    if (j_max >= SQUARES_PER_MAP ) j_max = SQUARES_PER_MAP -1;
    
    float output = 0;
    float temp;
    
    //distance^2 * mass
    output = ((((drawable_A->preferred_next_location.i) - test_point.i)*1.0/POINTS_PER_PIXEL)
           * (((drawable_A->preferred_next_location.i) - test_point.i)*1.0/POINTS_PER_PIXEL)
           
           + (((drawable_A->preferred_next_location.j) - test_point.j)*1.0/POINTS_PER_PIXEL)
           * (((drawable_A->preferred_next_location.j) - test_point.j)*1.0/POINTS_PER_PIXEL))
           /tuneable_mass * drawable_A->mass; 
    
    
    
    int Square_i, Square_j;
    for(Square_i = i_min; Square_i< i_max; Square_i++){
        for(Square_j = j_min; Square_j< j_max; Square_j++){
        
            temp = sum_potential_per_square(drawable_A_int, test_point, Square_i, Square_j);
            
            output += temp;
    
        }
    
    }

    
    return output;
    
    
}

int plot_potential(int drawable_int){

    int i, j;
    
    remove("potential.csv");
    FILE* fp = fopen("potential.csv", "w+");
    
    bbDrawable* drawable = bbDrawable_Pool_Lookup(drawable_int);
    
    bbSquareCoords SC = bbMapCoords_getSquareCoords(drawable->location);
    
    
    for (i = -PIXELS_PER_SQUARE; i < PIXELS_PER_SQUARE *2; i++){
    
        for (j = -PIXELS_PER_SQUARE; j < PIXELS_PER_SQUARE * 2; j++){
        
            bbMapCoords test_point, corner;
            
            
            test_point.i = SC.i * POINTS_PER_SQUARE + i * POINTS_PER_PIXEL;
            test_point.j = SC.j * POINTS_PER_SQUARE + j * POINTS_PER_PIXEL;
            
            float potential;
            
            potential = sum_potential_Nearby(drawable_int, test_point);
            
            
            fprintf(fp, "%f, ", potential);
        
        
        }
    
    fprintf(fp,"\n");
    
    }

    fclose(fp);

}
