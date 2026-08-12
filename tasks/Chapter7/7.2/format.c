#include <stdio.h>
#include <ctype.h>

#define FORMAT "%x"
#define LINELEN 100

int main()
{
    int c;
    int i = 0;
    while ((c = getchar()) != EOF) {
       if (c == '\n') {
           i = 0;
       } else if (++i == LINELEN) {
           i = 0;
           putchar('\n');
       }
       if (isgraph(c) || isspace(c)) {
           putchar(c);
       } else {
           printf(FORMAT, c);
       }
    }
    return 0;
}

