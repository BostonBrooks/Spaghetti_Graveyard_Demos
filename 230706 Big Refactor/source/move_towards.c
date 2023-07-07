#define MOVE_TOWARDS
//-----------------------------INCLUDES----------------------------//


#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif


int bbMapCoords_move_toward(bbMapCoords* subject, bbMapCoords object, float distance){

    float delta_i;
    float delta_j;
    
    delta_i = object.i - subject->i;
    delta_j = object.j - subject->j;
    
    float delta = sqrt(delta_i * delta_i + delta_j*delta_j);
    
    if (delta<distance) {
        subject->i = object.i;
        subject->j = object.j;
        return(0);
    }
    
    delta_i *= distance / delta;
    delta_j *= distance / delta;
    
    subject->i += delta_i;
    subject->j += delta_j;
    
    return(0);
}

