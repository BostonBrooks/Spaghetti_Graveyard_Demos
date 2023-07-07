/**
 * @file
 *
 * @ingroup 
 * @brief Some data for launching and updating the window
 */
 #define WINDOW

//-----------------------------INCLUDES----------------------------//

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

#ifndef MEDIA
#define MEDIA
#include "../headers/media.h"
#endif

//-----------------------------CONSTANTS----------------------------//

//Constants may be found in constants.h

//-----------------------------STRUCURES------------------------------//

//-----------------------------GLOBALS----------------------------//

/** represents the window */
extern sfRenderWindow* window;


//-----------------------------FUNCTIONS--------------------------//

/** creates the window */
int Window_Init(void);

/** TODO add text */
int Display_Decal(void);

/** TODO Window_Display not defined */
int Window_Display(void);

