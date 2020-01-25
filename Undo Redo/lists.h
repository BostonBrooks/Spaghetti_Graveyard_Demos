#define LISTS
    
    
#ifndef POOLS
#include "pools.h"
#endif
    
    

#define DECLARE_LIST_OPS(CLASS, LIST)\
\
    int CLASS##_##LIST##_Push_Head(int* Head, int* Tail, int Object_Int){\
\
        CLASS* Object = CLASS##s(Object_Int);\
        assert (Object->Pool_In_Use == 1);\
\
        if (*Head == -1){\
            assert(*Tail == -1);\
            *Head = Object_Int;\
            *Tail = Object_Int;\
            Object->LIST##_Prev = -1;\
            Object->LIST##_Next = -1;\
            return Object_Int;\
\
        }\
\
        CLASS* Original_Head = CLASS##s(*Head);\
        Object->LIST##_Prev = -1;\
        Object->LIST##_Next = *Head;\
        Original_Head->LIST##_Prev = Object_Int;\
        *Head = Object_Int;\
        return Object_Int;\
    }\
\
    int CLASS##_##LIST##_Push_Tail(int* Head, int* Tail, int Object_Int){\
\
        CLASS* Object = CLASS##s(Object_Int);\
        assert (Object->Pool_In_Use == 1);\
\
        if (*Tail == -1){\
            assert(*Head == -1);\
            *Head = Object_Int;\
            *Tail = Object_Int;\
            Object->LIST##_Prev = -1;\
            Object->LIST##_Next = -1;\
            return Object_Int;\
\
        }\
\
        CLASS* Original_Tail = CLASS##s(*Tail);\
        Object->LIST##_Next = -1;\
        Object->LIST##_Prev = *Tail;\
        Original_Tail->LIST##_Next = Object_Int;\
        *Tail = Object_Int;\
        return Object_Int;\
    }\
\
    int CLASS##_##LIST##_Remove(int* Head, int* Tail, int Object_Int){\
\
        CLASS* Object = CLASS##s(Object_Int);\
        assert (Object->Pool_In_Use == 1);\
\
        int Prev_Int = Object->LIST##_Prev;\
        int Next_Int = Object->LIST##_Next;\
\
        CLASS* Object_Next;\
        CLASS* Object_Prev;\
\
        if (Prev_Int == -1 && Next_Int == -1){\
            assert(*Head == Object_Int && *Tail == Object_Int);\
            *Head = -1;\
            *Tail = -1;\
            Object->LIST##_Prev = 0;\
            Object->LIST##_Next = 0;\
            return Object_Int;\
        }\
\
        if (Prev_Int == -1) {\
            assert(*Head == Object_Int);\
\
            Object_Next = CLASS##s(Next_Int);\
            Object_Next->LIST##_Prev = -1;\
            *Head = Next_Int;\
\
            Object->LIST##_Prev = -1;\
            Object->LIST##_Next = -1;\
\
            return Object_Int;\
        }\
\
        if (Next_Int == -1) {\
            assert(*Tail == Object_Int);\
\
            Object_Prev = CLASS##s(Prev_Int);\
            Object_Prev->LIST##_Next = -1;\
            *Tail = Prev_Int;\
\
            Object->LIST##_Prev = -1;\
            Object->LIST##_Next = -1;\
\
            return Object_Int;\
        }\
\
        Object_Next = CLASS##s(Next_Int);\
        Object_Prev = CLASS##s(Prev_Int);\
\
        Object_Prev->LIST##_Next = Next_Int;\
        Object_Next->LIST##_Prev = Prev_Int;\
\
        Object->LIST##_Prev = -1;\
        Object->LIST##_Next = -1;\
        return Object_Int;\
\
    }\
\
    int CLASS##_##LIST##_Pop_Head(int* Head, int* Tail){\
\
        if (*Head == -1){\
            assert (*Tail == -1);\
            return -1;\
        }\
\
        int Object_Int = *Head;\
        CLASS* Object = CLASS##s(Object_Int);\
        *Head = Object->LIST##_Next;\
        if (*Head == -1) *Tail = -1;\
        return Object_Int;\
    }\
\
\
    int CLASS##_##LIST##_Pop_Tail(int* Head, int* Tail){\
\
        if (*Tail == -1){\
            assert (*Head == -1);\
            return -1;\
        }\
\
        int Object_Int = *Tail;\
        CLASS* Object = CLASS##s(Object_Int);\
        *Tail = Object->LIST##_Prev;\
        if (*Tail == -1) *Head = -1;\
        return Object_Int;\
    }\
