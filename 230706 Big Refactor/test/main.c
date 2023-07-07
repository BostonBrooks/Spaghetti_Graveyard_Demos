#define MAIN

#ifndef ALL_INCLUDES
#define ALL_INCLUDES
#include "../headers/all_includes.h"
#endif


int Current_Time = 0;

int Test_All(void);

int main (void){

    printf("Program Starting:\n");
    
    Test_All();
    
    sleep(1);
    
    exit(0);


}

int Test_All(void){

    Window_Init();

    sfColor beige = COLOUR_BEIGE;
    sfColor dark_brown = COLOUR_DARKBROWN;
    sfVector2f position = {50, 440};
    
    sfFont* font = sfFont_createFromFile("./media/Bowman.ttf");
    if (!font) return EXIT_FAILURE;
    sfText* text = sfText_create();
    sfText_setString(text, "Hello Spaghetti Graveyard!");
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 128);
    sfText_setColor(text, dark_brown);
    sfText_setPosition(text, position);
    
    sfRenderWindow_drawText(window, text, NULL);
    sfRenderWindow_display(window);
    
    //Setting the angle of the sun.
    LightIncedence.i = 1;
    LightIncedence.j = 1;
    LightIncedence.k = 1;
    
    //Setting the position of the viewer.
    viewpoint.i = 2000;
    viewpoint.j = 3000;
    viewpoint.k = 0;

    //setting the direction the viewer is travelling
    bbMapCoords goalpoint;
    goalpoint.i = 2000;
    goalpoint.j = 3000;
    goalpoint.k = 0;
    
    
    
    #ifdef VERBOSE
        printf("bbDrawable_Pool_Make_Null();\n");
    #endif
        
    bbDrawable_Pool_Make_Null();
    
    #ifdef VERBOSE
        printf("bbAIControl_Pool_Make_Null();\n");
    #endif
    
    bbAIControl_Pool_Make_Null();
    
    //#ifdef DEBUG
    //int drawable_int = bbDrawable_new(viewpoint);
    //printf("drawable_int = %d\n", drawable_int);
    
    //drawable_int = bbDrawable_new(viewpoint);
    //printf("drawable_int = %d\n", drawable_int);
    
    //drawable_int = bbDrawable_new(viewpoint);
    //printf("drawable_int = %d\n", drawable_int);
    
    //exit(1);
    //#endif
    
    
    #ifdef VERBOSE
        printf("Load_Media();\n");
    #endif
    
    Load_Media();
    
    #ifdef VERBOSE
        printf("bbTerrainSquare_initAll();\n");
    #endif
    
    bbTerrainSquare_initAll();
    
    #ifdef VERBOSE
        printf("bbElevations_load();\n");
    #endif
    
    bbElevations_load();
    
    #ifdef VERBOSE
        printf("bbViewport_init();\n");
    #endif
    
    
    bbViewport_init();
    
    #ifdef VERBOSE
        printf("bbGroundSurface_initAll();\n");
    #endif
    
    
    bbGroundSurface_initAll();
    
    #ifdef VERBOSE
        printf("bbDrawfunction_initAll();\n");
    #endif
    
    
    bbDrawfunction_initAll();
    
    #ifdef VERBOSE
        printf("bbAIControl_initTypes();\n");
    #endif
    
    
    bbAIControl_init();
    

    
    
    #ifdef VERBOSE
        printf("bbMessage_initTypes();\n");
    #endif
        
    bbMessage_initTypes();
    
    #ifdef VERBOSE
        printf("potential_avoidance_initShapes();\n");
    #endif
    
    potential_avoidance_initShapes();
    
    #ifdef VERBOSE
        printf("Create_Ground_Shaders();\n");
    #endif
    
    Create_Ground_Shaders();
    
    #ifdef VERBOSE
        printf("prompt_init();\n");
    #endif
    
    prompt_init();
    
    #ifdef VERBOSE
        printf("some text stuff\n");
    #endif
    
    char string2[1000];
    sfText* text2 = sfText_create();
    sfText_setString(text2, "Hello SFML");
    sfText_setFont(text2, font);
    sfText_setCharacterSize(text2, 50);
    sfVector2f text_position;
    text_position.x = 0;
    text_position.y = 150;
    sfText_setPosition(text2, text_position);
    
    #ifdef VERBOSE
        printf("Spawn some null AIs\n");
    #endif
    
    

    
    
    bbMapCoords mc;   
    
    for (int i=0; i<64; i++){
    
        float i = rand() % POINTS_PER_SQUARE;
        float j = (1.0 * rand())/RAND_MAX * (POINTS_PER_SQUARE);
    
        mc.i = i;
        mc.j = j;
        mc.k = 0;
        
        bbMapCoords_updateElevation(&mc);
    
        bbAI_constructor_vtable[AI_NULL](mc);
        
        //#ifdef DEBUG
        //printf("AI pool head = %d, AI pool tail = %d\n", bbAIControl_Pool_In_Use_Head, bbAIControl_Pool_In_Use_Tail);
        //#endif
           
        //#ifdef DEBUG
        //printf("AI available head = %d, AI available tail = %d\n", bbAIControl_Pool_Available_Head, bbAIControl_Pool_Available_Tail);
        //#endif   
    }


    sfRenderTexture_drawSprite(viewport, sfSprite_vtable[28], NULL); //viewport background
    

    
    while(1) {
    
        input_process();
        #ifdef VERBOSE
        printf("Beginning Main Loop: time = %d\n", Current_Time);
        #endif   
        sfEvent event;
        
        
        #ifdef VERBOSE
        printf("User Input:\n");
        #endif 
        
        bbScreenCoords point;
        while(sfRenderWindow_pollEvent(window, &event)){
            
        
        
            if (event.type == sfEvtMouseMoved){
                point.x = event.mouseMove.x;
                point.y = event.mouseMove.y;
                
                point.x = point.x -  VIEWPORT_LEFT;
                point.y = point.y -  VIEWPORT_TOP;
            } else if (event.type == sfEvtMouseButtonPressed){
            
                point.x = event.mouseButton.x -  VIEWPORT_LEFT;
                point.y = event.mouseButton.y -  VIEWPORT_TOP;
                mc = bbScreenCoords_getMapCoords (point);
                goalpoint = mc;
                
                
            } else if (event.type == sfEvtKeyPressed){
            
                if (event.key.code == sfKeyQ) exit(EXIT_SUCCESS);
                //if (event.key.code == sfKeyP) plot_potential(Fox_drawable_int);
                if (event.key.code == sfKeyR) exit(11);
            }
        
        }
        
        if (sfMouse_isButtonPressed(sfMouseRight)){
        
            sfVector2i position = sfMouse_getPosition(window);
            point.x = position.x - VIEWPORT_LEFT;
            point.y = position.y - VIEWPORT_TOP;
            mc = bbScreenCoords_getMapCoords (point);
            goalpoint = mc;
        }
        
        
        
        bbMapCoords_move_toward(&viewpoint, goalpoint, 8);

        if (sfMouse_isButtonPressed(sfMouseLeft)){
        
            sfVector2i position = sfMouse_getPosition(window);
            viewpoint = mc;
            goalpoint = mc;
        }
        
        #ifdef VERBOSE
        printf("bbAIControl_updatePool();\n");
        #endif 
        
        bbAIControl_updatePool();
                
        #ifdef VERBOSE
        printf("bbMessage_processAll();\n");
        #endif 
        
        bbMessage_processAll();
        
        #ifdef VERBOSE
        printf("Draw to Screen\n");
        #endif 
    
        bbTerrainSquare_drawVisible();
        bbGroundSurface_drawVisible(); 
        sfRenderTexture_display(viewport); 
        sfRenderWindow_drawSprite(window, viewport_sprite, &bbViewport_renderer);
        Display_Decal();
        prompt_display();
        sfRenderWindow_display(window);
        
        #ifdef VERBOSE
        printf("Clear Screen:\n");
        #endif 
        sfRenderTexture_clear(bbViewport_background, sfBlue);
        sfRenderTexture_clear(bbViewport_ground, sfTransparent);
        sfRenderTexture_clear(bbViewport_main, sfTransparent);
        sfRenderTexture_clear(bbViewport_highlights, sfTransparent);
        sfRenderTexture_clear(bbViewport_healthbars, sfTransparent);
        
        Current_Time++;
    }
    
    
    
    
    
    
}
