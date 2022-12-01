#define INTERPOLATION

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


//-----------------------------headers----------------------------//

//this stuff belongs in some header file


float* lanczos_kernel;
float* gaussdiff_kernel;

//-----------------------------CODE----------------------------//
float lanczos2(float x){

    if (x < 0.01 && x>-0.01){
	
         return (
              1.0
              - 5.0 * M_PI * M_PI * x * x / 24.0
              + 91 * M_PI * M_PI * M_PI * M_PI * x * x * x * x / 5760.0
	 );
	}

	if (x>= 2 || x <= -2) return 0;

	return 2*sin(x * M_PI) * sin( x * M_PI/2.0) / (M_PI * M_PI * x * x);

}

float dlanczos2(float x){

    if (x < 0.01 && x>-0.01){
	
	 return (
            - 5.0 * M_PI * M_PI * x / 12.0
            + 91.0 * M_PI * M_PI * M_PI * M_PI * x * x * x /1440.0
            - 41.0 * M_PI * M_PI * M_PI * M_PI * M_PI * M_PI * x * x * x * x * x / 10752.0
        );
    }

    if (x>= 2 || x <= -2) return 0;
    
    return sin(M_PI * x / 2.0) * ( M_PI * x + 3.0 * M_PI * x * cos( M_PI * x) - 4 * sin( M_PI * x)) / (M_PI * M_PI * x * x * x);

}

int generate_lanczos(){

    lanczos_kernel = malloc(4* (POINTS_PER_PIXEL * PIXELS_PER_TILE)*sizeof(float));
    int n, X;
    float x;
    for (n = 0; n< 4* (POINTS_PER_PIXEL * PIXELS_PER_TILE); n++){
        X = n - 2* (POINTS_PER_PIXEL * PIXELS_PER_TILE);
        x = X/(1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));
        lanczos_kernel[n] =  lanczos2(x);
    
    }
    
    
}



float discrete_lanczos(int X){
    if (lanczos_kernel ==  NULL) generate_lanczos();

    if (X>= 2* (POINTS_PER_PIXEL * PIXELS_PER_TILE) || X <= -2* (POINTS_PER_PIXEL * PIXELS_PER_TILE)) return 0;
    int n = X + 2* (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    return lanczos_kernel[n];
    
}
    
float gaussdiff(float x){
	
    return exp(-x*x)*x;

}
//float* gaussdiff_kernel;

int generate_gaussdiff(){

    gaussdiff_kernel = malloc(10* (POINTS_PER_PIXEL * PIXELS_PER_TILE)*sizeof(float));
    int n, X;
    float x;
    for (n = 0; n< 10* (POINTS_PER_PIXEL * PIXELS_PER_TILE); n++){
        X = n - 5* (POINTS_PER_PIXEL * PIXELS_PER_TILE);
        x = X/(1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));
        gaussdiff_kernel[n] =  gaussdiff(x);
    
    }
    
    
}



float discrete_gaussdiff(int X){
    if (gaussdiff_kernel ==  NULL) generate_gaussdiff();

    if (X>= 5* (POINTS_PER_PIXEL * PIXELS_PER_TILE) || X <= -5* (POINTS_PER_PIXEL * PIXELS_PER_TILE)) return 0;
    int n = X + 5* (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    return gaussdiff_kernel[n];
    
}


int lanczos2_getElevation(bbMapCoords mc){

    bbTileCoords tc = bbMapCoords_getTileCoords (mc);

    
    
    int residual_i, residual_j;
    float normalised_residual_i, normalised_residual_j;
    
    residual_i = mc.i % (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    residual_j = mc.j % (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    
    normalised_residual_i = 
         (residual_i * 1.0) / (1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));
         
    normalised_residual_j = 
         (residual_j * 1.0) / (1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));

    float S = 0; float s;
    int i, j;
    
    for(i=-5; i < 5; i++){
    	for (j=-5; j < 5; j++){
    		
            s = bbTileCoords_getElevation(tc.i + i, tc.j + j);
    	    S+= s*lanczos2(i - normalised_residual_i) *lanczos2( j - normalised_residual_j);
    	}
    
    
    }

    return S;

};

bbFloat3D lanczos2_getNormal(bbMapCoords mc){

    bbTileCoords tc = bbMapCoords_getTileCoords (mc);

    
    
    int residual_i, residual_j;
    float normalised_residual_i, normalised_residual_j;
    
    residual_i = mc.i % (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    residual_j = mc.j % (POINTS_PER_PIXEL * PIXELS_PER_TILE);
    
    normalised_residual_i = 
         (residual_i * 1.0) / (1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));
         
    normalised_residual_j = 
         (residual_j * 1.0) / (1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));

    float partialx = 0;
    float partialy = 0;
    float s;
    int i, j;
    
    for(i=-5; i < 5; i++){
    	for (j=-5; j < 5; j++){
    	
            int X = i*(POINTS_PER_PIXEL * PIXELS_PER_TILE)-residual_i;
            int Y = j*(POINTS_PER_PIXEL * PIXELS_PER_TILE)-residual_j;
            s = bbTileCoords_getElevation(tc.i + i, tc.j + j);
    	    partialx+= s*discrete_gaussdiff( X)*discrete_lanczos(Y);
    	    partialy+= s*discrete_lanczos(X) *discrete_gaussdiff(Y);
    	}
    
    
    }
    partialx /=(1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));
    partialy /=(1.0 * (POINTS_PER_PIXEL * PIXELS_PER_TILE));
    
    bbFloat3D surface_normal;
         
    surface_normal.i = -partialx; //-df/di
    surface_normal.j = -partialy; //-df/dj
    surface_normal.k = 1;

         
         return (bbFloat3D_normalise(surface_normal));

};


