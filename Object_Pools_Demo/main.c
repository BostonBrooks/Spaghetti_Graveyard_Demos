// gcc main.c object.c

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"




int main (void) {

    char Name[16];
    int Score;

    Restore_All();

    while(1) {

        Print_All();

        int input;

        printf("Continiue? y/n:");
        do{
        input = getchar();

        } while (input != 'y' && input != 'n');

        if (input == 'n'){
            Save_All();
            Free_All();
            exit(0);
        }                

        printf("Enter Name: ");
        scanf("%15s", Name);

        printf("Enter Score: ");
        scanf("%d", &Score);


        int object_int = New_Object();

        Set_Object_Name(object_int, Name);
        Set_Object_Score(object_int, Score);




        Sort_Object(object_int);

    }


}
