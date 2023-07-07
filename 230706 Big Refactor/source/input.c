/**
 * @file
 *
 * @ingroup 
 * @brief Input key presses and mouse clicks
 */

#define INPUT


//-----------------------------INCLUDES----------------------------//


#ifndef PROMPT
#define PROMPT
#include "../headers/prompt.h"
#endif

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif


#ifndef GEOMETRY
#define GEOMETRY
#include "../headers/geometry.h"
#endif


//-----------------------------GLOBALS----------------------------//


extern sfRenderWindow* window; //usually found in window.h


//-----------------------------FUNCTIONS--------------------------//

/** Get raw input */
int input_process(void){

    bbScreenCoords point;

    sfEvent event;
    while(sfRenderWindow_pollEvent(window, &event)){ //this block could be moved to input.h
        
        if (event.type == sfEvtClosed) exit(1);
 
// get mouse click/movement event?
// is the player clicking on a widget or the viewport or whatever?       
 /*       if (event.type == sfEvtMouseMoved){
            point.x = event.mouseMove.x;
                point.y = event.mouseMove.y;
                
                point.x = point.x -  VIEWPORT_LEFT;
                point.y = point.y -  VIEWPORT_TOP;
        } else if (event.type == sfEvtMouseButtonPressed){
            
            point.x = event.mouseButton.x -  VIEWPORT_LEFT;
            point.y = event.mouseButton.y -  VIEWPORT_TOP;
            mc = bbScreenCoords_getMapCoords (point);
            goalpoint = mc;
                
//get key presses and pass to command prombt                
        } else */if (event.type == sfEvtKeyPressed){
            
             switch(event.key.code){
            
                case sfKeyA:
                     prompt_pass_char('a');
                        break;
                case sfKeyB:
                    prompt_pass_char('b');
                    break;
                case sfKeyC:
                    prompt_pass_char('c');
                    break;
                case sfKeyD:
                    prompt_pass_char('d');
                    break;
                case sfKeyE:
                    prompt_pass_char('e');
                    break;
                case sfKeyF:
                    prompt_pass_char('f');
                    break;
                case sfKeyG:
                    prompt_pass_char('g');
                    break;
                case sfKeyH:
                    prompt_pass_char('h');
                    break;
                case sfKeyI:
                    prompt_pass_char('i');
                    break;
                case sfKeyJ:
                    prompt_pass_char('j');
                    break;
                case sfKeyK:
                    prompt_pass_char('k');
                    break;
                case sfKeyL:
                    prompt_pass_char('l');
                    break;
                case sfKeyM:
                    prompt_pass_char('m');
                    break;
                case sfKeyN:
                    prompt_pass_char('n');
                    break;
                case sfKeyO:
                    prompt_pass_char('o');
                    break;
                case sfKeyP:
                    prompt_pass_char('p');
                    break;
                case sfKeyQ:
                    prompt_pass_char('q');
                    break;
                case sfKeyR:
                    prompt_pass_char('r');
                    break;
                case sfKeyS:
                    prompt_pass_char('s');
                    break;
                case sfKeyT:
                    prompt_pass_char('t');
                    break;
                case sfKeyU:
                    prompt_pass_char('u');
                    break;
                case sfKeyV:
                    prompt_pass_char('v');
                    break;
                case sfKeyW:
                    prompt_pass_char('w');
                    break;
                case sfKeyX:
                    prompt_pass_char('x');
                    break;
                case sfKeyY:
                    prompt_pass_char('y');
                    break;
                case sfKeyZ:
                    prompt_pass_char('z');
                    break;
                case sfKeyReturn:
                    prompt_pass_char('\n');
                    break;
                case sfKeyEscape:
                    exit(EXIT_SUCCESS);
                    break;
                case sfKeyTab:
                    //plot_potential(Fox_drawable_int);
                    break;
                case sfKeyTilde:
                    exit(11);
                case sfKeyDivide:
                    prompt_pass_char('/'); //let prompt handle escape char
                    break;
                default:
                    break;
            
            }
        
        }
        
    }
    
// Get mouse state
/*    if (sfMouse_isButtonPressed(sfMouseLeft)){
        
        sfVector2i position = sfMouse_getPosition(window);
        viewpoint = mc;
        goalpoint = mc;
    } else if (sfMouse_isButtonPressed(sfMouseRight)){
        
        sfVector2i position = sfMouse_getPosition(window);
        point.x = position.x - VIEWPORT_LEFT;
        point.y = position.y - VIEWPORT_TOP;
        mc = bbScreenCoords_getMapCoords (point);
        goalpoint = mc;
    }
    
*/    
    return 0;
}
