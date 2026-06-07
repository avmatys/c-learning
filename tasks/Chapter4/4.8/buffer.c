#include <stdio.h>

#define EMPTY -1

int BUF = EMPTY; 

int getch(void) 
{
    int tmp;
    if (BUF != EMPTY) {
        tmp = BUF;
        BUF = EMPTY;
        return tmp; 
    }
    return getchar();
}

void ungetch(int c)
{
    BUF = c;
}
