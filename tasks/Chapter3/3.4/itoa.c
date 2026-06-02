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

void itoa(int n, char s[]) 
{
    bool neg = n < 0;
    int i = 0;
    do {
        s[i++] = abs(n % 10) + '0';        
    } while((n = n / 10) != 0);
    if (neg) 
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

int main() 
{
    char buf[1000];
    int n = 100;
    itoa(n, buf);
    printf("Int: %d String: %s\n", n, buf);
    n = -100;
    itoa(n, buf);
    printf("Int: %d String: %s\n", n, buf);
    n = INT_MIN;
    itoa(n, buf);
    printf("Int: %d String: %s\n", n, buf);
    n = INT_MAX;
    itoa(n, buf);
    printf("Int: %d String: %s\n", n, buf);
    return 0;
}
