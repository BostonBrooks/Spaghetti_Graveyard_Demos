/**
 * @file
 *
 * @ingroup 
 * @brief Uses Gaussian derivative and Lanczos interpolation to calculate a smooth ground surface, in order to calculate hill shading
 */

//-----------------------------INCLUDES----------------------------//
#define INTERPOLATION


#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif


//-----------------------------GLOBALS----------------------------//

//this stuff belongs in some header file

/** Discrete representation of Lanczos kernel */
extern float* lanczos_kernel;

/** Discrete representation of Gaussian derivative kernel */
extern float* gaussdiff_kernel;

//-----------------------------CODE----------------------------//

/** Algebraic equation of Lanczos kernel */
float lanczos(float x);

/** Algebraic equation of Lanczos derivative kernel, use Gaussian derivative */
float dlanczos(float x);

/** Calculate discrete representation of Lanczos kernel */
int generate_lanczos(void);


/** Return discrete representation of Lanczos kernel */
float discrete_lanczos(int X);
    
/** Algebraic equation of Gaussian derivative kernel, use Gaussian derivative */
float gaussdiff(float x);
//float* gaussdiff_kernel;

/** Calculate discrete representation of Gaussian derivative kernel */
int generate_gaussdiff(void);


/** Return discrete representation of Gaussian derivative kernel */
float discrete_gaussdiff(int X);

/** Use Lanczos kernel to calculate elevation at a point */
int lanczos_getElevation(bbMapCoords mc);

/** use interpolation to calculate normal to ground surface at a point */
bbFloat3D lanczos_getNormal(bbMapCoords mc);


