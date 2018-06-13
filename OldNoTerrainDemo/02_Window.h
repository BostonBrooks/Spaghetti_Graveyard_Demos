    // ---- Data Types ---- //


    // ---- Globals ---- //

#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 800

sfVector2f LeftScale = {-1.f, 1.f};
sfVector2f RightScale = {1.f, 1.f};

#define XSCALE 1
#define YSCALE 2

int VIEW_RADIUS  = 500;

sfVideoMode mode = {SCREEN_WIDTH + 440, SCREEN_HEIGHT, 32};

sfRenderWindow* window;

IJ_Point ViewPoint;


    // ---- Init Function ---- //

int Init_Window (void) {
 /* Create the main window */
    window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);

    sfVector2i ScreenPosition;
    ScreenPosition.x = 0;
    ScreenPosition.y = 0;

    sfRenderWindow_setPosition(window, ScreenPosition);
}
    // ---- Other Functions ---- //

XY_Point IJtoXY ( IJ_Point a ) {
    XY_Point b;
    b.x = SCREEN_WIDTH/2 + ((a.i - ViewPoint.i) - (a.j - ViewPoint.j))/XSCALE;
    
    b.y = SCREEN_HEIGHT/2 + ((a.i - ViewPoint.i) + (a.j - ViewPoint.j))/YSCALE;
    
    return b;
}

IJ_Point XYtoIJ ( XY_Point b ) {
    IJ_Point a;
    
    a.i = ((b.x - SCREEN_WIDTH/2)*XSCALE + (b.y - SCREEN_HEIGHT/2)*YSCALE)/2 + ViewPoint.i;
    
    a.j = ((b.y - SCREEN_HEIGHT/2)*YSCALE - (b.x - SCREEN_WIDTH/2)*XSCALE)/2 + ViewPoint.j;
    
    return a;
}


    // ---- Release Function ---- //
