#define MESSAGING

#ifndef POOLS
#include "pools.h"
#endif



#define REUSE_MESSAGE     1
#define DELETE_MESSAGE    0


typedef struct {
    //
    int Pool_Self;
    int Pool_Prev;
    int Pool_Next;
    int Pool_In_Use;
    //
    //... data ...
    int Message_Time;
    int Message_Next;
    char Text[16];
    int Frequency;
    //
    //... functions ...
    int Message_Function;
} Message;

DECLARE_POOL(Message, 100, 3)


int Game_Time = 0;
int Message_Head = -1;

//
//void Message_Make_NULL();
//int Message_New(int);
//int Message_Delete(int Address);
//Message* Messages(int Address);

int Message_Register(int Address);
int Message_Insert_After(Message*, Message*);
int Message_Check(void);
int Message_Loop(void);


int Message_Insert_After(Message* message, Message* after){

    message->Message_Next = after->Message_Next;
    after->Message_Next = message->Pool_Self;

    return 1;
}

int Message_Register (int Address) {
    Message* message = Messages(Address);

    Message* Next;
    Message* Prev;

    if (Message_Head == -1) {
        Message_Head = Address;
        message->Message_Next = -1;
        return 1;
    }

    Next = Messages(Message_Head);

    if (message->Message_Time < Next->Message_Time) {
        message->Message_Next = Message_Head;
        Message_Head = Address;
        return 1;
    }


    //loop until:
    //A - Next has a greater Message_Time than Message,
    //or
    //B - Message_Next == -1

    //If A - insert Message before Next
    //If B - insert Message after Next


    int Next_Int;

    Next = Messages(Message_Head);

    while(1) {
        Prev = Next;
        Next_Int = Next->Message_Next;
        if(Next_Int == -1) break;

        Next = Messages(Next_Int);
        if (Next->Message_Time > message->Message_Time) break;

    }

    if (Next_Int == -1) {
        Next->Message_Next = Address;
        message->Message_Next = -1;
        return 1;
    }

    if (Next->Message_Time > message->Message_Time) {
        Prev->Message_Next = Address;
        message->Message_Next = Next_Int;
        return 1;
    }
}

int Message_Check(void) {



    if (Message_Head == -1) return 0;



    Message* message = Messages(Message_Head);




    if (message->Message_Time > Game_Time) return 0;


    int Address = Message_Head;


    Message_Head = message->Message_Next; // Possibly -1


    int Flag = Message_Functions[message->Message_Function](Address);


    if (Flag == REUSE_MESSAGE){ Message_Register(Address);

    }

    else {

        Message_Delete(Address);
    }

    return 1;

}

int Message_Loop(void) {


    int Flag = 1;



    while (Flag != 0) {

        Flag = Message_Check();

    }


    return 1;
}
