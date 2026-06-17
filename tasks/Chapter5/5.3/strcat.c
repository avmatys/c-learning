#include <stdio.h>

void my_strcat(char *s, char *t)
{
    while(*s++) ;
    s--;
    while((*s++ = *t++)) ;
}

int main()
{
    char s[1000] = "Hello, ";
    char t[] = "Andrei";
    my_strcat(s, t);
    printf("Result: %s\n", s);
    return 0;
}
