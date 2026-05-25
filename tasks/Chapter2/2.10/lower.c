#include <stdio.h>

int lower(int c) 
{
    return ('A' <= c && c <= 'Z') ? c - 'A' + 'a' : c;
}

int main() 
{
    printf("Upper: %c, Lower: %c\n", 'A', lower('A'));
    printf("Upper: %c, Lower: %c\n", '9', lower('9'));
    printf("Upper: %c, Lower: %c\n", 'a', lower('a'));
    printf("Upper: %c, Lower: %c\n", '-', lower('-'));
    return 1;
}
