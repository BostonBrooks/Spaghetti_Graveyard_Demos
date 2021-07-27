#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>



#ifndef MESSAGES
#include "messages.h"
#endif






int main(void){


    Message_Make_NULL();

    Init_Messages();


    int FIZZint = Message_New(NEXT_AVAILABLE);
    Message* FIZZ = Messages(FIZZint);
    FIZZ->Message_Time = 0;
    FIZZ->Message_Function = MESSAGE_FIZZBUZZ;
    strncpy(FIZZ->Text, " fizz", 16);
    FIZZ->Frequency = 3;
    Message_Register(FIZZint);

    int BUZZint = Message_New(NEXT_AVAILABLE);
    Message* BUZZ = Messages(BUZZint);
    BUZZ->Message_Time = 0;
    BUZZ->Message_Function = MESSAGE_FIZZBUZZ;
    strncpy(BUZZ->Text, " buzz", 16);
    BUZZ->Frequency = 5;
    Message_Register(BUZZint);

    int BANGint = Message_New(NEXT_AVAILABLE);
    Message* BANG = Messages(BANGint);
    BANG->Message_Time = 0;
    BANG->Message_Function = MESSAGE_BANG;
    strncpy(BANG->Text, " bang", 16);
    BANG->Frequency = 7;
    Message_Register(BANGint);






    int object_int = 0;
    Message* Object;
    while (object_int != -1){
        Object = Messages(object_int);
        object_int = Object->Message_Next;

    }

    for(Game_Time = 0; Game_Time < 128; Game_Time++){


        printf("\n%d ", Game_Time);fflush(stdout);


        Message_Loop();


        
    }


    exit(0);
}


