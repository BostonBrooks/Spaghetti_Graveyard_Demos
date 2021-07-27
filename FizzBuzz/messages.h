#define MESSAGES

int (*Message_Functions[2])(int address);

#ifndef MESSAGING
#include "messaging.h"
#endif


#define MESSAGE_FIZZBUZZ 0

int fizzbuzz (int address){

    Message* message = Messages(address);

    printf(" %s",message->Text);
    
    fflush(stdout);




    message->Message_Time += message->Frequency;

    return REUSE_MESSAGE;
    
}

#define MESSAGE_BANG 1

int bang (int address){

    Message* message = Messages(address);

    printf(" %s",message->Text);
    
    fflush(stdout);




    message->Message_Time += rand() % message->Frequency;

    return REUSE_MESSAGE;
    
}


int Init_Messages(void){

    Message_Functions[MESSAGE_FIZZBUZZ] = &fizzbuzz;
    Message_Functions[MESSAGE_BANG] = &bang;

}




