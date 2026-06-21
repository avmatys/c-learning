#include <stdio.h>

int my_getline(char *s, int lim)
{
    char *ptr = s;
    int c = 0;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
        *ptr++ = c;
    if (c == '\n')
        *ptr++ = c;
    *ptr = '\0';
    return (ptr - s);
}

int main()
{
    char buf[1024];
    for(int i = 0; i < 5; i++) {
        my_getline(buf, 1024);
        printf("Line: %s\n", buf); 
    }
    return 0;
}
