#define REPLAY



#define DB(X) printf(" %d", X); fflush(stdout);

typedef struct {

    int Pool_Self;
    int Pool_Prev;
    int Pool_Next;
    int Pool_In_Use;

    int List_Prev;
    int List_Next;

    int Function;
    int Data[16];

} Replay;

int Replay_Head = -1;
int Replay_Tail = -1;


DECLARE_POOL(Replay, 1028, 1028);
DECLARE_LIST_OPS(Replay, List);



int (*Replay_Functions[16])(int);



int Foo = 0;
#define REPLAY_FOO 1
#define REPLAY_CREATE 2
#define REPLAY_DELETE 3
#define REPLAY_UNDO 4
#define REPLAY_REDO 5



int Replay_One(int Replay_Int){
    if (Replay_Int < 0) return -1;

    Replay* Object = Replays(Replay_Int);

    if (Object < 0) return -1;

    int flag = Replay_Functions[Object->Function](Replay_Int);

    Draw_Circles();




    return flag;

}

int Replay_All(){

    Circle_Free_All();


    Circles_Head = -1;
    Circles_Tail = -1;

    Command_Free_All();

    Undo_Head = -1;
    Undo_Tail = -1;
    Redo_Head = -1;
    Redo_Tail = -1;
    
    Replay* Replay_Address;
    int flag = 0;

    int Replay_Int = Replay_Tail;


    while (flag >= 0 && Replay_Int >= 0) { 

        usleep(100000);
        Replay_Address = Replays(Replay_Int);

        flag = Replay_One(Replay_Int);

        Replay_Int = Replay_Address->List_Prev;

    }
}


int Replay_Print_Foo(int Replay_Int){

    printf("Foo = %d\n", Foo);fflush(stdout);
    Foo++;
    return 0;
}

int Replay_Create_Circle_Event(int i , int j){

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

    return 0;
}

int Replay_Delete_Circle_Event(int Circle_Int){

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


int Replay_Create_Circle(int Replay_Int){



    Replay* Replay_Address = Replays(Replay_Int);
    int x = Replay_Address->Data[0];
    int y = Replay_Address->Data[1];
    Replay_Create_Circle_Event(x, y);

    return 0;
}

int Replay_Delete_Circle(int Replay_Int){

    Replay* Replay_Address = Replays(Replay_Int);
    int Circle_Int = Replay_Address->Data[0];
    Replay_Delete_Circle_Event(Circle_Int);

    return 0;
}

int Replay_Undo(int Replay_Int){

        Command* Comd = Commands(Undo_Tail);
        Command_Functions[Comd->Function](Undo_Tail);
}

int Replay_Redo(int Replay_Int){

        Command* Comd = Commands(Redo_Tail);
        Command_Functions[Comd->Function](Redo_Tail);



}
int Init_Replay(void){

    Replay_Make_NULL();
    Replay_Functions[REPLAY_FOO] = &Replay_Print_Foo;
    Replay_Functions[REPLAY_CREATE] = &Replay_Create_Circle;
    Replay_Functions[REPLAY_DELETE] = &Replay_Delete_Circle;
    Replay_Functions[REPLAY_UNDO] = &Replay_Undo;
    Replay_Functions[REPLAY_REDO] = &Replay_Redo;

}


