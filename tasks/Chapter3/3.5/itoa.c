#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void reverse(char s[]) 
{
    int n = strlen(s);
    int tmp;
    for (int i = 0, j = n - 1; i < j; i++, j--) {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }
}

void itoa(int n, char s[], int b) 
{
    int dig, i;
    bool neg;

    neg = n < 0;
    i = 0;
    do {
        dig = abs(n % b);
        dig += (dig < 10) ? '0' : 'A' - '0';
        s[i++] = dig;        
    } while((n = n / b) != 0);
    if (neg) 
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

int main() 
{
    char buf[1000];
    int n = 100;
    int b = 10;
    itoa(n, buf, b);
    printf("Int: %d Base: %d String: %s\n", n, b, buf);
    b = 8;
    itoa(n, buf, b);
    printf("Int: %d Base: %d String: %s\n", n, b, buf);
    b = 16;
    itoa(n, buf, b);
    printf("Int: %d Base: %d String: %s\n", n, b, buf);
    return 0;
}
