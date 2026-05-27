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

void itoa(int n, char s[], int w) 
{
    bool neg = n < 0;
    int i = 0;
    do {
        s[i++] = abs(n % 10) + '0';        
    } while((n = n / 10) != 0);
    if (neg) 
        s[i++] = '-';
    while(i < w) 
        s[i++] = ' ';
    s[i] = '\0';
    reverse(s);
}

int main() 
{
    char buf[1000];
    int n, w; 

    w = 10;
    n = 100;
    itoa(n, buf, w);
    printf("Int: %d String: %s\n", n, buf);
    w = 20;
    itoa(n, buf, w);
    printf("Int: %d String: %s\n", n, buf);
    w = 100;
    itoa(n, buf, w);
    printf("Int: %d String: %s\n", n, buf);
    return 0;
}
