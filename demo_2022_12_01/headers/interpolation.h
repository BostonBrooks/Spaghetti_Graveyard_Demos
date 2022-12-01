#define INTERPOLATION

#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif



float lancsoz2(float x);

float dlancsoz2(float x);

float not_dlancsoz2(float x);

int lanczos2_getElevation(bbMapCoords mc);

bbFloat3D lanczos2_getNormal(bbMapCoords mc);
