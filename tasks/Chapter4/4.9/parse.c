#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "parse.h"

#define BUFSIZE 256

int getch(void);
void ungetch(int);

int getop(char s[])
{
    int i, c;
    
    while ((s[0] = c = getch()) == ' ' || c == '\t')
        ;

    s[1] = '\0';
    i = 0;

    // String code operation
    if (isalpha(c)) {
        while(isalpha(s[++i] = c = getch())) 
            ;
        s[i] = '\0';
        if (c != EOF) 
            ungetch(c);
        if (strcmp(s, "sin") == 0) return SIN;
        if (strcmp(s, "exp") == 0) return EXP;
        if (strcmp(s, "pow") == 0) return POW;
        if (strcmp(s, "set") == 0) return SET;
        if (strlen(s) == 1 
            && 'a' <= s[0] && s[0] <= 'z') return VAR;
        return UNKNOWN;
    }

    // Plus, multiply and etc
    if (!isdigit(c) && c != '.' && c != '-')
        return c;

    // Case for the negative number or minus 
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
