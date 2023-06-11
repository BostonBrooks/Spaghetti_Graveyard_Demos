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

extern sfRenderWindow* window; //usually found in window.h

sfFont* prompt_font;

char display_char[PROMPT_BUFFER_SIZE] = "";
sfText* display_text;

char input_char[PROMPT_BUFFER_SIZE] = "";
char temp_char[PROMPT_BUFFER_SIZE] = "";
sfText* input_text;

int collumn = 0;

//-----------------------------FUNCTIONS--------------------------//

int prompt_init(void){

    prompt_font = sfFont_createFromFile("./media/Bowman.ttf");
    if (!prompt_font)
        return EXIT_FAILURE;


    sfVector2f text_position;
    text_position.x = PROMPT_LEFT;
    text_position.y = PROMPT_TOP;
    
    display_text = sfText_create();
    sfText_setFont(display_text, prompt_font);
    sfText_setCharacterSize(display_text, PROMPT_CHAR_SIZE);
    sfText_setPosition(display_text, text_position);
    
    sfColor dark_brown;
    dark_brown.r = 60;
    dark_brown.g = 40;
    dark_brown.b = 19;
    dark_brown.a = 255;
    
    sfText_setColor(display_text, dark_brown);
    

    text_position.x = PROMPT_LEFT + 600;
    text_position.y = PROMPT_TOP;
    
    input_text = sfText_create();
    sfText_setFont(input_text, prompt_font);
    sfText_setCharacterSize(input_text, PROMPT_CHAR_SIZE);
    sfText_setPosition(input_text, text_position);
    sfText_setColor(input_text, dark_brown);
    
    
    prompt_display_string("Welcome to prompt!\n >");

}


int prompt_pass_char(char input){


    if (input == '\n') {
    
        printf("input was: %s\n", input_char);
        prompt_display_string("\n >");
        
        strcpy(temp_char, input_char);
        strcpy(input_char,"");
    
    } else {
    
        temp_char[0] = input;
        temp_char[1] = 0;
        
        strcat(input_char, temp_char);
        strcat(display_char, temp_char); collumn += 1;
        if (collumn >= PROMPT_WIDTH_CHARS){
            temp_char[0] = '\n';
            temp_char[1] = 0;
            strcat(display_char, temp_char);
            collumn = 0;
        }
        
        
    }
    
    
    
    return 0;
    
}

//we may or may not want to pass a string as a result of pressing a button
int prompt_pass_string(char input){

    
}

int chars_after_last_newline(char* input){

    int position = 0;
    int count = 0;
    while(1){
        char character = input[position];
        if (character == '\n') count = 0;
        if (character == '\0') return count-1;
        count++;
        position++;
        
    }

}

int n_rows(char* input){

    int position = 0;
    int count = 0;
    while(1){
        char character = input[position];
        if (character == '\n') count++;
        if (character == '\0') return count+1;
        position++;
        
    }

}


//text to be displayed on screen
int prompt_display_string(char* input){
 
    collumn = chars_after_last_newline(input);
    strcat(display_char, input);
    

}

int nth_newline(char* input, int n){

    int row = 0;
    int position = 0;
    char current_char;
    while(1){
    
        current_char = input[position];
        if (current_char == '\n') row += 1;
        if (current_char = '\0') return 0;
        if (row == n) return position;
        
        position++;
    
    }

}

int prompt_display(void){

    //count the number of rows in display text
    int rows = n_rows(display_char);
    //lop off rows from beginning
    
    int excess_rows = rows - PROMPT_HEIGHT_CHARS;
    
    if (excess_rows > 0) {
    
        int position = nth_newline(display_char, excess_rows);
        
       
        strcpy(temp_char, &display_char[position]);
        strcpy(display_char, temp_char);
    
    
    }
    
    

    sfText_setString(display_text, display_char);
    sfText_setString(input_text, input_char);
    sfRenderWindow_drawText(window, display_text, NULL);
    sfRenderWindow_drawText(window, input_text, NULL);
    
    
}


