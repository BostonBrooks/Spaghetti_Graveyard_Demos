#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define    LEVEL1   3
#define    LEVEL2   5


typedef struct {
    int Prev;
    int Next;
    char Name[16];
    int Score;
    int In_Use;
} Object;

Object* Pool[LEVEL1] = { 0 };
//Note on LEVEL1, LEVEL2: Pool[] is an array of pointers to arrays of type object. The number of objects in these arrays is LEVEL2, the maximum number of arrays of type object is LEVEL1; The arrays of type object are allocated when needed.

int Available_Head = -1;
int Available_Tail = -1;

int List_Head = -1;
int List_Tail = -1;

int Increase_Pool(int);

int New_Object(void);
void Delete_Object(int);

void List_Object(int);
void DeList_Object(int);

Object* Lookup_Object(int);

void Print_All(void);
void Print_None(void);



int Initialise_object_int(int);


void Restore_All(void);

void Save_All(void);

void Free_All(void);



void Set_Object_Name(int object_int, char Name[16]);


void Set_Object_Score(int object_int, int Score);



void DeList_Object (int object_int){
    //Remove from list

    Object* object_address = Lookup_Object(object_int);

    if (List_Head == -1 && List_Tail == -1){
        //List Empty

    } else if (object_address->Prev == -1 && object_address->Next == -1) {
        //Object not in list
    

    } else if ( List_Head == object_int && List_Tail == object_int){
        //Only object in list

        List_Head = -1;
        List_Tail = -1;


        object_address->Prev = -1; 
        object_address->Next = -1; 

    } else if (List_Head == object_int) {
        // Object at top of list
        List_Head = object_address->Next;
        object_address->Next = -1;
        object_address->Prev = -1;

        object_address = Lookup_Object(List_Head);
        object_address->Prev = -1;

    } else if (List_Tail == object_int) {
        //Object at end of list
        List_Tail = object_address->Prev;
        object_address->Next = -1;
        object_address->Prev = -1;

        object_address = Lookup_Object(List_Tail);
        object_address->Next = -1;

    } else {
        //Object in middle of list
        Object* Next = Lookup_Object(object_address->Next);
        Object* Prev = Lookup_Object(object_address->Prev);

        Next->Prev = object_address->Prev;
        Prev->Next = object_address->Next;

        object_address->Prev = -1;
        object_address->Next = -1;
    }


}


void Delete_Object(int object_int){
     // Remove from list
     DeList_Object (object_int);

     Object* object_address = Lookup_Object(object_int);
     object_address->In_Use = 0;
     //Return to Pool
     if (Available_Head == -1 && Available_Tail == -1){

         Available_Head = object_int;
         Available_Tail = object_int;

     } else{
         Object* head_address = Lookup_Object(Available_Head);
         head_address->Prev = object_int;
         object_address->Next = Available_Head;
         Available_Head = object_int;
     }

}


int New_Object(void){
    if (Available_Head == -1){ 
        assert (Available_Tail == -1);
        int success = Increase_Pool(-1);
        assert(success == 1);
    }
    int object_int = Available_Head;
    Object* object_address = Lookup_Object(object_int);
    Available_Head = object_address->Next;

    object_address->In_Use = 1;
    object_address->Prev = -1;
    object_address->Next = -1;

    if (Available_Head != -1) {
        Object* Head = Lookup_Object(Available_Head);
        Head->Prev = -1;
    } else {
        Available_Tail = -1;
    }
    return object_int;
}

int Increase_Pool(int i){ 

    if (i == -1){
        for (i = 0; i < LEVEL1; i++) {
            if (Pool[i] == 0) break;  // Clearly is an intended pointer compare
        }
    }

    if (i == LEVEL1) {
        printf("out of memory\n");
        return(0);
    } else if (Pool[i] != 0){
        printf("attempting to overwrite Pool[i]");
        return(0);
    } else {
        Pool[i] =  calloc(LEVEL2, sizeof Pool[i][0]);
        if (Pool[i] == NULL) {

            printf("calloc failed");
            return(0);
        }
    }
    
    int j = 0;

    if (Available_Head == -1){
        assert (Available_Tail == -1);
        Available_Head = i * LEVEL2;
        Available_Tail = i * LEVEL2;

        Object* object = Lookup_Object(Available_Head);
        object->Prev = -1;
        object->Next = i * LEVEL2 + 1;
        object->In_Use = 0;
        j++;
        
    } else {

        Object* Tail = Lookup_Object(Available_Tail);
        Object* New = Lookup_Object(i * LEVEL2);
        New-> In_Use = 0;
        Tail->Next = i * LEVEL2;
        New->Prev = Available_Tail;
        New->Next = i * LEVEL2 + 1;
        j++;
    }


    for (; j < LEVEL2; j++){
    //add elements of Pool[i] to the Available list 
        Object* object_address = Lookup_Object(i * LEVEL2 + j);


        object_address->In_Use = 0;
        object_address->Prev = i * LEVEL2 + j - 1;
        object_address->Next = i * LEVEL2 + j + 1;


    } 

    Available_Tail = (i+1) * LEVEL2 - 1;

    Object* tail_address = Lookup_Object(Available_Tail);

    tail_address->Next = -1;
    tail_address->In_Use = 0;

    return(1);
}

Object* Lookup_Object(int i){

    return (&(Pool[i / LEVEL2])[i % LEVEL2]);
}

void List_Object(int object_int){
    //Remove from list

    DeList_Object(object_int);


    //Add to list

    Object* object_address = Lookup_Object(object_int);

    if (List_Head == -1){ //list empty
        assert (List_Tail == -1);
        List_Head = object_int;
        List_Tail = object_int;
        return;
    }


    Object* head_address = Lookup_Object(List_Head);

    if (object_address->Score >= head_address->Score) {//add to start of list
        object_address->Prev = -1;
        object_address->Next = List_Head;
        head_address->Prev = object_int;
        List_Head = object_int;
        return;
    }

    Object* tail_address = Lookup_Object(List_Tail);
    
    if (object_address->Score <= tail_address->Score) {//add to end of list
        object_address->Next = -1;
        object_address->Prev = List_Tail;
        tail_address->Next = object_int;
        List_Tail = object_int;
        return;
    }

    int target_int = List_Head;
    Object* target_address = Lookup_Object(target_int);

    //add to middle of list
    while(object_address->Score < target_address->Score){
        target_int = target_address->Next;
        target_address = Lookup_Object(target_int);
    }

    int target_prev_int = target_address->Prev;
    Object* target_prev_address = Lookup_Object(target_prev_int);

    target_prev_address->Next = object_int;
    object_address->Prev = target_prev_int;
    object_address->Next = target_int;
    target_address->Prev = object_int;



}

void Print_All(void){

    if (List_Head == -1){
        assert(List_Tail == -1);
        printf("\n List Empty\n\n");
        return;
    }

    int object_int = List_Head;
    Object* object_address;

    printf("\nName     Score\n");

    while (object_int != -1){

        object_address = Lookup_Object(object_int);
        //printf("%s    %d\n", object_address->Name, object_address->Score);

        printf("%s    %d -- %d\n", object_address->Name, object_address->Score, object_address->In_Use); //debug
        assert(object_int != object_address->Next);
        object_int = object_address->Next;

    }

    printf("\n");
    return;

}


//Print_None prints a list of available objects in the pool.
void Print_None(void){

    if (Available_Head == -1){
        assert(Available_Tail == -1);
        printf("\n List Empty\n\n");
        return;
    }

    int object_int = Available_Head;
    Object* object_address;

    printf("\nPrevious     Next\n");

    while (object_int != -1){

        object_address = Lookup_Object(object_int);
        printf("%d    %d\n", object_address->Prev, object_address->Next);
        assert(object_int != object_address->Next);
        object_int = object_address->Next;

    }

    printf("\n");
    return;

}


void Set_Object_Name(int object_int, char Name[16]){
    Object* object_address = Lookup_Object(object_int);
    strcpy(object_address->Name, Name);
}


void Set_Object_Score(int object_int, int Score){

    Object* object_address = Lookup_Object(object_int);
    object_address->Score = Score;
}




int Initialise_object_int(int object_int){

    int i = object_int / LEVEL2;
    if (Pool[i] == 0){
        Increase_Pool(i); //malloc Pools[i] and add to Available_Tail
    }

    Object *object_address =  Lookup_Object(object_int);

    if (object_address->In_Use == 0){ //object not in use?
    //if object specified is in Available list, remove from list
        object_address->In_Use = 1;
    
        if (Available_Head == object_int && Available_Tail == object_int){

            Available_Head = -1;
            Available_Tail = -1;

            object_address->Prev = -1;
            object_address->Next = -1;
        

        } else if (Available_Head == object_int) {
            //remove from head
            Available_Head = object_address->Next;
            Object* next_address = Lookup_Object(object_address->Next);
            next_address->Prev = -1;

        } else if (Available_Tail == object_int) {

            //remove from tail
            Available_Tail = object_address->Prev;
            Object* prev_address = Lookup_Object(object_address->Prev);
            prev_address->Next = -1;

        } else if (Available_Head == object_int) {

            //remove from middle

            Object* prev_address = Lookup_Object(object_address->Prev);
            Object* next_address = Lookup_Object(object_address->Next);

            prev_address->Next = object_address->Next;
            next_address->Prev = object_address->Prev;

            object_address->Next = -1;
            object_address->Prev = -1;

        }





    } else { //if object specified is in "List" remove from list
        DeList_Object(object_int);
    }
}


void Restore_All(void){

    //open scores.txt
    FILE *scores;
    scores = fopen("scores.txt", "r");

    if (scores == NULL) return;

    fscanf(scores, "Index Name Score Prev Next ");

    //grab objects and set values,
    int object_int;
    char Name[16];
    int Score;
    int Prev;
    int Next;

    while (fscanf(scores, " %d %15s %d %d %d "
             , &object_int, Name, &Score, &Prev, &Next )==5){

        Initialise_object_int(object_int);

        Object* object_address = Lookup_Object(object_int);
        strcpy(object_address->Name, Name);
        object_address->Score = Score;
        object_address->Prev = Prev;
        object_address->Next = Next;

        if (object_address->Prev == -1) List_Head = object_int;
        if (object_address->Next == -1) List_Tail = object_int;

        //List_Object(object_int);
    }

    fclose(scores);
    //add to "List"
}

     

void Save_All(void){
    //clear scores.txt
    remove("scores.txt");

    FILE *scores = fopen("scores.txt", "w");
    fprintf(scores, "Index Name Score Prev Next\n\n");
    printf( "Index Name Score Prev Next\n\n");
    //Go through Pool[][] and save all data
    //Close scores.txt

    int i, j; Object* object_address;

    for (i = 0; i < LEVEL1; i++){
         if (Pool[i] != 0) {
              for (j = 0; j < LEVEL2; j++){
                  
                   object_address = Lookup_Object(i * LEVEL2 + j);
                   if (object_address->In_Use != 0){
                        fprintf(scores, "%d %15s %d %d %d\n",
                           i * LEVEL2 +j,
                           object_address->Name,
                           object_address->Score,
                           object_address->Prev,
                           object_address->Next
                       );
                        printf("%d %15s %d %d %d\n",
                           i * LEVEL2 +j,
                           object_address->Name,
                           object_address->Score,
                           object_address->Prev,
                           object_address->Next
                       );
                   }
              }
         }
    }

    fclose(scores);
}

void Free_All(void) {
     int i;
     for (i = 0; i < LEVEL1; i++){
          if (Pool[i] != 0){
              free(Pool[i]);
          }
     }


}












