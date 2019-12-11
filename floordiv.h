#define FLOORDIV

int floordiv (int a, int b){

    if (b == 0) {
        return 0;
    }

    int c = a / b;

    if (b % a != 0){
        if ((a < 0 && b > 0) || (a > 0 && b < 0)){

            c = c - 1;
        }
    }

    return c;
}

