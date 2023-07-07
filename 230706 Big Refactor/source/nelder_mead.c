#define NELDER_MEAD

//-----------------------------INCLUDES----------------------------//

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif


#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif


#ifndef POTENTIAL_FIELDS
#define POTENTIAL_FIELDS
#include "../headers/potential_fields.h"
#endif

#ifndef BBDRAWABLE
#define BBDRAWABLE
#include "../headers/bbDrawable.h"
#endif

//-----------------------------CODE----------------------------//

bbMapCoords nelder_mead (int drawable_int){

// Tune-able parameters (default values)

    float alpha = 1;   //scale of first reflection
    float beta = 0.5;  //scale of contracted reflection
    float gamma = 2;   //scale of expanded reflection
    float delta = 0.5; //scale of triangle shrinkage
    
    int initial_size = POINTS_PER_PIXEL; //TODO try increasing initial size
    
  //vertices
  
    bbMapCoords xh;       //worst vertex (high)
    bbMapCoords xs;       //second worst vertex
    bbMapCoords xl;       //best vertex (low)
    bbMapCoords centroid; //centre of line opposite worst vertex
    bbMapCoords xr;       //reflected vertex
    bbMapCoords xe;       //point extended from reflected vertex
    bbMapCoords xc;       //point contracted reflected vertex / worst vertex
    
    float fh;       //value at worst vertex
    float fs;       //value at second worst vertex
    float fl;       //value at best vertex
    float fr;       //value at reflected vertex
    float fe;       //value at point extended from reflected vertex
    float fc;       //value at point contracted reflected vertex / worst vertex 
    
//initialise 3 vertices
    bbDrawable* drawable = bbDrawable_Pool_Lookup(drawable_int);

    xh = xs = xl = drawable->location;
    
    xh.i += initial_size;
    xs.j += initial_size;
    
    fh = sum_potential_Nearby(drawable_int, xh);
    fs = sum_potential_Nearby(drawable_int, xs);
    fl = sum_potential_Nearby(drawable_int, xl);    
    
   // printf("fh = %f, fs = %f, fl = %f\n",fh, fs, fl); 
    
    
//identify worst vertex (sort 3 numbers)
int test_iteration;

//TODO try reducing the 32 value
    for(test_iteration = 0; test_iteration < 32; test_iteration ++){ //break;
        bbMapCoords temp_vertex;
        float temp_float;

  //to get in the order largest to smallest
  //if (x < y) swap(x, y);
    
        if(fh < fs){
            temp_float = fh;
            temp_vertex = xh;
        
            fh = fs;
            xh = xs;
        
            fs = temp_float;
            xs = temp_vertex;
        
    
        }
    
  //if (y < z) swap(y, z)
  
        if(fs < fl){
            temp_float = fs;
            temp_vertex = xs;
        
            fs = fl;
            xs = xl;
        
            fl = temp_float;
            xl = temp_vertex;
        
    
       }
  //if (x < y) swap(x, y);
  
        if(fh < fs){
            temp_float = fh;
            temp_vertex = xh;
        
            fh = fs;
            xh = xs;
        
            fs = temp_float;
            xs = temp_vertex;
        
    
        }
  

//reflect across centre of line formed by the other 2 vertices to create a new vertex *

  //Calculate Centroid of line opposite worst vertex (fs, fl)
        centroid.i = (xs.i + xl.i)/2; 
        centroid.j = (xs.j + xl.j)/2;
        
 //xr = c + alpha (c-xh);
        
        xr.i = centroid.i + alpha * (centroid.i - xh.i);
        xr.j = centroid.j + alpha * (centroid.j - xh.j);
        
        fr = sum_potential_Nearby(drawable_int, xr);

//if new vertex is better than worst vertex but not better than the three vertices:

        if (fr < fh && fr >= fl){
    
            fh = fr;
            xh = xr;
    
            continue; //restart loop
        }

  //replace old vertex with new vertex.
  
//if the new vertex is better than the three vertices,

        if (fr < fl) {
  //increase the "jump"
  
    
            xe.i = centroid.i + gamma * (centroid.i - xh.i);
            xe.j = centroid.j + gamma * (centroid.j - xh.j);
        
            fe = sum_potential_Nearby(drawable_int, xe);
  
  //If this improves the new vertex even mode, replace the worst vertex with the new point.
            if (fe < fr){
        
                fh = fe;
                xh = xe;
            
                continue;
            }
        
            fh = fr;
            xh = xr;
        
            continue;
        }
//If the new vertex (*) is worse than the original 3 vertices

//CONTRACT move worst vertex to make a smaller triangle:
    //if fs <= fr < fh,
        if (fs <= fr && fr < fh){
      //xc = c + beta (xr-c)
      
            xc.i = centroid.i + beta * (xr.i - centroid.i);
            xc.j = centroid.j + beta * (xr.i - centroid.j);
      
            fc = sum_potential_Nearby(drawable_int, xc);
      
            if(fc < fr){ //if fc < fr, replace fh with fc
      
                fh = fc;
                xh = xc;
      
                continue;
      
            }
      
        } else if (fr >= fh){
    //else if fr >= fh,
      //xc = c + beta (xh-c)
      
            xc.i = centroid.i + beta * (xh.i - centroid.i);
            xc.j = centroid.j + beta * (xh.i - centroid.j);
        
            fc = sum_potential_Nearby(drawable_int, xc);
        
      //if fc < fh, replace fh with fc
      
            if (fc < fh){
        
                fh = fc;
                xh = xc;
            
                continue;
            }
      
       }
  
  
  
//If none of the above new vertices improve the minimisation problem:

  //shrink the triangle toward the best vertex
  
    //xh = xl + delta (xh-xl)
    
    
        xh.i = xl.i + delta * (xh.i - xl.i);
        xh.j = xl.j + delta * (xh.i - xl.j);
    
        fh = sum_potential_Nearby(drawable_int, xh);
    
    //xs = xl + delta (xs-xl)
    
    
        xs.i = xl.i + delta * (xs.i - xl.i);
        xs.j = xl.j + delta * (xs.i - xl.j);
    
        fs = sum_potential_Nearby(drawable_int, xs);

        continue;





   }
        return(xl);

}
