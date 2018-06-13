    // ---- Includes ---- //

#include <time.h>
#include <SFML/System.h>
#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>

    // ---- Data Types ---- //

typedef sfVector2i XY_Point;

typedef struct {
    int i;
    int j;
} IJ_Point;


    // ---- Globals ---- //


int GameTime = 0;
int ReferenceTime = 0;
int GamePaused = 0;
int NowTime = 0;
sfClock* GameClock;
sfTime sftime;

    // ---- Init Function ---- //
int Init_Rand (void) {
    srand (time(NULL));
}

int Init_Time(void) {
    GameClock = sfClock_create();
    //Sets game time to 0
    GamePaused = 0;
    GameTime = 0;
    ReferenceTime = sfTime_asMilliseconds(sfClock_getElapsedTime(GameClock));	
    return GameTime;
}

    // ---- Other Functions ---- //

int GetTime(void) {
    //Returns game time in ms
    if (!GamePaused) {
        return (GameTime + sfTime_asMilliseconds(sfClock_getElapsedTime (GameClock)) - ReferenceTime);
    } else {
        return GameTime;
    }
    
}

int UpdateTime (void) {
    NowTime = GetTime();
}

int PauseTime(void) {
    //Returns game time in ms, stops time passing
    GamePaused = 1;
    int t = sfTime_asMilliseconds(sfClock_getElapsedTime (GameClock));
    GameTime = GameTime + t - ReferenceTime;
    ReferenceTime = t;
    return GameTime;
}

int UnpauseTime(void) {
    //Returns game time in ms, starts time passing
    ReferenceTime =  sfTime_asMilliseconds(sfClock_getElapsedTime (GameClock));
    GamePaused = 0;
    return GameTime;
}

    // ---- Release Function ---- //

int Release_Environment() {

}
