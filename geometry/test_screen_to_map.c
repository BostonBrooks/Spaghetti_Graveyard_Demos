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

    load_elevations();
    
    Screen_Coords screen_coords;
    Map_Coords map_coords;
   
    int i;
    
    for(i = 0; i < 20; i++){
      
        viewpoint.i =  rand() % 32000 - 1000;
        viewpoint.j =  rand() % 32000 - 1000;
        viewpoint.k = interpolate_elevation(viewpoint.i, viewpoint.j);
    
        screen_coords.x = rand() % 800;
        screen_coords.y = rand() % 600;
    
        print_screen_coords(screen_coords);
    
        map_coords = screen_to_map_centre_viewpoint(screen_coords);
    
        print_map_coords(map_coords);
    
        screen_coords = map_to_screen_centre_viewpoint(map_coords);
    
        print_screen_coords(screen_coords);
    
        map_coords = screen_to_map_centre_viewpoint(screen_coords);
    
        print_map_coords(map_coords);
        
        printf("\n");
    
    }
    
}
    
    
