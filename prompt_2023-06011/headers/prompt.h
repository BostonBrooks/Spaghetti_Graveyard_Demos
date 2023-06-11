#define PROMPT

//-----------------------------INCLUDES----------------------------//

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif



//-----------------------------CONSTANTS----------------------------//

//-----------------------------STRUCURES------------------------------//

//-----------------------------GLOBALS----------------------------//



//-----------------------------FUNCTIONS--------------------------//

int prompt_init(void);

int prompt_pass_char(char);

int prompt_display(void);

int chars_after_last_newline(char* input);
