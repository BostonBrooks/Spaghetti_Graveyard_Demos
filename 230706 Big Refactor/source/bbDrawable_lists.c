#define BBDRAWABLE_LISTS
//-----------------------------INCLUDES----------------------------//

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

#ifndef MEDIA
#define MEDIA
#include "../headers/media.h"
#endif


#ifndef BBDRAWABLE_PLOT
#define BBDRAWABLE_PLOT
#include "../headers/bbDrawable_plot.h"
#endif

#ifndef BBTERRAINSQUARE
#define BBTERRAINSQUARE
#include "../headers/bbTerrainSquare.h"
#endif


//-----------------------------CODE------------------------------//





//private, use bbDrawable_setLocation()
int bbDrawable_removefromTS (int drawable_int){
    bbDrawable* drawable = bbDrawable_Pool_Lookup(drawable_int);
    
    if (drawable->SquareCoords.i == -1){
        
        assert(drawable->SquareCoords.i == -1);
        
        //remove from list denoted by: 
            //extern int bbDrawable_lost_head;
            //extern int bbDrawable_lost_tail;
        int prev_int = drawable->Square_Prev;
        int next_int = drawable->Square_Next;
        
        if (prev_int == -1 && next_int == -1){
        
            //empty list
            bbDrawable_lost_head = -1;
            bbDrawable_lost_tail = -1;
            
        } else if (prev_int == -1){
            //remove from start of list
            bbDrawable* next_drawable = bbDrawable_Pool_Lookup(next_int);
            
            next_drawable->Square_Prev = -1;
            bbDrawable_lost_head = next_int;
            
        
        
        } else if (next_int == -1){
            //remove from end of list
            bbDrawable* prev_drawable = bbDrawable_Pool_Lookup(prev_int);
            
            prev_drawable->Square_Next = -1;
            bbDrawable_lost_tail = prev_int;
        
        
        
        } else {
            //remove from middle of list
            bbDrawable* prev_drawable = bbDrawable_Pool_Lookup(prev_int);
            bbDrawable* next_drawable = bbDrawable_Pool_Lookup(next_int);
            
            next_drawable->Square_Prev = prev_int; 
            prev_drawable->Square_Next = next_int; 
        
        
        }
    
    
    } else {
    
       // lookup terrain square
       bbTerrainSquare* TS = &bbTerrainSquare_grid[drawable->SquareCoords.i][drawable->SquareCoords.j];
       
               //remove from list denoted by: 
            //TS->bbDrawable_head;
            //TS->bbDrawable_tail;
        int prev_int = drawable->Square_Prev;
        int next_int = drawable->Square_Next;
        
        if (prev_int == -1 && next_int == -1){
        
            //empty list
            TS->bbDrawable_head = -1;
            TS->bbDrawable_tail = -1;
            
        } else if (prev_int == -1){
            //remove from start of list
            bbDrawable* next_drawable = bbDrawable_Pool_Lookup(next_int);
            
            next_drawable->Square_Prev = -1;
            TS->bbDrawable_head = next_int;
            
        
        
        } else if (next_int == -1){
            //remove from end of list
            bbDrawable* prev_drawable = bbDrawable_Pool_Lookup(prev_int);
            
            prev_drawable->Square_Next = -1;
            TS->bbDrawable_tail = prev_int;
        
        
        
        } else {
            //remove from middle of list
            bbDrawable* prev_drawable = bbDrawable_Pool_Lookup(prev_int);
            bbDrawable* next_drawable = bbDrawable_Pool_Lookup(next_int);
            
            next_drawable->Square_Prev = prev_int; 
            prev_drawable->Square_Next = next_int; 
        
        
        }
    
    }
    return 0;
}

//private, use bbDrawable_setLocation()
int bbDrawable_addtoTS (int drawable_int){

    //copy and paste from Balloon_new(bbMapCoords mc);
    bbDrawable* drawable = bbDrawable_Pool_Lookup(drawable_int);
    bbMapCoords mc = drawable->location;
    bbSquareCoords SC = bbMapCoords_getSquareCoords(mc);
    
    if (SC.i >= SQUARES_PER_MAP || SC.j >= SQUARES_PER_MAP || SC.i < 0 || SC.j < 0){
        printf("drawable out of bounds\n");
        
        
    
        drawable->SquareCoords.i = -1;
        drawable->SquareCoords.j = -1;
        
        if (bbDrawable_lost_head == -1){
        
            assert(bbDrawable_lost_tail == -1);
            
            bbDrawable_lost_head = drawable_int;
            bbDrawable_lost_tail = drawable_int;
            
            drawable->Square_Prev = -1;
            drawable->Square_Next = -1;
            
        
        } else {
        
            bbDrawable* tail = bbDrawable_Pool_Lookup(bbDrawable_lost_tail);
            
            drawable->Square_Prev = bbDrawable_lost_tail;
            drawable->Square_Next = -1;
            
            tail->Square_Next = drawable_int;
            bbDrawable_lost_tail = drawable_int;
            
            
            
        
        }
            return(0);
    
    }
    
    bbTerrainSquare* TS = &bbTerrainSquare_grid[SC.i][SC.j];
    
    drawable->SquareCoords.i = SC.i;
    drawable->SquareCoords.j = SC.j;
    
    int tail_int = TS->bbDrawable_tail;
    int head_int = TS->bbDrawable_head;
    
    if (tail_int == -1){
    
        assert(head_int == -1);
    
        drawable->Square_Prev = -1;
        drawable->Square_Next = -1;
    
        TS->bbDrawable_head = drawable_int;
        TS->bbDrawable_tail = drawable_int;
        return drawable_int;
    
    } else {
    
        bbDrawable* list_element = bbDrawable_Pool_Lookup(head_int);
        
        //if drawable is closer than list_element,
           //select next element in list
           //if list_element is now -1, insert drawable after last element and end
           //repeat
        //else insert drawable before list_element
         
        while(bbDrawable_isCloser(drawable, list_element)){
           int list_element_int = list_element->Square_Next;
           if (list_element_int == -1){
           //insert drawable after last element and end
              list_element->Square_Next = drawable_int;
              drawable->Square_Prev = list_element->Pool_Self;
              drawable->Square_Next = -1;
              TS->bbDrawable_tail = drawable_int;
              
              return(drawable_int);
           
           }
           
           list_element = bbDrawable_Pool_Lookup(list_element_int);
        
        
        }
        //else insert drawable before list_element
        
        bbDrawable* next_element = list_element;
        
             
//printf(  "list_element->Square_Prev = %d\n",   list_element->Square_Prev);
//assert(list_element->Square_Prev >= 0);//searching for bug...
        if(list_element->Square_Prev == -1){
        //bug found, handle exception
        
                
            //lets try inserting balloon before list element?
            
            TS->bbDrawable_head = drawable_int;
            drawable->Square_Prev = -1;
            drawable->Square_Next = list_element->Pool_Self;
            list_element->Square_Prev = drawable_int;
            
            
            
                
            return drawable_int;
                
            
                
        }
        
        bbDrawable* prev_element = bbDrawable_Pool_Lookup(list_element->Square_Prev);
        
        next_element->Square_Prev = drawable_int;
        prev_element->Square_Next = drawable_int;
        
        drawable->Square_Prev = prev_element->Pool_Self;
        drawable->Square_Next = next_element->Pool_Self;
        
        return drawable_int;
        
    
    }
    
    printf("Dead code?\n");
    return drawable_int;
     //TODO does this indincate failure?
    
    
}



int bbDrawable_setLocation(int drawable_int, bbMapCoords mc){

    bbDrawable* drawable = bbDrawable_Pool_Lookup(drawable_int);
    
    bbSquareCoords SC = bbMapCoords_getSquareCoords(mc);
    
    //if moving from bbDrawable_lost to bbDrawable_lost do nothing
    if (SC.i >= SQUARES_PER_MAP || SC.j >= SQUARES_PER_MAP || SC.i < 0 || SC.j < 0){
    
        if (drawable->SquareCoords.i == -1){
        
            assert(drawable->SquareCoords.j == -1);
            return 0;
            
        }
    }
    
    bbTerrainSquare* TS = &bbTerrainSquare_grid[SC.i][SC.j];
    
    
    if (SC.i == drawable->SquareCoords.i && SC.j == drawable->SquareCoords.j){
    
        //TODO move up or down the list of drawables in the given terrain square 
        bbDrawable_removefromTS (drawable_int);
        drawable->location = mc;
        bbDrawable_addtoTS(drawable_int);
        
    } else {
    
        //remove from current TS, update location and add to new TS
        bbDrawable_removefromTS (drawable_int);
        drawable->location = mc;
        bbDrawable_addtoTS(drawable_int);
        
    }
    
    return 0;
}




