
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>


#define DB(X) printf(" %d", X); fflush(stdout);

#ifndef CIRCLES
#include "circles.h"
#endif

#ifndef COMMANDS
#include "commands.h"
#endif

int Handle_Input(void);
int Command_List_All(void);
int Circle_List_All(void);

int main(void){

    Init_Circles();
    Init_Commands();

    while(1){
        Handle_Input();


        Draw_Circles();

    }
}

sfEvent event;

int Handle_Input(void){

    sfVector2i position;
    while(1){
        sfRenderWindow_waitEvent(window, &event);
        
        if (event.type == sfEvtMouseMoved){
            if (!(sfMouse_isButtonPressed (sfMouseLeft))) continue;
            if (event.mouseMove.x < 0) continue;
            if (event.mouseMove.x > 800) continue;
            if (event.mouseMove.y < 0) continue;
            if (event.mouseMove.y > 600) continue;
        
            Create_Circle_Event(event.mouseMove.x, event.mouseMove.y);
            return 0;

        }

        if (event.type == sfEvtMouseButtonPressed){
            if (event.mouseButton.button == sfMouseLeft){
                if (event.mouseButton.x < 0) continue;
                if (event.mouseButton.x > 800) continue;
                if (event.mouseButton.y < 0) continue;
                if (event.mouseButton.y > 600) continue;
        
                Create_Circle_Event(event.mouseButton.x, event.mouseButton.y);
                return 0;

            } else if (event.mouseButton.button == sfMouseRight){

                if (event.mouseButton.x < 0) continue;
                if (event.mouseButton.x > 800) continue;
                if (event.mouseButton.y < 0) continue;
                if (event.mouseButton.y > 600) continue;

        
                int Circle_Int = Find_Nearest_Circle(
                    event.mouseButton.x,
                    event.mouseButton.y
                );

                if (Circle_Int >= 0) {
                    Delete_Circle_Event(Circle_Int);
                    return 0;
                }
            continue;
            }

        continue;

        }

        if (event.type == sfEvtKeyPressed){
            if (event.key.code == sfKeyLeft){

                Command_Undo();

                return 0;
            } else if (event.key.code == sfKeyRight){
                Command_Redo();
                return 0;
            } else {
                continue;
            }

        }

    }
    
   
}



    int Command_List_All(void){
        int j;
            for (j = 0; j < 4; j++){
                Command* Object = Commands_sudo(j);
                printf("command, pool prev %d, self %d, next,  %d, in use %d, List prev %d, next %d\n", Object->Pool_Prev, Object->Pool_Self, Object->Pool_Next, Object->Pool_In_Use, Object->List_Prev, Object->List_Next); fflush(stdout);
        }
    }

    int Circle_List_All(void){
        int j;
            for (j = 0; j < 16; j++){
                Circle* Object = Circles_sudo(j);
                printf("Circle, pool prev %d, self %d, next,  %d, in use %d, List prev %d, next %d\n", Object->Pool_Prev, Object->Pool_Self, Object->Pool_Next, Object->Pool_In_Use, Object->List_Prev, Object->List_Next); fflush(stdout);
        }
    }
