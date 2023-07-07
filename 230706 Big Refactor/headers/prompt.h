/**
 * @file
 *
 * @ingroup 
 * @brief Handle text input
 */

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

#ifndef INPUT
#define INPUT
#include "../headers/input.h"
#endif



//-----------------------------CONSTANTS----------------------------//

//-----------------------------STRUCURES------------------------------//

//-----------------------------GLOBALS----------------------------//

/** Usually found in window.h */
extern sfRenderWindow* window; //usually found in window.h

extern sfFont* prompt_font;


/** Text string visible on screen */
extern char display_char[PROMPT_BUFFER_SIZE];
/** Text visible on screen */
extern sfText* display_text;

/** Text string being input by player */
extern char input_char[PROMPT_BUFFER_SIZE];

/** Used in various locations as a temp value, since we cant put it on the stack,
 and we are avoiding using malloc{}*/
extern char temp_char[PROMPT_BUFFER_SIZE];

/** Text displaying current input */ 
extern sfText* input_text;

/** The last think typed before pressing enter */
extern char last_input[PROMPT_BUFFER_SIZE];

/** Has the last thing typed before pressing enter been processed externally? */
extern int input_processed;

/** Keeps track of length of string being entered, so newline character can be added if the line wraps */
extern int collumn;

//-----------------------------FUNCTIONS--------------------------//

/** Initiates text input handling */
int prompt_init(void);

/** Handles keypress passed by input.c */
int prompt_pass_char(char input);

/** We may or may not want to pass a string as a result of pressing a button */
int prompt_pass_string(char* input);

/** TODO update text */
int chars_after_last_newline(char* input);

/** Count the number of newline characters in a string */
int n_rows(char* input);


/** Pew Pew */
int prompt_display_string(char* input);

/** Return the position of the nth newline in a string */
int nth_newline(char* input, int n);

/** Display text to screen */
int prompt_display(void);

