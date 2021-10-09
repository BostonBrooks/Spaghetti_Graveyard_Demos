#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

#include "geometry.h"

int main(void){

    float Mat_A [4][4] = {
        {1, 2, 3, 4},
        {2, 1, 4, 3},
        {4, 5, 7, 6},
        {3, 4, 6, 7}
    };

    float Mat_B [4][4] = {
        {3, 5, 2, 1},
        {3, 2, 6, 7},
        {3, 2, 2, 6},
        {3, 4, 6, 7}
    };

    float Mat_C [4][4];

    
    
    print_matrix(Mat_A);
    print_matrix(Mat_B);

    matrix_multiply(Mat_A, Mat_B, Mat_C);   
    
    print_matrix(Mat_C); 
    
}
/*
    30    31    44    61
    30    32    36    54
    66    68    88   123
    60    63    84   116
*/
