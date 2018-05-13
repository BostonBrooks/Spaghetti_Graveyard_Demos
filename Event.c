    // ---- External Includes ---- //


#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>



    // ---- Data Structures ---- //

struct Evt;

typedef int (event_function_ptr)(struct Evt*);

typedef enum
{
    timed_event,
    tick_event,
    spontaneous_event,
} Event_Type;


typedef struct Evt
{
    Event_Type type;
    struct Evt* next;
    long time;             
//execute timed_event when now_time is greater than time,
    event_function_ptr* function;
    //function args.
    int arg;
} Event;


    // ---- Function Headers ---- //
    // ---- Environment ---- //

int update_time (void);

    // ---- Event ---- //

int register_timed_event (Event*);
int reregister_timed_event (Event*);
int deregister_timed_event  (Event*);

int register_tick_event (Event*);
int reregister_tick_event (Event*);
int deregister_tick_event (Event*);

int register_spontaneous_event (Event*);
int reregister_spontaneous_event (Event*);
int deregister_spontaneous_event (Event*);

int register_event (Event*);
int reregister_event (Event*);
int deregister_event  (Event*);

int check_spontaneous_events(void);
int check_timed_events (void);
int check_tick_events (void);

int UpdateEvents(void);

    // ---- Constants ---- //
    // ---- Event ---- //

#define REREGISTER    (1)
#define DEREGISTER    (0)

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

#define TIMEDCONVERTSTOSPONTANEOUS TRUE //????

    // ---- Globals ---- //
    // ---- Environment ---- //

long time_milliseconds;

    // ---- Event ---- //

Event tick_event_head;
Event timed_event_head;
Event spontaneous_event_head;

Event* tick_event_tail;  //Should we use *?
Event* timed_event_tail;
Event* spontaneous_event_tail;

Event* tick_stopper;

    // ---- Functions ---- //
    // ---- Environment ---- //


int update_time (void) {
    struct timeval time_val;
    gettimeofday(&time_val, NULL);
    time_milliseconds = time_val.tv_usec / 1000 + (long) time_val.tv_sec * 1000;
}

int register_timed_event (Event* p)
{
//Takes an Event structure and adds it to the timed event list,
// use register_event
//updates  timed_event_tail if necessary
    Event* q = &timed_event_head;
    while (q->next != NULL && q->next->time <= p->time)
    {
        q = q->next;
    }
    p->next = q->next;
    q->next = p;
    if (p->next == NULL)
    {
        timed_event_tail = p;
    }

}
int reregister_timed_event (Event* p)
{
//use reregister_event.
//Takes an Event in the timed event list, which has been modified, 
 //and re-inserts it into the into the list.
//Note that the argument is a pointer to the previous entry.
    Event* q = p->next;
    p->next = p->next->next;
    if (p->next == NULL)
    {
        timed_event_tail = p;  //update tail if necessary
    }
    register_event(q);
}

int deregister_timed_event  (Event* p)
{
// use deregister_event
// remove event from current location in one of the lists
// free() the structure.
//Note that the argument is a pointer to the previous entry.
    Event* q = p->next;
    p->next = p->next->next;
    if (p->next == NULL)
    {
        timed_event_tail = p;
    }
    free(q);
}


int register_tick_event (Event* p)
{
//Takes an Event structure and adds it to the on-tick event list
//use register_event
//updates  tick_event_tail if necessary
//If a tick is registered after the line "tick_stopper = tick_event_tail;"
 //in "UpdateEvents()", the execution of the event will be deferred 
 //until the following clock tick
    tick_event_tail->next = p;
    p->next = NULL;
    tick_event_tail = p;


}

int reregister_tick_event (Event* p)
{
//use reregister_event
//Takes tick event and moves it to the end of the tick event list.
//Note that the argument is a pointer to the previous entry
    Event* q = p->next;
    p->next = p->next->next;
    if (p->next == NULL)
    {
        tick_event_tail = p;
    }
    register_event(q);
}


int deregister_tick_event (Event* p)
{
//use deregister_event
// remove event from current location in one of the lists
// free() the structure.
//Note that the argument is a pointer to the previous entry.
    Event* q = p->next;
    p->next = p->next->next;
    if (p->next == NULL)
    {
        tick_event_tail = p; //update tail if necessary
    }
    free(q);
}


int register_spontaneous_event (Event* p)
{
//Takes an Event structure and adds it to the spontaneous event list
//use register_event
    spontaneous_event_tail->next = p;
    p->next = NULL;
    spontaneous_event_tail = p;
}

int reregister_spontaneous_event (Event* p)
{
//use reregister_event
//Takes an Event in the spontaneous event list, which has been modified, 
 //and re-inserts it to the end of the list.
//Note that the argument is a pointer to the previous entry.
    Event* q = p->next;
    p->next = p->next->next;
    if (p->next == NULL)
    {
        spontaneous_event_tail = p; //update tail if necessary
    }
    register_event(q);
}

int deregister_spontaneous_event (Event* p)
{
//use deregister_event
// remove event from current location in one of the lists
// free() the structure.
//Note that the argument is a pointer to the previous entry.
    Event* q = p->next;
    p->next = p->next->next;
    if (p->next == NULL)
    {
        spontaneous_event_tail = p;   //update tail if necessary
    }
    free(q);
}



int register_event (Event* p)
{
//if event type is tick event, add to the end of the tick event list
//if event type is spontaneous, add to the end of spontaneous list
//if event type is timed but the time variable is less than now_time
 //the event will be executed immediately after the current event.
//if event type is timed and the time variable is greater than now_time,
 //add to the timed event list

    if (p->type == tick_event)
    {
        register_tick_event (p);
    }
    else if (p->type == spontaneous_event)
    {
        register_spontaneous_event (p);
    }
    else if (p->type == timed_event)
    {
#if TIMEDCONVERTSTOSPONTANEOUS == TRUE
        
        if (p->time <= time_milliseconds) {
            p->type = spontaneous_event;
//function will have to check/re-set its own event type if this may happen.
            
            register_spontaneous_event (p);
//if (p->time <= time_milliseconds) the event will be added to the spontaneous
 //list, and executed immediately
        } else {
            register_timed_event (p);
        }   
        
#else
        
        register_timed_event (p);
        
#endif
    }
}

int reregister_event (Event* p)
{
// remove event from current location in one of the lists
// Add to new location in one of the lists
// Note that the argument is a pointer to the previous entry.
    Event* q = p->next;
    p->next = p->next->next;
    if (p->next == NULL)    //update tail if necessary
    {
        if (p->type == tick_event)
        {
            tick_event_tail = p;
        }
        else if (p->type == timed_event)
        {
            timed_event_tail = p;
        }
        else if  (p->type == spontaneous_event)
        {
            spontaneous_event_tail = p;
        }
    }
    register_event(q);
//Note that the argument is not a pointer to the previous entry.

}

int deregister_event  (Event* p)
{
// remove event from current location in one of the lists
// free() the structure.
//Note that the argument is a pointer to the previous entry.
    Event* q = p->next;
    p->next = p->next->next;
    if (p->next == NULL)    //update tail if necessary
    {
        if (p->type == tick_event)
        {
            tick_event_tail = p;
        }
        else if (p->type == timed_event)
        {
            timed_event_tail = p;
        }
        else if  (p->type == spontaneous_event)
        {
            spontaneous_event_tail = p;
        }
    }
    free(q);
}


int check_spontaneous_events(void)
{
    while (spontaneous_event_head.next != NULL)
    {
        int flag = spontaneous_event_head.next->function(spontaneous_event_head.next);
        if (flag == DEREGISTER)
        {
            deregister_event(&spontaneous_event_head);
//Note that the argument is a pointer to the previous entry.
        }
        else if (flag == REREGISTER)
        {
            reregister_event(&spontaneous_event_head);
//Note that the argument is a pointer to the previous entry.
        }
        else
        {
            printf("bad flag in check_spontaneous_events");
        }
    }
}

int check_timed_events (void)
{
    while (
        timed_event_head.next != NULL 
        && timed_event_head.next->time <= time_milliseconds
    )
    {
        int flag = timed_event_head.next->function(timed_event_head.next);
        if (flag == DEREGISTER)
        {
            deregister_event(&timed_event_head);
//Note that the argument is a pointer to the previous entry.
        }
        else if (flag == REREGISTER)
        {
            reregister_event(&timed_event_head);
//Note that the argument is a pointer to the previous entry.
        }
        else
        {
            printf("bad flag in check_timed_events");
        }
        check_spontaneous_events();
    }
}

int check_tick_events (void)
{
    //We put a stopper on end of tick_event list,
    //so that new tick events created during execution of the rest of the
    // code will not execute until the following clock tick.

    Event* p = &tick_event_head;
    Event* q;
    while (p->next != NULL &&  p != tick_stopper)
    {
        int flag = p->next->function(p->next);
        if (flag == DEREGISTER)
        {
            q = p->next;
            deregister_event(p); 
//Note that the argument is a pointer to the previous entry.
            p = q;
        }
        else if (flag == REREGISTER)
        {
            q = p->next;
            if (p->next->type != tick_event)
            {
                reregister_event(p);  
//Note that the argument is a pointer to the previous entry.
            }
            p = q; 
//if type is tick_event, 
 //leave the event in place in the tick_event list and go on to the next entry.
        }
        else
        {
            printf("bad flag in check_tick_events");
        }
        check_spontaneous_events();   //Do we want this?
    }
    tick_stopper = NULL;
}

int UpdateEvents(void)
{
    tick_stopper = tick_event_tail;
    check_spontaneous_events(); 
//PS check_spontaneous_events() is called after each event function call
    check_timed_events();
    check_tick_events();
}

int SetupEvents(void)
{
    tick_event_head.type = tick_event;
    timed_event_head.type = timed_event;
    spontaneous_event_head.type = spontaneous_event;
    
    tick_event_head.next = NULL;
    timed_event_head.next = NULL;
    spontaneous_event_head.next = NULL;
    
    tick_event_tail = &tick_event_head;
    timed_event_tail = &timed_event_head;
    spontaneous_event_tail = &spontaneous_event_head;
}


    // ---- Test Functions ---- //

int foo (Event* p) {
    printf("timed_event\n");
    p->time = p->time + 500; 
    p->type = timed_event;
    return REREGISTER;
}


int bar (Event* p) {
    printf("spontaneous_event\n");
    return DEREGISTER;
}

int baz (Event* p) {
    printf("tick_event\n");

    Event* q;
    q = malloc(sizeof(Event));   
    q->type = spontaneous_event;
    q->function = &bar;
    register_event(q);

    return REREGISTER;    
}


    // ---- Main ---- // 

int main (void) {

    SetupEvents();
    
    update_time();

    Event* p;
int i;
for (i=0; i<5; i++){    
    p = malloc(sizeof(Event));
    p->time = time_milliseconds+1000;    
    p->type = timed_event;
    p->function = &foo;
    register_event(p);
    
    p = malloc(sizeof(Event));  
    p->type = tick_event;
    p->function = &baz;
    register_event(p);
    
}    
    while (1) {
        update_time();
        UpdateEvents();
        printf("Run other code\n");
        sleep(1);
    }
}



 
