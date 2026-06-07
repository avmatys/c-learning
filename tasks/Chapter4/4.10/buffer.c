#include <stdio.h>

#define BUFSIZE 1000

char buf[BUFSIZE];
int idx = 0;
int len = 0;

int my_getline(char s[], int lim) 
{
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
        s[i] = c;
    if (c == '\n')
        s[i++] = c;
    s[i] = '\0';
    return i;
}

int getch(void) 
{
    if (idx >= len) {
        len = my_getline(buf, BUFSIZE);
        if (len == 0)
            return EOF;
        idx = 0;
    }
    return buf[idx++]; 
}

void ungetch(int c)
{
    if (idx > 0) 
        idx--;
}
