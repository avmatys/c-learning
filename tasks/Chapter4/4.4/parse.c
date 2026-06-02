#include <stdio.h>
#include <ctype.h>

#ifndef NUMBER
    #define NUMBER '0'
#endif

int getch(void);
void ungetch(int);

int getop(char s[])
{
    int i, c;
    
    while ((s[0] = c = getch()) == ' ' || c == '\t')
        ;

    s[1] = '\0';
    if (!isdigit(c) && c != '.' && c != '-')
        return c;
    i = 0;
    if (c == '-') {
        if ((c = getch()) == '.' || isdigit(c)) {
            s[++i] = c;
        } else {
            if (c != EOF) 
                ungetch(c);
            return '-';
        }
    }
    if (isdigit(c))
        while (isdigit(s[++i] = c = getch())) ;
    if (c == '.')
        while (isdigit(s[++i] = c = getch())) ;
    s[i] = '\0';
    if (c != EOF) 
        ungetch(c);

    return NUMBER;
}
