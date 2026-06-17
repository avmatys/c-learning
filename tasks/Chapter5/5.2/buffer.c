#include <stdio.h>

#define BUFSIZE 100

int buf[BUFSIZE];
int idx;

int getch(void)
{
    return (idx > 0) ? buf[--idx] : getchar();
}   

void ungetch(int c)
{
    if (idx > BUFSIZE) printf("ungetch: out of space");
    else buf[idx++] = c;
}  

void bufreset(void)
{
    idx = 0;
}
