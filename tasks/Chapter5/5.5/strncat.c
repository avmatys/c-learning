#include <stdio.h>

size_t my_strlen(const char *s)
{
    const char *tmp;
    for(tmp = s; *tmp; tmp++) ;
    return (tmp - s);
}

char *my_strncat(char *dest, const char *src, size_t n)
{
    size_t dlen = my_strlen(dest);
    char *d;
    for (d = dest + dlen; n-- > 0 && *src; src++, d++)
        *d = *src;
    *d = '\0';
    return dest;
}

int main()
{
    char dest[1024] = "Hello,";
    char src[] = "Andrei!";
    printf("Concat str: %s\n", my_strncat(dest, src, 1));
    return 0;
}
