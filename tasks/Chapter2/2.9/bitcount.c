#include <stdio.h>

unsigned bitcount(unsigned x) 
{
    int b = 0;
    for (b = 0; x > 0; x &= (x - 1))   
        b++;
    return b;
}

int main()
{
    printf("Number: %u, Bit count: %u \n", 5, bitcount(5));
    printf("Number: %u, Bit count: %u \n", 0, bitcount(0));
    printf("Number: %u, Bit count: %u \n", 16, bitcount(16));
    printf("Number: %u, Bit count: %u \n", 2147483647, bitcount(2147483647));
    return 1;
}
