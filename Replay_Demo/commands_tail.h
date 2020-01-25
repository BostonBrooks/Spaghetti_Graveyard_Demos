#define COMMANDS_TAIL

int Command_Undo(void){

    if (Undo_Tail != -1){
        assert(Undo_Head != -1);
        Command* Comd = Commands(Undo_Tail);
        Command_Functions[Comd->Function](Undo_Tail);

        int Replay_Int = Replay_New(NEXT_AVAILABLE);
        Replay* Replay_Address = Replays(Replay_Int);

        Replay_Address->Function = REPLAY_UNDO;
        Replay_List_Push_Head(&Replay_Head, &Replay_Tail, Replay_Int);



    }

    return 0;
}

int Command_Redo(void){

    if (Redo_Tail != -1){
        assert(Redo_Head != -1);
        Command* Comd = Commands(Redo_Tail);
        Command_Functions[Comd->Function](Redo_Tail);

        int Replay_Int = Replay_New(NEXT_AVAILABLE);
        Replay* Replay_Address = Replays(Replay_Int);

        Replay_Address->Function = REPLAY_REDO;
        Replay_List_Push_Head(&Replay_Head, &Replay_Tail, Replay_Int);
        
    }

    return 0;
}


int Command_Create_Circle(int Command_Int){

//unregister command
    Command* Object = Commands(Command_Int);
//printf("Create %d, %d, %d \n", Object->List_Prev, Object->Pool_Self, Object->List_Next); fflush(stdout);

    Command_Unregister(Command_Int);

//change environment
    int Circle_Int = Object->Data[0];
    int Circle_x = Object->Data[1];
    int Circle_y = Object->Data[2];

    Create_Circle(Circle_x, Circle_y, Circle_Int);

    Circle* Circ = Circles(Circle_Int);


//printf("Circ Created %d, %d, %d \n", Circ->List_Prev, Circ->Pool_Self, Circ->List_Next); fflush(stdout);


    
   // printf("% %d, %d, %d \n", Circ->List_Prev, Circ->Pool_Self, Circ->List_Next);

    
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

//printf("Delete %d, %d, %d \n", Object->List_Prev, Object->Pool_Self, Object->List_Next); fflush(stdout);

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


    int Replay_Int = Replay_New(NEXT_AVAILABLE);
    Replay* Replay_Address = Replays(Replay_Int);

    Replay_Address->Function = REPLAY_CREATE;
    Replay_Address->Data[0] =  i;
    Replay_Address->Data[1] =  j;

    Replay_List_Push_Head(&Replay_Head, &Replay_Tail, Replay_Int);

    return 0;

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

    int Replay_Int = Replay_New(NEXT_AVAILABLE);
    Replay* Replay_Address = Replays(Replay_Int);

    Replay_Address->Function = REPLAY_DELETE;
    Replay_Address->Data[0] =  Circle_Int;

    Replay_List_Push_Head(&Replay_Head, &Replay_Tail, Replay_Int);

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
