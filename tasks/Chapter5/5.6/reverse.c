#include <stdio.h>

size_t my_strlen(const char *s)
{
    const char *tmp;
    for(tmp = s; *tmp; tmp++) ;
    return (tmp - s);
}

void my_reverse(char *start)
{
    char c;
    size_t n = my_strlen(start);
    if (n < 2) 
        return;
    char *end = start + n - 1;
    while (start < end) {
       c = *start;
       *start = *end;
       *end = c;
       start++; end--;
    }
}

int main()
{
    char s[] = "Hello, Andrei";
    my_reverse(s);
    printf("Reversed string %s\n", s); 
    return 0;
}
