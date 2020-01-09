#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>



#ifndef MESSAGING
#include "messaging.h"
#endif




int fizz(int address){


    printf(" fizz");fflush(stdout);

    Message* message = Messages(address);


    int BANGint = Message_New(NEXT_AVAILABLE);
    Message* BANG = Messages(BANGint);
    BANG->Message_Time = message->Message_Time + rand() % 16;
    BANG->Message_Function = 2;
    Message_Register(BANGint);

    message->Message_Time += 3;




    return REUSE_MESSAGE;

   
}

int buzz(int address){


    printf(" buzz");fflush(stdout);

    Message* message = Messages(address);



    message->Message_Time += 5;

    return REUSE_MESSAGE;
    
}

int bang(int address){


    printf(" bang!");fflush(stdout);

    Message* message = Messages(address);



    return DELETE_MESSAGE;

}

int main(void){


    Message_Make_NULL();

    Message_Functions[0] = &fizz;
    Message_Functions[1] = &buzz;
    Message_Functions[2] = &bang;


    int FIZZint = Message_New(NEXT_AVAILABLE);
    Message* FIZZ = Messages(FIZZint);
    FIZZ->Message_Time = 0;
    FIZZ->Message_Function = 0;
    Message_Register(FIZZint);




    int BUZZint = Message_New(NEXT_AVAILABLE);
    Message* BUZZ = Messages(BUZZint);
    BUZZ->Message_Time = 0;
    BUZZ->Message_Function = 1;
    Message_Register(BUZZint);



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


