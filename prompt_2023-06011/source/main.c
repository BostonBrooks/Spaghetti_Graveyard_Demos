#define MAIN

//-----------------------------INCLUDES----------------------------//

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

#ifndef PROMPT
#define PROMPT
#include "../headers/prompt.h"
#endif


#ifndef INPUT
#define INPUT
#include "../headers/input.h"
#endif




//-----------------------------CONSTANTS----------------------------//

//-----------------------------STRUCURES------------------------------//

//-----------------------------GLOBALS----------------------------//
sfRenderWindow* window;

//-----------------------------FUNCTIONS--------------------------//



int main (void){

//create window
    sfVideoMode mode = {WINDOW_WIDTH, WINDOW_HEIGHT, 32};
    window = sfRenderWindow_create(mode, "Prompt", sfResize | sfClose, NULL);
    
    sfColor beige_background;
    beige_background.r = 246;
    beige_background.g = 224;
    beige_background.b = 201;
    beige_background.a = 255;
    
    sfRenderWindow_clear(window, beige_background);

//initialise prompt
    prompt_init();
    

    while(1){

        //pass input to prompt
    
    
        //clear screen
        sfRenderWindow_clear(window, beige_background);
        //display prompt
        prompt_display();
        sfRenderWindow_display(window);
        
        
        input_process(); //hand the job of processing input to separate file
        

        
        

    }

//destroy prompt?

//close window
sfRenderWindow_destroy(window);

//exit




}
