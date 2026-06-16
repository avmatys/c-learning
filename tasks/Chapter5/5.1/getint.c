#include <ctype.h>
#include <stdio.h>

int getch(void);
void ungetch(int);

int getint(int *pn)
{
    int c, tmp, sign;
    while(isspace(c = getch())) 
        ;
    if (!isdigit(c) && c != EOF && c != '-' && c != '+') {
        ungetch(c);
        return 0;
    }
    sign = c == '-' ? -1 : 1;
    if (c == '-' || c == '+') {
        tmp = getch();
        if (!isdigit(tmp)) {
            ungetch(tmp);
            ungetch(c);
            return 0;
        } else {
            c = tmp;
        }
    }
    for (*pn = 0; isdigit(c); c = getch())
        *pn = 10 * *pn + (c - '0');
    *pn *= sign;
    if (c != EOF)
        ungetch(c);
    return c; 
}

#define SIZE 100

int main()
{
    int n, array[SIZE], getint(int *);
    for (int i = 0; i < SIZE; i++) array[i] = 0;
    for (n = 0; n < SIZE && getint(&array[n]) != EOF; n++)
        printf("Next number is: %d \n", array[n]);
    return 0;
}

