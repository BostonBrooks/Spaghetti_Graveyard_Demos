#define BBMESSAGE_PASSING


//-----------------------------INCLUDES----------------------------//

#ifndef CONSTANTS
#define CONSTANTS
#include "../headers/constants.h"
#endif

#ifndef BBMESSAGE
#define BBMESSAGE
#include "../headers/bbMessage.h"
#endif

#ifndef BBMESSAGE_FUNCTIONS
#define BBMESSAGE_FUNCTIONS
#include "../headers/bbMessage_functions.h"
#endif

//-----------------------------GLOBALS----------------------------//

#define DELETE_MESSAGE    0
#define REUSE_MESSAGE     1

//-----------------------------CODE----------------------------//

extern int Current_Time; //TODO put in a new file. keep synched with other players

int print_message_details(int message_int){
        bbMessage* message = bbMessage_Pool_Lookup(message_int);
        
        printf("prev %d, self %d, next %d, time = %d, type = %d\n",
            message->Sorted_Prev,
            message->Pool_Self,
            message->Sorted_Next,
            message->Message_Time,
            message->type
        );
            

    
}

int print_message_details_ALL(){

    
    if (bbMessage_head == -1){
        assert(bbMessage_tail == -1);
        printf("message list empty");
    }
        
    int list_element = bbMessage_head;
    
    int i = 0;
    
    while (list_element != -1 && i<64){
    
        print_message_details(list_element);
        bbMessage* message = bbMessage_Pool_Lookup(list_element);
        list_element = message->Sorted_Next;
        i++;
        
    
    }

}


int bbMessage_submit(int message_int){ //copy and paste from bbDrawable_addtoTS


    bbMessage* message = bbMessage_Pool_Lookup(message_int);
//add to sorted list of messages
    if (bbMessage_head == -1){
        assert(bbMessage_tail == -1);
        
        bbMessage* message = bbMessage_Pool_Lookup(message_int);
        message->Sorted_Prev = -1;
        message->Sorted_Next = -1;
        bbMessage_head = message_int;
        bbMessage_tail = message_int;
    
        
        return 0;
    }
    
    
    //loop through list until list_element is after message
    // insert message before list element
       //if we get a -1 in the list, insert message at end of list.
    bbMessage* list_element = bbMessage_Pool_Lookup(bbMessage_head);
    
    
    while(bbMessage_isAfter(list_element, message)!=1){
    
   // print_message_details(list_element->Pool_Self);
    
    int list_element_int = list_element->Sorted_Next;
        if (list_element_int == -1){
    
            list_element->Sorted_Next = message->Pool_Self;
            message->Sorted_Prev = list_element->Pool_Self;
            message->Sorted_Next = -1;
            bbMessage_tail = message->Pool_Self;
            
            return 0;
        
        }
    
    list_element = bbMessage_Pool_Lookup(list_element_int);
    }
    
    int prev_int = list_element->Sorted_Prev;
    bbMessage* prev = bbMessage_Pool_Lookup(prev_int);
    prev->Sorted_Next = message_int;
    list_element->Sorted_Prev = message_int;
    message->Sorted_Prev = prev_int;
    message->Sorted_Next = list_element->Pool_Self;
}


int bbMessage_remove(int message_int){
//remove from sorted list of messages
    bbMessage* message = bbMessage_Pool_Lookup(message_int);
    
    if (message->Sorted_Prev == -1 && message->Sorted_Next == -1){
    
        bbMessage_head = -1;
        bbMessage_tail = -1;
    } else if (message->Sorted_Prev == -1){
        
        bbMessage_head = message->Sorted_Next;
        bbMessage* message_next = bbMessage_Pool_Lookup(bbMessage_head);
        message_next->Sorted_Prev = -1;
    } else if (message->Sorted_Next == -1){
        
        bbMessage_tail = message->Sorted_Prev;
        bbMessage* message_prev = bbMessage_Pool_Lookup(bbMessage_tail);
        message_prev->Sorted_Next = -1;
        
    } else {
        int prev_int = message->Sorted_Prev;
        int next_int = message->Sorted_Next;
        
        bbMessage* message_prev = bbMessage_Pool_Lookup(prev_int);
        bbMessage* message_next = bbMessage_Pool_Lookup(next_int);
        
        message_prev->Sorted_Next = next_int;
        message_next->Sorted_Prev = prev_int;
        
    }
}

int bbMessage_process(int message_int){

//remove from list
    bbMessage_remove(message_int);
    
    bbMessage* message = bbMessage_Pool_Lookup(message_int);
    int type = message->type;
    
//execute message function and receive flag
    int flag = bbMessage_type_vtable[type](message);
    //cannot use message->member after the above step
    
//if flag is REUSE_MESSAGE, bbMessage_submit(bbMessage_int);
    if (flag == REUSE_MESSAGE) {
        bbMessage_submit(message_int);

//if flag is DELETE_MESSAGE, remove from pool    
    } else {
        assert(flag == DELETE_MESSAGE);
        bbMessage_Pool_Delete(message_int);
    }
    
    return flag;


}

int bbMessage_processAll(){

   //bbMessage_process() all messages up to the global variable long long int Current_Time;
   //be carefull about knowing what to do next because 
   //bbMessage_process() contains bbMessage_remove(int message_int)

   if (bbMessage_head == -1){
       assert (bbMessage_tail ==-1);
       return 0;
   
   }
   
   bbMessage* message = bbMessage_Pool_Lookup(bbMessage_head);
   long long int message_time = message->Message_Time;
   //continue to process the beginning of the list until ...
   while (message_time <= Current_Time){
       bbMessage_process(bbMessage_head);
       if (bbMessage_head == -1){
           assert (bbMessage_tail ==-1);
           return 0;
       
       } 
       message = bbMessage_Pool_Lookup(bbMessage_head);
       message_time = message->Message_Time;
   }
return 0;
}
