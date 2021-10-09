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

    viewpoint.i = 123;
    viewpoint.j = 543;
    viewpoint.k = 234;
    
    Map_Coords mc;
    Screen_Coords sc;
    sc.x = 567;
    sc.y = 765;
    
    print_screen_coords(sc);
    

    mc = screen_to_map_k_0(sc);
    print_map_coords(mc);
    
    sc = map_to_screen_centre_viewpoint(mc);
    print_screen_coords(sc);
    
    mc = screen_to_map_k_0(sc);
    print_map_coords(mc);
    
    printf("\n");
    
    mc.i = 720;
    mc.j = 512;
    mc.k = ELEVATION_MAX;
    
    print_map_coords(mc);
    
    sc = map_to_screen_centre_viewpoint(mc);
    print_screen_coords(sc);
    
    mc = screen_to_map_k_max(sc);
    print_map_coords(mc);
    
    sc = map_to_screen_centre_viewpoint(mc);
    print_screen_coords(sc);
    
    return 0;
}
