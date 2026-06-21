#include <stdio.h>

char*  my_strncpy(char *dest, const char *src, size_t n)
{
    size_t i;
    for(i = 0; i < n && src[i] != '\0'; i++)
        dest[i] = src[i];
    for (; i < n; i++)
        dest[i] = '\0';
    return dest;
}

int main()
{
    char s[50] = "Here we are";
    char t[] = "Mememe";
    printf("Old string: %s\n", s);
    printf("New string: %s\n", my_strncpy(s, t, 4));
    return 0;
}
