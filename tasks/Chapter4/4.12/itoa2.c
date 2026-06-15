#include <stdio.h>


void itoa(int n, char **buffer)
{
    if (n < 0) {
        *(*buffer)++ = '-';
        itoa(-n, buffer);
        return;
    }
    if (n / 10)
        itoa(n / 10, buffer);
    char c = '0' + n % 10;
    *(*buffer)++ = c;
    **buffer = '\0';
}

int main()
{
    char buffer[32];
    char *ptr = buffer;
    int x = -1234;
    itoa(x, &ptr);
    printf("Number: %d String: %s\n", x, buffer); 
    return 0;
}
