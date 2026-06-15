#include <stdio.h>


void itoa(int n, char *buffer)
{
    if (n < 0) {
        *buffer++ = '-';
        itoa(-n, buffer);
        return;
    }
    char c = '0' + n % 10;
    if (n / 10)
        itoa(n / 10, buffer);
    while (n /= 10) buffer++;
    *buffer++ = c;
    *buffer = '\0';
}

int main()
{
    char buffer[32];
    int x = -1234;
    itoa(x, buffer);
    printf("Number: %d String: %s\n", x, buffer); 
    return 0;
}
