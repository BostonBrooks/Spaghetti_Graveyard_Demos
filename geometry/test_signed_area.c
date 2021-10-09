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

    Screen_Coords p1, p2, p3;
    float area;

    p1.x = 0;
    p1.y = 0;

    p2.x = 1;
    p2.y = 0;

    p3.x = 0;
    p3.y = 1;

    area = signed_area(p1, p2, p3);

    printf("(%f, %f), (%f, %f), (%f, %f) ... area = %f\n", p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, area );

    p1.x = 0;
    p1.y = 0;

    p2.x = 0;
    p2.y = 3;

    p3.x = 3;
    p3.y = 0;

    area = signed_area(p1, p2, p3);

    printf("(%f, %f), (%f, %f), (%f, %f) ... area = %f\n", p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, area );
}
