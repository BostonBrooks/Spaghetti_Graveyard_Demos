
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>



#include "pools.h"
#include "lists.h"
#include "circles.h"
#include "commands_head.h"
#include "replay.h"
#include "commands_tail.h"


#define DB(X) printf(" %d", X); fflush(stdout);



int Handle_Input(void);
int Command_List_All(void);
int Circle_List_All(void);

int main(void) {

    Init_Circles();
    Init_Commands();
    Init_Replay();


    while(1) {
        Handle_Input();


        Draw_Circles();

    }
}

    sfEvent event;




int Handle_Input(void) {

    sfVector2i position;
    while(1) {
        sfRenderWindow_waitEvent(window, &event);

        if (event.type == sfEvtMouseMoved) {
            if (!(sfMouse_isButtonPressed (sfMouseLeft))) continue;
            if (event.mouseMove.x < 0) continue;
            if (event.mouseMove.x > 800) continue;
            if (event.mouseMove.y < 0) continue;
            if (event.mouseMove.y > 600) continue;

            Create_Circle_Event(event.mouseMove.x, event.mouseMove.y);
            return 0;

        }

        if (event.type == sfEvtMouseButtonPressed) {
            if (event.mouseButton.button == sfMouseLeft) {
                if (event.mouseButton.x < 0) continue;
                if (event.mouseButton.x > 800) continue;
                if (event.mouseButton.y < 0) continue;
                if (event.mouseButton.y > 600) continue;

                Create_Circle_Event(event.mouseButton.x, event.mouseButton.y);
                return 0;

            } else if (event.mouseButton.button == sfMouseRight) {

                if (event.mouseButton.x < 0) continue;
                if (event.mouseButton.x > 800) continue;
                if (event.mouseButton.y < 0) continue;
                if (event.mouseButton.y > 600) continue;


                fflush(stdout);

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

        if (event.type == sfEvtKeyPressed) {
            if (event.key.code == sfKeyLeft) {

                Command_Undo();

                return 0;
            } else if (event.key.code == sfKeyRight) {
                Command_Redo();
                return 0;
            } else if (event.key.code == sfKeyR) {
                Replay_All();
                return 0;
            } else {
                continue;
            }

        }

    }


}
