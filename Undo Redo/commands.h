#define COMMANDS

#ifndef POOLS
#include "pools.h"
#endif

typedef struct {

    int Pool_Self;
    int Pool_Prev;
    int Pool_Next;
    int Pool_In_Use;

    int List_Prev;
    int List_Next;
    int Type;
    int Function;
    int Data[16];

} Command;

#define UNDO 0
#define REDO 1

#define CREATE 0
#define DELETE 1

DECLARE_POOL(Command, 1028, 1028);
DECLARE_LIST_OPS(Command, List);


int (*Command_Functions[16])(int);

int Undo_Head = -1;
int Undo_Tail = -1;
int Redo_Head = -1;
int Redo_Tail = -1;

int Init_Commands(void);
int Clear_Redo_List(void);
int Create_Circle_Event(int i, int j);
int Command_Register(int);
int Command_Unregister(int);
int Command_Undo(void);
int Command_Redo(void);

int Command_Undo(void){

    if (Undo_Tail != -1){
        assert(Undo_Head != -1);
        Command* Comd = Commands(Undo_Tail);
        Command_Functions[Comd->Function](Undo_Tail);
        
    }

    return 0;
}

int Command_Redo(void){

    if (Redo_Tail != -1){
        assert(Redo_Head != -1);
        Command* Comd = Commands(Redo_Tail);
        Command_Functions[Comd->Function](Redo_Tail);
        
    }

    return 0;
}


int Command_Create_Circle(int Command_Int){

//unregister command
    Command* Object = Commands(Command_Int);

    Command_Unregister(Command_Int);

//change environment
    int Circle_Int = Object->Data[0];
    int Circle_x = Object->Data[1];
    int Circle_y = Object->Data[2];

    Create_Circle(Circle_x, Circle_y, Circle_Int);

    Circle* Circ = Circles(Circle_Int);


  
//update command

    if (Object->Type == UNDO){
        Object->Type = REDO;
    } else {
        Object->Type = UNDO;
    }
    Object->Function = DELETE;
//register command
    Command_Register(Command_Int);
}

int Command_Delete_Circle(int Command_Int){

//unregister command
    Command* Object = Commands(Command_Int);


    Command_Unregister(Command_Int);

//Update Command
    int Circle_Int = Object->Data[0];

    Circle* Circ = Circles(Circle_Int);
    Object->Data[1] = Circ->x; 
    Object->Data[2] = Circ->y;

    if (Object->Type == UNDO){
        Object->Type = REDO;
    } else {
        Object->Type = UNDO;
    }
    Object->Function = CREATE;

//change environment
    Delete_Circle(Circle_Int);


//register command
    Command_Register(Command_Int);


}


int Init_Commands(void){

    Command_Make_NULL();
    Command_Functions[CREATE] = &Command_Create_Circle;
    Command_Functions[DELETE] = &Command_Delete_Circle;

}

int Clear_Redo_List(void){

    int Object_Int = Command_List_Pop_Tail(&Redo_Head, &Redo_Tail);
    while (Object_Int != -1){
        Command_Delete(Object_Int);
        Object_Int = Command_List_Pop_Tail(&Redo_Head, &Redo_Tail);

    }

}

int Create_Circle_Event(int i, int j){

    //create circle
    int Circle_Int = Create_Circle(i, j, NEXT_AVAILABLE);

    //add command to end of undo list

    int Command_Int = Command_New(NEXT_AVAILABLE);
    Command* Object = Commands(Command_Int);


    Object->Function = 1;
    Object->Data[0] = Circle_Int;
    Object->Type = UNDO;

    Command_Register(Command_Int);

    //clear redo list
    Clear_Redo_List();
}

int Delete_Circle_Event(int Circle_Int){

    int Command_Int = Command_New(NEXT_AVAILABLE);
    Command* Object = Commands(Command_Int);
    Circle* Circle_Address = Circles(Circle_Int);

    Object->Function = CREATE;
    Object->Type = UNDO;
    Object->Data[0] = Circle_Int;
    Object->Data[1] = Circle_Address->x;
    Object->Data[2] = Circle_Address->y;

    Command_Register(Command_Int);

    
    Delete_Circle(Circle_Int);

    return 0;
}


int Command_Register(int Command_Int){
    Command* Object = Commands(Command_Int);
//    Command* Prev;

    if (Object->Type == UNDO){
//add to undo tail
        Command_List_Push_Tail(&Undo_Head, &Undo_Tail, Command_Int);


    } else {
//add to redo tail
        Command_List_Push_Tail(&Redo_Head, &Redo_Tail, Command_Int);

    }

}

int Command_Unregister(int Command_Int){

    Command* Object = Commands(Command_Int);

    if (Object->Type == UNDO){
        Command_List_Remove(&Undo_Head, &Undo_Tail, Command_Int);

    } else {
        Command_List_Remove(&Redo_Head, &Redo_Tail, Command_Int);


    }
}
