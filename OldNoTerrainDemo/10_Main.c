    // ---- Includes ---- //
#include "01_Environment.h"
#include "02_Window.h"
#include "03_Surface.h"
#include "04_Inanimate.h"
#include "05_Weapons.h"
#include "06_Units.h"
#include "07_Player.h"
#include "08_Text.h"
#include "09_Control.h"



    // ---- Data Types ---- //


    // ---- Globals ---- //


    // ---- Init Function ---- //



    // ---- Other Functions ---- //



    // ---- Entry Point ---- //

int main (void) {
    Init_Rand();
    Init_Window();
    Init_Surfaces();
    Init_Inanimate();
    Init_Weapons();
    Init_Units();
    Init_Player();
    Init_Text();
    Init_Time();
    Init_Control();

    while (1) {
        GetMouse();
        UpdateTime();
// Handle Input
        CheckSelection();
        GetKey();
        UpdateUnitsLocation();
        UpdateProjectiles();
        UpdateViewPoint();
        SortedClear();
        ListVisible();
        ClearVisibleUnits();
        ListVisibleUnits();
        ClearVisibleProjectiles();
        ListVisibleProjectiles();
        TargetPlayer();
        CalcForces();
        CalcPath();

        sfRenderWindow_clear (window, sfColor_fromRGB (255,255,255));
        Plot_Surfaces();
        Plot_Circles();
        Plot_Units_Objects_Projectiles();
        Plot_Health_Bars();
        Plot_Text();
        sfRenderWindow_display(window);

//We're so good, we can sit around doing nothing
        sfTime durationa;
        durationa.microseconds = 20000;
        sfSleep (durationa);
    }

    Release_Control();
    Release_Text();
    Release_Player();
    Release_Units();
    Release_Weapons();
    Release_Inanimate();
    Release_Surfaces();
    Release_Window();
    Release_Environment();
}
