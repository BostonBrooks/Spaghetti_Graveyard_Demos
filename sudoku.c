#include <stdio.h>
#include <stdlib.h>

int game[81]

    = {0, 0, 0, 2, 0, 4, 0, 0, 0,
       0, 0, 3, 0, 9, 0, 2, 0, 0,
       0, 9, 6, 0, 0, 0, 3, 8, 0,
       7, 0, 0, 5, 0, 9, 0, 0, 3,
       0, 1, 0, 0, 0, 0, 0, 7, 0,
       5, 0, 0, 4, 0, 7, 0, 0, 1,
       0, 2, 9, 0, 0, 0, 7, 3, 0,
       0, 0, 5, 0, 2, 0, 8, 0, 0,
       0, 0, 0, 6, 0, 5, 0, 0, 0};
   
#define SUCCESS 1
#define FAIL    0

int check(int, int);

int try(int, int);    

int main(void){

    int i, j, flag;
    int cell = 0;

    for (i = 1; i <= 9; i++){
        flag = try(cell, i);
        if (flag == SUCCESS) break;
    }

    if (flag == FAIL) {

        printf("No Solution");

    } else {


        for (i = 0; i < 9; i++) {

            for (j = 0; j < 9; j++){

                printf(" %d", game[i * 9 + j]);
            }
        printf("\n");
        }

    }

    while(1){

        if (getchar() == 'e') exit(1);
    }

}

int try(int cell, int number){


    int flag = check(cell, number);

    if (flag == FAIL) return FAIL;

    game[cell] = number;

    cell = cell + 1;

    if (cell == 81) return SUCCESS;

    int num = game[cell];

    if (num != 0) return try (cell, num);

    int i;
    for (i = 1; i <= 9; i++){
        flag = try(cell, i);
        if (flag == SUCCESS) return SUCCESS;
    }
    game[cell] = 0;

    return(FAIL);

    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

int check (int cell, int number){

    int row = cell / 9;
    int column = cell % 9;
    int square_row = row / 3;
    int square_column = column / 3;

    int target_row;
    int target_column;
    int target_cell;
    int target_number;



//check vertical

    target_column = column;
    
    for (target_row = 0; target_row < 9; target_row++){

        target_cell = target_column + 9 * target_row;

        target_number = game[target_cell];

        if (target_number == number && target_cell != cell){


            return FAIL;

        }
    }

//check horizontal

    target_row = row;
    
    for (target_column = 0; target_column < 9; target_column++){

        target_cell = target_column + 9 * target_row;

        target_number = game[target_cell];

        if (target_number == number && target_cell != cell){


            return FAIL;

        }
    }

//check square

    for (target_row = square_row * 3;
        target_row < square_row * 3 + 3;
        target_row++){

        for (target_column = square_column * 3;
            target_column < square_column * 3 + 3;
            target_column++){

            target_cell = target_column + 9 * target_row;

            target_number = game[target_cell];

            if (target_number == number && target_cell != cell){

              return FAIL;

            }

        }
    }

    return SUCCESS;

}
