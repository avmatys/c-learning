#include <stdio.h>

#define swap(T, a, b) \
    T tmp = a; \
    a = b; \
    b = tmp; 

int main()
{
    int a = 10;
    int b = 20;
    swap(int, a, b);
    printf("a = %d, b = %d\n", a, b);
    return 0;
}
