#define CIRCLES



#include <SFML/System.h>
#include <SFML/Window.h>
#include <SFML/Graphics.h>


typedef struct {

    int Pool_Self;
    int Pool_Prev;
    int Pool_Next;
    int Pool_In_Use;

    int List_Next;
    int List_Prev;
    int x;
    int y;

} Circle;

int Circles_Head = -1;
int Circles_Tail = -1;

sfRenderWindow* window;
sfCircleShape* circle;


DECLARE_POOL(Circle, 1028, 1028);
DECLARE_LIST_OPS(Circle, List);


int Init_Circles (void){

    Circle_Make_NULL();
    sfVideoMode mode = {800, 600, 32};

    window = sfRenderWindow_create(mode, "Undo-Redo", sfResize | sfClose, NULL);
    circle = sfCircleShape_create();

    sfVector2f vect2f;

    vect2f.x = 5;
    vect2f.y = 5;    
    sfCircleShape_setOrigin(circle, vect2f);

    sfCircleShape_setFillColor(circle, sfBlack);

    sfCircleShape_setRadius(circle, 5);

    sfRenderWindow_clear(window, sfWhite);

    sfRenderWindow_display(window);


}

int Create_Circle(int x, int y, int Address){

    int New_Circle_Int = Circle_New(Address);



    Circle* New_Circle = Circles(New_Circle_Int);
    New_Circle->x = x;
    New_Circle->y = y;

    return Circle_List_Push_Tail(
        &Circles_Head,
        &Circles_Tail,
        New_Circle_Int
    );
}

int Delete_Circle(int Circle_Int){

    Circle_List_Remove(&Circles_Head, &Circles_Tail, Circle_Int);

    Circle_Delete(Circle_Int);

    return Circle_Int;
}

int Find_Nearest_Circle(int x, int y){

    int Circle_Int = Circles_Head;

    int best_squared_distance = 100;
    int best_Circle_Int = -1;
    int x_distance;
    int y_distance;
    int squared_distance;

    while (Circle_Int != -1){

        Circle* Circle_Address = Circles(Circle_Int);

        x_distance = x - Circle_Address->x;
        y_distance = y - Circle_Address->y;

        squared_distance = x_distance * x_distance + y_distance * y_distance;

        if (squared_distance < best_squared_distance){

            best_Circle_Int = Circle_Int;
            best_squared_distance = squared_distance;
        }
    
        Circle_Int = Circle_Address->List_Next;

    }
    
    return best_Circle_Int;


}

int Create_Circles (void){

   int i;
   for (i = 0; i < 64; i++){

       Create_Circle(rand() % 800, rand() % 600, NEXT_AVAILABLE);

   }
}

int Draw_Circles(){


   if (Circles_Head == -1){
       assert(Circles_Tail == -1);
       sfRenderWindow_clear(window, sfWhite);
       sfRenderWindow_display(window);
       return 1;
   }

   int Current_Circle_Int = Circles_Head;
   Circle* Current_Circle;
   sfVector2f position;

   sfRenderWindow_clear(window, sfWhite);

   while(Current_Circle_Int != -1){
       Current_Circle = Circles(Current_Circle_Int);
       position.x = Current_Circle->x;
       position.y = Current_Circle->y;
       sfCircleShape_setPosition(circle, position);
       sfRenderWindow_drawCircleShape(window, circle, NULL);
       Current_Circle_Int = Current_Circle->List_Next;
   }

   sfRenderWindow_display(window);

   return 1;
}


