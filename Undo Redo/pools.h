#define POOLS


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

    // DECLARE_POOL(Object, LEVEL_1, LEVEL_2)

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


#define CONCAT1(x,y) x##y
#define CONCAT2(x,y) CONCAT1(x,y)


#define DECLARE_POOL_GLOBALS(CLASS, LEVEL_1, LEVEL_2)\
\
    CLASS* CONCAT2(CLASS, _Pool)[LEVEL_1];\
\
    int CONCAT2(CLASS, _Available_Head) = -1;\
    int CONCAT2(CLASS, _Available_Tail) = -1;\
\
    int CONCAT2(CLASS, _In_Use_Head) = -1;\
    int CONCAT2(CLASS, _In_Use_Tail) = -1;\
\
    int CONCAT2(CLASS, _Make_NULL)(void);\
\
    int CONCAT2(CLASS, _New)(int);\
    int CONCAT2(CLASS, _Delete)(int Address);\
\
    int CONCAT2(CLASS, _Increase_Pool)(int Level1_Address);\
\
    CLASS* CONCAT2(CLASS, s)(int Address);\
    CLASS* CONCAT2(CLASS, s_sudo)(int Address);\


//initialise pointers to NULL
#define DECLARE_POOL_MAKE_NULL(CLASS, LEVEL_1, LEVEL_2)\
\
\
    int CONCAT2(CLASS, _Make_NULL)(void) {\
        for (int i = 0; i < LEVEL_1; i++){\
            CONCAT2(CLASS, _Pool)[i] = NULL;\
        }\
    }\
\

//Allocate memory for use in pool, add to tail end of available list.
#define DECLARE_POOL_INCREASE(CLASS, LEVEL_1, LEVEL_2)\
    int CONCAT2(CLASS, _Increase_Pool)(int Level1_Address){\
\
        int i = 0;\
        if(Level1_Address == NEXT_AVAILABLE){\
            while(CONCAT2(CLASS, _Pool)[i] != NULL && i < LEVEL_1) {\
               i++;\
            }\
            if (i == LEVEL_1) {\
                printf("LEVEL_1_POOL_FULL"); fflush(stdout);\
                return (LEVEL_1_POOL_FULL);\
            }\
        } else {\
            i = Level1_Address;\
            if (CONCAT2(CLASS, _Pool)[i] != NULL){\
                return(LEVEL_1_POOL_ALREADY_INITIALISED);\
            }\
        }\
\
        CLASS *j = calloc(LEVEL_2, sizeof(CLASS));\
        if  (j == NULL) return (MALLOC_FAILED);\
        CONCAT2(CLASS, _Pool)[i] = j;\
\
        if (CONCAT2(CLASS, _Available_Head) == -1){\
            assert(CONCAT2(CLASS, _Available_Tail) == -1);\
\
            for (int l = 0; l < LEVEL_2; l++) {\
                CONCAT2(CLASS, _Pool)[i][l].Pool_Self = i * LEVEL_2 + l;\
                CONCAT2(CLASS, _Pool)[i][l].Pool_Prev = i * LEVEL_2 + l - 1;\
                CONCAT2(CLASS, _Pool)[i][l].Pool_Next = i * LEVEL_2 + l + 1;\
                CONCAT2(CLASS, _Pool)[i][l].Pool_In_Use = 0;\
\
            }\
\
            CONCAT2(CLASS, _Pool)[i][0].Pool_Prev = -1;\
            CONCAT2(CLASS, _Pool)[i][LEVEL_2 - 1].Pool_Next = -1;\
\
            CONCAT2(CLASS, _Available_Head) = i * LEVEL_2;\
            CONCAT2(CLASS, _Available_Tail) = (i+1) * LEVEL_2 - 1;\
            \
        } else {\
\
            for (int l = 0; l < LEVEL_2; l++) {\
                CONCAT2(CLASS, _Pool)[i][l].Pool_Self = i * LEVEL_2 + l;\
                CONCAT2(CLASS, _Pool)[i][l].Pool_Prev = i * LEVEL_2 + l - 1;\
                CONCAT2(CLASS, _Pool)[i][l].Pool_Next = i * LEVEL_2 + l + 1;\
                CONCAT2(CLASS, _Pool)[i][l].Pool_In_Use = 0;\
\
            }\
            CONCAT2(CLASS, _Pool)[i][LEVEL_2 - 1].Pool_Next = -1;\
            CONCAT2(CLASS, _Pool)[i][0].Pool_Prev = CONCAT2(CLASS, _Available_Tail);\
            CONCAT2(CLASS, _Available_Tail) = (i+1) * LEVEL_2 -1; \
\
        }\
    }\

//if address not given, find the next available address.
//remove from available list, set in use flag to 1
//////update available/in use head & tail/////TODO
//add to in use list, return address
#define DECLARE_POOL_NEW(CLASS, LEVEL_1, LEVEL_2)\
    int CONCAT2(CLASS, _New)(int Address){\
\
        if(Address == NEXT_AVAILABLE){\
\
            if (CONCAT2(CLASS, _Available_Head) == -1){\
                assert(CONCAT2(CLASS, _Available_Tail) == -1);\
\
                if(CONCAT2(CLASS, _Increase_Pool)\
                (NEXT_AVAILABLE) == LEVEL_1_POOL_FULL){\
\
                    printf("LEVEL_1_POOL(still)_FULL");fflush(stdout);\
                    return LEVEL_1_POOL_FULL;\
                }\
            }\
\
            Address = CONCAT2(CLASS, _Available_Head);\
            CLASS* Object = CONCAT2(CLASS, s_sudo)(Address);\
            CONCAT2(CLASS, _Available_Head) = Object->Pool_Next;\
            if(CONCAT2(CLASS, _Available_Head) == -1){\
                CONCAT2(CLASS, _Available_Tail) = -1;\
            } else {\
                CLASS* Head = CONCAT2(CLASS, s_sudo)(CONCAT2(CLASS, _Available_Head));\
                Head->Pool_Prev = -1;\
            }\
\
            Object->Pool_In_Use = 1;\
\
            if(CONCAT2(CLASS, _In_Use_Head) == -1){\
                assert(CONCAT2(CLASS, _In_Use_Tail) == -1);\
                CONCAT2(CLASS, _In_Use_Head) = Address;\
                CONCAT2(CLASS, _In_Use_Tail) = Address;\
                Object->Pool_Prev = -1;\
                Object->Pool_Next = -1;\
                return Address;\
            }\
\
            CLASS* Tail = CONCAT2(CLASS, s)(CONCAT2(CLASS, _In_Use_Tail));\
            Tail->Pool_Next = Address;\
            Object->Pool_Prev = CONCAT2(CLASS, _In_Use_Tail);\
            Object->Pool_Next = -1;\
            CONCAT2(CLASS, _In_Use_Tail) = Address;\
            return Address;\
        } else {\
DB(0)\
            int i = Address / LEVEL_2;\
            int j = Address % LEVEL_2;\
\
            if (CONCAT2(CLASS, _Pool)[i] == NULL) {\
DB(1)\
                CONCAT2(CLASS, _Increase_Pool)(i);\
            }\
            assert (CONCAT2(CLASS, _Pool)[i][j].Pool_In_Use == 0);\
\
            CLASS* Object = &CONCAT2(CLASS, _Pool)[i][j];\
            Object->Pool_In_Use = 1;\
\
            int Prev_Int = Object->Pool_Prev;\
            int Next_Int = Object->Pool_Next;\
            CLASS* Prev;\
            CLASS* Next;\
            if (Prev_Int == -1 && Next_Int == -1){\
DB(2)\
                \
                CONCAT2(CLASS, _Available_Head) = -1;\
                CONCAT2(CLASS, _Available_Tail) = -1;\
\
            } else if (Prev_Int == -1){\
DB(3)\
printf("\nAvailable Head: %d, Address: %d\n", CONCAT2(CLASS, _Available_Head), Address); fflush(stdout);\
                assert(CONCAT2(CLASS, _Available_Head) == Address);\
                Next = CONCAT2(CLASS, s_sudo)(Next_Int);\
                Next->Pool_Prev = -1;\
                CONCAT2(CLASS, _Available_Head) = Next_Int;\
                \
            } else if (Next_Int == -1){\
DB(4)\
                assert(CONCAT2(CLASS, _Available_Tail) == Address);                \
                Prev = CONCAT2(CLASS, s_sudo)(Prev_Int);\
                Prev->Pool_Next = -1;\
                CONCAT2(CLASS, _Available_Tail) = Prev_Int;\
                \
            } else {\
DB(5)\
\
                Prev =  CONCAT2(CLASS, s_sudo)(Prev_Int);\
                Next =  CONCAT2(CLASS, s_sudo)(Next_Int);\
\
                Prev->Pool_Next = Next_Int;\
                Next->Pool_Prev = Prev_Int;\
\
            }\
\
            if (CONCAT2(CLASS, _In_Use_Head) == -1){\
DB(6)\
                assert(CONCAT2(CLASS, _In_Use_Tail) == -1);\
                CONCAT2(CLASS, _In_Use_Head) = Address;\
                CONCAT2(CLASS, _In_Use_Tail) = Address;\
                Object->Pool_Prev = -1;\
                Object->Pool_Next = -1;\
            } else {\
DB(7)\
                \
                Object->Pool_Next = -1;\
                Object->Pool_Prev = CONCAT2(CLASS, _In_Use_Tail);\
                CLASS* Tail = CONCAT2(CLASS, s)(CONCAT2(CLASS, _In_Use_Tail));\
                Tail->Pool_Next = Address;\
                CONCAT2(CLASS, _In_Use_Tail) = Address;\
            }\
            return Address;\
\
        }\
    }\
\

// remove object from in use list, add object to available list
#define DECLARE_POOL_DELETE(CLASS, LEVEL_1, LEVEL_2)\
    int CONCAT2(CLASS, _Delete)(int Address){\
\
        CLASS* Object = CONCAT2(CLASS, s)(Address);\
        assert(Object->Pool_In_Use == 1);\
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
            assert (CONCAT2(CLASS, _In_Use_Head) == Address);\
            assert (CONCAT2(CLASS, _In_Use_Tail) == Address);\
            CONCAT2(CLASS, _In_Use_Head) = -1;\
            CONCAT2(CLASS, _In_Use_Tail) = -1;\
        } else if (Object->Pool_Prev == -1){\
\
            assert(CONCAT2(CLASS, _In_Use_Head) == Address);\
\
            CONCAT2(CLASS, _In_Use_Head) = Object->Pool_Next;\
\
            CLASS* Next = CONCAT2(CLASS, s)(Object->Pool_Next);\
\
            Next->Pool_Prev = -1;\
\
        } else if (Object->Pool_Next == -1){\
\
            assert(CONCAT2(CLASS, _In_Use_Tail) == Address);\
\
            CONCAT2(CLASS, _In_Use_Tail) = Object->Pool_Prev;\
\
            CLASS* Prev = CONCAT2(CLASS, s)(Object->Pool_Prev);\
\
            Prev->Pool_Next = -1;\
\
        } else {\
\
            CLASS* Prev = CONCAT2(CLASS, s)(Object->Pool_Prev);\
            CLASS* Next = CONCAT2(CLASS, s)(Object->Pool_Next);\
\
            Prev->Pool_Next = Object->Pool_Next;\
            Next->Pool_Prev = Object->Pool_Prev;\
        }\
\
        Object->Pool_In_Use = 0;\
\
        if (CONCAT2(CLASS, _Available_Head) == -1){\
\
            assert(CONCAT2(CLASS, _Available_Tail) == -1);\
\
            CONCAT2(CLASS, _Available_Head) = Address;\
            CONCAT2(CLASS, _Available_Tail) = Address;\
\
            Object->Pool_Prev = -1;\
            Object->Pool_Next = -1;\
\
        } else {\
            Object->Pool_Prev = -1;\
            Object->Pool_Next = CONCAT2(CLASS, _Available_Head);\
            CLASS* Head = CONCAT2(CLASS, s_sudo)(CONCAT2(CLASS, _Available_Head));\
            Head->Pool_Prev = Address;\
            CONCAT2(CLASS, _Available_Head) = Address;\
        }\
    }\

//Return a pointer to object at integer address in pool
#define DECLARE_POOL_LOOKUP(CLASS, LEVEL_1, LEVEL_2)\
    CLASS* CONCAT2(CLASS, s)(int Address){\
        assert(Address >= 0);\
        int i = Address / LEVEL_2;\
        int j = Address % LEVEL_2;\
\
        if (CONCAT2(CLASS, _Pool)[i] == NULL){\
            return((long)LEVEL_1_POOL_NOT_INITIALISED);\
        }\
\
        if (CONCAT2(CLASS, _Pool)[i][j].Pool_In_Use == 0){\
            return((long)OBJECT_NOT_IN_USE);\
        }\
\
        return (&(CONCAT2(CLASS, _Pool)[i][j]));\
    }\
\
    CLASS* CONCAT2(CLASS, s_sudo)(int Address){\
        assert(Address >= 0);\
\
        int i = Address / LEVEL_2;\
        int j = Address % LEVEL_2;\
\
        if (CONCAT2(CLASS, _Pool)[i] == NULL){\
            return((long)LEVEL_1_POOL_NOT_INITIALISED);\
        }\
\
        return (&(CONCAT2(CLASS, _Pool)[i][j]));\
    }\

//dealocate all memory used in the second level of the pool
#define DECLARE_POOL_FREE_ALL(CLASS, LEVEL_1, LEVEL_2)\
\
    int CONCAT2(CLASS, _Free_All)(void) {\
        for (int i = 0; i < LEVEL_1; i++){\
            if (CONCAT2(CLASS, _Pool)[i] != NULL){\
                free(CONCAT2(CLASS, _Pool)[i]);\
                CONCAT2(CLASS, _Pool)[i] = NULL;\
            }\
        }\
        \
        CONCAT2(CLASS, _Available_Head) = -1;\
        CONCAT2(CLASS, _Available_Tail) = -1;\
\
        CONCAT2(CLASS, _In_Use_Head) = -1;\
        CONCAT2(CLASS, _In_Use_Tail) = -1;\
    }\
\
\
    
//declare all of the globals and functions used by the pool
#define DECLARE_POOL(CLASS, LEVEL_1, LEVEL_2)\
\
    DECLARE_POOL_GLOBALS   (CLASS, LEVEL_1, LEVEL_2)\
    DECLARE_POOL_MAKE_NULL (CLASS, LEVEL_1, LEVEL_2)\
    DECLARE_POOL_INCREASE  (CLASS, LEVEL_1, LEVEL_2)\
    DECLARE_POOL_NEW       (CLASS, LEVEL_1, LEVEL_2)\
    DECLARE_POOL_DELETE    (CLASS, LEVEL_1, LEVEL_2)\
    DECLARE_POOL_LOOKUP    (CLASS, LEVEL_1, LEVEL_2)\
    DECLARE_POOL_FREE_ALL  (CLASS, LEVEL_1, LEVEL_2)\

