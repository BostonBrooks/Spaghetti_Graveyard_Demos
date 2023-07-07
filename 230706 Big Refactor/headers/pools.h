#define POOLS

#ifndef SYSTEM_INCLUDES
#define SYSTEM_INCLUDES
#include "../headers/system_includes.h"
#endif

// Implements object pools to speed up code that frequently adds and removes
// data from the heap. Additionally helps with pointer swizzling

// Suggested usage:

// assume sizeof(long) == sizeof(void*)
// Let "Object" be the name of the class,
// Let "LEVEL_1" be the size of the first level of the pool
// Let "LEVEL_2" be the size of the second level of the pool
// (The pool is an array of pointers to arrays of Objects)
// (to reiterate, memory address = &Object_Pool[l1][l2])

    // #ifndef POOLS
    // #include "pools.h"
    // #endif
    //
    // typedef struct{
    //
    //     int Pool_Self;
    //     int Pool_Prev;
    //     int Pool_Next;
    //     int Pool_In_Use;
    //
    //     ... data ...
    //     ... functions ...
    //
    // } Object;

    // DEFINE_POOL(Object, LEVEL_1, LEVEL_2)

    // int main (){
    //     Object_Make_NULL();
    //
    //     ...
    //
    // }



// Error Codes:
#define SUCCESS                           0
#define OBJECT_IN_USE                    -1
#define OBJECT_NOT_IN_USE                -2
#define LEVEL_1_POOL_NOT_INITIALISED     -3
#define LEVEL_1_POOL_ALREADY_INITIALISED -4
#define LEVEL_1_POOL_OUT_OF_BOUNDS       -5
#define MALLOC_FAILED                    -6
#define POOL_FULL                        -7
#define LEVEL_1_POOL_FULL                -8

// Command Codes:

#define NEXT_AVAILABLE                   -1

// The Code:


#define DECLARE_POOL_GLOBALS(CLASS, LEVEL_1, LEVEL_2)\
\
    extern CLASS* CLASS##_Pool[LEVEL_1];\
\
    extern int CLASS##_Pool_Available_Head;\
    extern int CLASS##_Pool_Available_Tail;\
\
    extern int CLASS##_Pool_In_Use_Head;\
    extern int CLASS##_Pool_In_Use_Tail;\
\
    int CLASS##_Pool_Make_Null(void);\
\
    int CLASS##_Pool_New(int);\
    int CLASS##_Pool_Delete(int Address);\
\
    int CLASS##_Pool_Increase(int Level1_Address);\
\
    CLASS* CLASS##_Pool_Lookup(int Address);\
    CLASS* CLASS##_Pool_Lookup_sudo(int Address);\
\
    int CLASS##_Pool_Free_All(void);\
\

#define DEFINE_POOL_GLOBALS(CLASS, LEVEL_1, LEVEL_2)\
\
    CLASS* CLASS##_Pool[LEVEL_1];\
\
    int CLASS##_Pool_Available_Head = -1;\
    int CLASS##_Pool_Available_Tail = -1;\
\
    int CLASS##_Pool_In_Use_Head = -1;\
    int CLASS##_Pool_In_Use_Tail = -1;\
\
    int CLASS##_Pool_Make_Null(void);\
\
    int CLASS##_Pool_New(int);\
    int CLASS##_Pool_Delete(int Address);\
\
    int CLASS##_Pool_Increase(int Level1_Address);\
\
    CLASS* CLASS##_Pool_Lookup(int Address);\
    CLASS* CLASS##_Pool_Lookup_sudo(int Address);\
\
    int CLASS##_Pool_Free_All(void);\
\


//initialise pointers to NULL
#define DEFINE_POOL_MAKE_NULL(CLASS, LEVEL_1, LEVEL_2)\
\
\
    int CLASS##_Pool_Make_Null(void) {\
        for (int i = 0; i < LEVEL_1; i++){\
            CLASS##_Pool[i] = NULL;\
        }\
    }\
\

//Allocate memory for use in pool, add to tail end of available list.
#define DEFINE_POOL_INCREASE(CLASS, LEVEL_1, LEVEL_2)\
    int CLASS##_Pool_Increase(int Level1_Address){\
\
        int i = 0;\
        if(Level1_Address == NEXT_AVAILABLE){\
            while(CLASS##_Pool[i] != NULL && i < LEVEL_1) {\
               i++;\
            }\
            if (i == LEVEL_1) return (LEVEL_1_POOL_FULL);\
        } else {\
            i = Level1_Address;\
            if (CLASS##_Pool[i] != NULL){\
                return(LEVEL_1_POOL_ALREADY_INITIALISED);\
            }\
        }\
\
        CLASS *j = calloc(LEVEL_2, sizeof(CLASS));\
        if  (j == NULL) return (MALLOC_FAILED);\
        CLASS##_Pool[i] = j;\
\
        if (CLASS##_Pool_Available_Head == -1){\
            assert(CLASS##_Pool_Available_Tail == -1);\
\
            for (int l = 0; l < LEVEL_2; l++) {\
                CLASS##_Pool[i][l].Pool_Self = i * LEVEL_2 + l;\
                CLASS##_Pool[i][l].Pool_Prev = i * LEVEL_2 + l - 1;\
                CLASS##_Pool[i][l].Pool_Next = i * LEVEL_2 + l + 1;\
                CLASS##_Pool[i][l].Pool_In_Use = 0;\
\
            }\
\
            CLASS##_Pool[i][0].Pool_Prev = -1;\
            CLASS##_Pool[i][LEVEL_2 - 1].Pool_Next = -1;\
\
            CLASS##_Pool_Available_Head = i * LEVEL_2;\
            CLASS##_Pool_Available_Tail = (i+1) * LEVEL_2 - 1;\
            \
        } else {\
\
            for (int l = 0; l < LEVEL_2; l++) {\
                CLASS##_Pool[i][l].Pool_Self = i * LEVEL_2 + l;\
                CLASS##_Pool[i][l].Pool_Prev = i * LEVEL_2 + l - 1;\
                CLASS##_Pool[i][l].Pool_Next = i * LEVEL_2 + l + 1;\
                CLASS##_Pool[i][l].Pool_In_Use = 0;\
\
            }\
            CLASS##_Pool[i][LEVEL_2 - 1].Pool_Next = -1;\
            CLASS##_Pool[i][0].Pool_Prev = CLASS##_Pool_Available_Tail;\
            CLASS##_Pool_Available_Tail = (i+1) * LEVEL_2 -1; \
\
        }\
    }\

//if address not given, find the next available address.
//remove from available list, set in use flag to 1
//////update available/in use head & tail/////TODO
//add to in use list, return address
#define DEFINE_POOL_NEW(CLASS, LEVEL_1, LEVEL_2)\
    int CLASS##_Pool_New(int Address){\
        assert(Address == NEXT_AVAILABLE);\
\
        if (CLASS##_Pool_Available_Head == -1){\
            assert(CLASS##_Pool_Available_Tail == -1);\
            CLASS##_Pool_Increase(NEXT_AVAILABLE);\
        }\
\
        Address = CLASS##_Pool_Available_Head;\
        CLASS* Object = CLASS##_Pool_Lookup_sudo(Address);\
        CLASS##_Pool_Available_Head = Object->Pool_Next;\
        if(CLASS##_Pool_Available_Head == -1){\
            CLASS##_Pool_Available_Tail = -1;\
        } else {\
            CLASS* Head = CLASS##_Pool_Lookup_sudo(CLASS##_Pool_In_Use_Head);\
            Head->Pool_Prev = -1;\
        }\
\
        Object->Pool_In_Use = 1;\
\
        if(CLASS##_Pool_In_Use_Head == -1){\
            assert(CLASS##_Pool_In_Use_Tail == -1);\
            CLASS##_Pool_In_Use_Head = Address;\
            CLASS##_Pool_In_Use_Tail = Address;\
            Object->Pool_Prev = -1;\
            Object->Pool_Next = -1;\
            return Address;\
        }\
\
        CLASS* Tail = CLASS##_Pool_Lookup(CLASS##_Pool_In_Use_Tail);\
        Tail->Pool_Next = Address;\
        Object->Pool_Prev = CLASS##_Pool_In_Use_Tail;\
        Object->Pool_Next = -1;\
        CLASS##_Pool_In_Use_Tail = Address;\
        return Address;\
    }\
\

// remove object from in use list, add object to available list
#define DEFINE_POOL_DELETE(CLASS, LEVEL_1, LEVEL_2)\
    int CLASS##_Pool_Delete(int Address){\
\
        CLASS* Object = CLASS##_Pool_Lookup(Address);\
\
        if ((long)Object == LEVEL_1_POOL_NOT_INITIALISED){\
            return(LEVEL_1_POOL_NOT_INITIALISED);\
        }\
\
        if ((long)Object == OBJECT_NOT_IN_USE){\
            return(OBJECT_NOT_IN_USE);\
        }\
\
        if (Object->Pool_Prev == -1 && Object->Pool_Next == -1){\
\
            assert (CLASS##_Pool_In_Use_Head == Address);\
            assert (CLASS##_Pool_In_Use_Tail == Address);\
            CLASS##_Pool_In_Use_Head = -1;\
            CLASS##_Pool_In_Use_Tail = -1;\
        } else if (Object->Pool_Prev == -1){\
\
            assert(CLASS##_Pool_In_Use_Head == Address);\
\
            CLASS##_Pool_In_Use_Head = Object->Pool_Next;\
\
            CLASS* Next = CLASS##_Pool_Lookup(Object->Pool_Next);\
\
            Next->Pool_Prev = -1;\
\
        } else if (Object->Pool_Next == -1){\
\
            assert(CLASS##_Pool_In_Use_Tail == Address);\
\
            CLASS##_Pool_In_Use_Tail = Object->Pool_Prev;\
\
            CLASS* Prev = CLASS##_Pool_Lookup(Object->Pool_Prev);\
\
            Prev->Pool_Next = -1;\
\
        } else {\
\
            CLASS* Prev = CLASS##_Pool_Lookup(Object->Pool_Prev);\
            CLASS* Next = CLASS##_Pool_Lookup(Object->Pool_Next);\
\
            Prev->Pool_Next = Object->Pool_Next;\
            Next->Pool_Prev = Object->Pool_Prev;\
        }\
\
        Object->Pool_In_Use = 0;\
\
        if (CLASS##_Pool_Available_Head == -1){\
\
            CLASS##_Pool_Available_Tail == -1;\
\
            CLASS##_Pool_Available_Head = Address;\
            CLASS##_Pool_Available_Tail = Address;\
\
            Object->Pool_Prev = -1;\
            Object->Pool_Next = -1;\
\
        } else {\
            Object->Pool_Prev = -1;\
            Object->Pool_Next = CLASS##_Pool_Available_Head;\
            CLASS* Head = CLASS##_Pool_Lookup_sudo ( CLASS##_Pool_Available_Head);\
            Head->Pool_Prev = Address;\
            CLASS##_Pool_Available_Head = Address;\
        }\
    }\

//Return a pointer to object at integer address in pool
#define DEFINE_POOL_LOOKUP(CLASS, LEVEL_1, LEVEL_2)\
    CLASS* CLASS##_Pool_Lookup(int Address){\
        assert(Address >= 0);\
\
        int i = Address / LEVEL_2;\
        int j = Address % LEVEL_2;\
\
        if (CLASS##_Pool[i] == NULL){\
            return((long)LEVEL_1_POOL_NOT_INITIALISED);\
        }\
\
        if (CLASS##_Pool[i][j].Pool_In_Use == 0){\
            return((long)OBJECT_NOT_IN_USE);\
        }\
        CLASS* obj = &CLASS##_Pool[i][j];\
\
        return (obj);\
    }\
\
    CLASS* CLASS##_Pool_Lookup_sudo(int Address){\
\
        int i = Address / LEVEL_2;\
        int j = Address % LEVEL_2;\
\
        if (CLASS##_Pool[i] == NULL){\
            return((long)LEVEL_1_POOL_NOT_INITIALISED);\
        }\
\
        return (&(CLASS##_Pool[i][j]));\
    }\

//dealocate all memory used in the second level of the pool
#define DEFINE_POOL_FREE_ALL(CLASS, LEVEL_1, LEVEL_2)\
\
    int CLASS##_Pool_Free_All(void) {\
        for (int i = 0; i < LEVEL_1; i++){\
            if (CLASS##_Pool[i] != NULL)\
                free(CLASS##_Pool[i]);\
                CLASS##_Pool[i] = NULL;\
        }\
        \
        CLASS##_Pool_Available_Head = -1;\
        CLASS##_Pool_Available_Tail = -1;\
\
        CLASS##_Pool_In_Use_Head = -1;\
        CLASS##_Pool_In_Use_Tail = -1;\
    }\
\
\
    
//declare all of the globals and functions used by the pool
#define DEFINE_POOL(CLASS, LEVEL_1, LEVEL_2)\
\
    DEFINE_POOL_GLOBALS   (CLASS, LEVEL_1, LEVEL_2)\
    DEFINE_POOL_MAKE_NULL (CLASS, LEVEL_1, LEVEL_2)\
    DEFINE_POOL_INCREASE  (CLASS, LEVEL_1, LEVEL_2)\
    DEFINE_POOL_NEW       (CLASS, LEVEL_1, LEVEL_2)\
    DEFINE_POOL_DELETE    (CLASS, LEVEL_1, LEVEL_2)\
    DEFINE_POOL_LOOKUP    (CLASS, LEVEL_1, LEVEL_2)\
    DEFINE_POOL_FREE_ALL  (CLASS, LEVEL_1, LEVEL_2)

