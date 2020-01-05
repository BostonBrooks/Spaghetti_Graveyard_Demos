
#define FLOORDIV

int floordiv (int a, int b){

    int c = a / b;

    if (a % b == 0) return c;

    if ((a < 0 && b > 0) || (a > 0 && b < 0)) return c - 1;

    return c;
}
