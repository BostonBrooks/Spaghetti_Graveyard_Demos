#define COMMANDS_Head



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


DECLARE_POOL(Command, 1028, 1028);
DECLARE_LIST_OPS(Command, List);

#define UNDO 0
#define REDO 1

#define CREATE 0
#define DELETE 1


int (*Command_Functions[16])(int);

int Undo_Head = -1;
int Undo_Tail = -1;
int Redo_Head = -1;
int Redo_Tail = -1;

int Init_Commands(void);
int Clear_Redo_List(void);
int Create_Circle_Event(int, int);
int Command_Create_Circle(int);
int Command_Delete_Circle(int Command_Int);
int Command_Register(int);
int Command_Unregister(int);
int Command_Undo(void);
int Command_Redo(void);
int Create_Circle_Event(int i, int j);
int Delete_Circle_Event(int Circle_Int);

