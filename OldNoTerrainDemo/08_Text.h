    // ---- Data Types ---- //


    // ---- Globals ---- //

sfFont* font;

sfText* dialogue;
char dialogue_str[1000];
int dialogue_len = 0;

sfText* prompt;
char prompt_str[1000];
int prompt_len = 0;

sfText* input;
char input_str[1000];
int input_len = 0;

int inputnum;


    // ---- Init Function ---- //

int Init_Text (void) {

/* Loading a Font */
    font = sfFont_createFromFile ("Bowman.ttf");

    sfVector2f position;
    sfColor brown = sfColor_fromRGB(55, 28, 0);

//init dialogue
    dialogue_str[0] = '\0';
    dialogue = sfText_create();
    sfText_setFont (dialogue, font);
    sfText_setCharacterSize (dialogue, 20);
    position.x = 810;
    position.y = 10;
    sfText_setPosition (dialogue, position);
    sfText_setColor(dialogue, brown);
    sfText_setString (dialogue, "Hello Dialogue\n12345678901234567890123456789012345678\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16");

//init promt
    prompt_str[0] = '\0';
    prompt = sfText_create();
    sfText_setFont (prompt, font);
    sfText_setCharacterSize (prompt, 20);
    position.x = 810;
    position.y = 310;
    sfText_setPosition (prompt, position);
    sfText_setColor(prompt, brown);
    sfText_setString (prompt, "Hello Prompt\n12345678901234567890123456789012345678\n3\n4\n5\n6\n7");

//init input
    input_str[0] = '\0';
    input = sfText_create();
    sfText_setFont (input, font);
    sfText_setCharacterSize (input, 20);
    position.x = 810;
    position.y = 460;
    sfText_setPosition (input, position);
    sfText_setColor(input, brown);
    sfText_setString (input, "Hello Input");

}
    // ---- Plot Function ---- //

int Plot_Text (void) {

//Clear area for text


    sfVertex points[6];

    points[0].color = sfWhite;
    points[1].color = sfWhite;
    points[2].color = sfWhite;
    points[3].color = sfWhite;

    points[0].position.x = 800;
    points[0].position.y = 0;
    points[1].position.x = 1240;
    points[1].position.y = 0;
    points[2].position.x = 800;
    points[2].position.y = 600;
    points[3].position.x = 1240;
    points[3].position.y = 600;

    sfRenderWindow_drawPrimitives(    window,
                                      &points[0],
                                      4,
                                      sfTrianglesStrip,
                                      NULL//const sfRenderStates *  	states 
                                 );

//Draw dividers between viewport and text

    sfColor brown = sfColor_fromRGB(55, 28, 0);


    points[0].color = brown;
    points[1].color = brown;
    points[2].color = brown;
    points[3].color = brown;
    points[4].color = brown;
    points[5].color = brown;

    points[0].position.x = 800;
    points[0].position.y = 0;
    points[1].position.x = 800;
    points[1].position.y = 600;
    points[2].position.x = 800;
    points[2].position.y = 300;
    points[3].position.x = 1240;
    points[3].position.y = 300;
    points[4].position.x = 800;
    points[4].position.y = 450;
    points[5].position.x = 1240;
    points[5].position.y = 450;

    sfRenderWindow_drawPrimitives(    window,
                                      &points[0],
                                      6,
                                      sfLines,
                                      NULL//const sfRenderStates *  	states 
                                 );

    sfRenderWindow_drawText(window, dialogue, NULL);
    sfRenderWindow_drawText(window, prompt, NULL);
    sfRenderWindow_drawText(window, input, NULL);

    return 5;
    

}
    // ---- Other Functions ---- //

int append_dialogue(const char* format, ...)
{
    va_list args;
    char str[256];

//pass arguments to sprintf
    va_start(args, format);
    vprintf(format, args);
    vsprintf(str, format, args);
    va_end(args);

    int source = 0;
//append exisiting dialogue
    while (str[source] != '\0') {
        dialogue_str[dialogue_len] = str[source];
        dialogue_len++;
        source++;
    }
    dialogue_str[dialogue_len] = '\0';

//add newline characters to wrap at most 38 characters + newline (be wary of existing newline characters)
//how should i insert newline characters into an existing string? Use a temporary intermediate string.  
    char intermediate[1000];
    source = 0;
    int destination = 0;
    int lines = 0;
    int chars_per_line = 0;

    while (dialogue_str[source] != '\0') {
        if (dialogue_str[source] == '\n') {   //What if you get "\n\n"? \n is treated as any character and the loop repeats.
            lines++;
            chars_per_line = 0;
        } else {
            chars_per_line++;
        }
        if (chars_per_line == 38) {    //we probably want to wrap only on a whitespace character... this leads to a new problem, what if there ae no whitespace characters?
            intermediate[destination] = '\n';
            destination++;
            lines++;
            chars_per_line = 0;
        }
        intermediate[destination] = dialogue_str[source];
        source++;
        destination++;
    }
    intermediate[destination] = '\0';
//delete lines from to top to make room at the bottom 16 lines

    source = 0;
    destination = 0;
    int lines_so_far = 0;
    //search through intermediate[] until the (lines - 16)th newline character
    while (lines_so_far < lines - 17) {
        if (intermediate[source] == '\n') {   //What if you get "\n\n"? \n is treated as any character and the loop repeats.
            lines_so_far++;
        }
        source++;
    }
    //update dialogue_len to take into account the number of chars being deleted by deleting lines.
    dialogue_len -= source;

    //start from there and copy intermediate[] back to dialogue_str[];
    while (intermediate[source] != '\0') {
        dialogue_str[destination] = intermediate[source];
        source++;
        destination++;
    }
    dialogue_str[destination] = '\0';

    sfText_setString (dialogue, dialogue_str);

//max 608 characters + str + \0 (I just used 1000)
}


int append_prompt(const char* format, ...)
{
    va_list args;
    char str[256];

//pass arguments to sprintf
    va_start(args, format);
    vprintf(format, args);
    vsprintf(str, format, args);
    va_end(args);

    int source = 0;
//append exisiting dialogue
    while (str[source] != '\0') {
        prompt_str[prompt_len] = str[source];
        prompt_len++;
        source++;
    }
    prompt_str[prompt_len] = '\0';

//add newline characters to wrap at most 38 characters + newline (be wary of existing newline characters)
//how should i insert newline characters into an existing string? Use a temporary intermediate string.  
    char intermediate[1000];
    source = 0;
    int destination = 0;
    int lines = 0;
    int chars_per_line = 0;

    while (prompt_str[source] != '\0') {
        if (prompt_str[source] == '\n') {   //What if you get "\n\n"? \n is treated as any character and the loop repeats.
            lines++;
            chars_per_line = 0;
        } else {
            chars_per_line++;
        }
        if (chars_per_line == 38) {    //we probably want to wrap only on a whitespace character... this leads to a new problem, what if there ae no whitespace characters?
            intermediate[destination] = '\n';
            destination++;
            lines++;
            chars_per_line = 0;
        }
        intermediate[destination] = prompt_str[source];
        source++;
        destination++;
    }
    intermediate[destination] = '\0';
//delete lines from to top to make room at the bottom 16 lines

    source = 0;
    destination = 0;
    int lines_so_far = 0;
    //search through intermediate[] until the (lines - 7)th newline character
    while (lines_so_far < lines - 7) {
        if (intermediate[source] == '\n') {   //What if you get "\n\n"? \n is treated as any character and the loop repeats.
            lines_so_far++;
        }
        source++;
    }
    //update dialogue_len to take into account the number of chars being deleted by deleting lines.
    prompt_len -= source;

    //start from there and copy intermediate[] back to dialogue_str[];
    while (intermediate[source] != '\0') {
        prompt_str[destination] = intermediate[source];
        source++;
        destination++;
    }
    prompt_str[destination] = '\0';

    sfText_setString (prompt, prompt_str);
//append exisiting dialogue
//add newline characters to wrap at most 38 characters + newline
//delete lines from to top to make room at the bottom 7 lines
//max 266 characters + str + \0  (I just used 1000)
}


int append_input(const char* format, ...)
{
    va_list args;
    char str[256];

//pass arguments to sprintf
    va_start(args, format);
    vprintf(format, args);
    vsprintf(str, format, args);
    va_end(args);

    int source = 0;
//append exisiting dialogue
    while (str[source] != '\0') {
        input_str[input_len] = str[source];
        input_len++;
        source++;
    }
    input_str[input_len] = '\0';

//add newline characters to wrap at most 38 characters + newline (be wary of existing newline characters)
//how should i insert newline characters into an existing string? Use a temporary intermediate string.  
    char intermediate[1000];
    source = 0;
    int destination = 0;
    int lines = 0;
    int chars_per_line = 0;

    while (input_str[source] != '\0') {
        if (input_str[source] == '\n') {   //What if you get "\n\n"? \n is treated as any character and the loop repeats.
            lines++;
            chars_per_line = 0;
        } else {
            chars_per_line++;
        }
        if (chars_per_line == 38) {    //we probably want to wrap only on a whitespace character... this leads to a new problem, what if there ae no whitespace characters?
            intermediate[destination] = '\n';
            destination++;
            lines++;
            chars_per_line = 0;
        }
        intermediate[destination] = input_str[source];
        source++;
        destination++;
    }
    intermediate[destination] = '\0';
//delete lines from to top to make room at the bottom 16 lines

    source = 0;
    destination = 0;
    int lines_so_far = 0;
    //search through intermediate[] until the (lines - 7)th newline character
    while (lines_so_far < lines - 7) {
        if (intermediate[source] == '\n') {   //What if you get "\n\n"? \n is treated as any character and the loop repeats.
            lines_so_far++;
        }
        source++;
    }
    //update dialogue_len to take into account the number of chars being deleted by deleting lines.
    input_len -= source;

    //start from there and copy intermediate[] back to dialogue_str[];
    while (intermediate[source] != '\0') {
        input_str[destination] = intermediate[source];
        source++;
        destination++;
    }
    input_str[destination] = '\0';

    sfText_setString (input, input_str);
//append exisiting dialogue
//add newline characters to wrap at most 38 characters + newline
//delete lines from to top to make room at the bottom 7 lines
//max 266 characters + str + \0  (I just used 1000)
}

int input_num (int input) {
//Handle negative numbers and decimals
    inputnum = inputnum * 10 + input;
    return inputnum;
}

int input_enter(void) {
    printf("\n%i\n", inputnum);
    inputnum = 0;
    return inputnum;
} 


    // ---- Release Function ---- //

int Release_Text() {

}

