#include <stdio.h>

size_t my_strlen(const char *s)
{
    const char *tmp = s;
    while (*tmp++) ;
    return tmp - s - 1;
}

int my_strcmp(const char *s, const char *t)
{
    for (; *s == *t; s++, t++)
        if (*s == '\0')
            return 0;
    return *s - *t;
}

int my_strend(const char *s, const char *t)
{
    size_t tlen = my_strlen(t);
    size_t slen = my_strlen(s); 
    if (slen >= tlen && my_strcmp(s + slen - tlen, t) == 0)
        return 1;
    return 0;
}

int main()
{
    char *s = "Hello, Andrei";
    char *t = "Andrei";
    printf("Length: %zu\n", my_strlen(t));
    printf("Result: %s, %s = %d\n", s, t, my_strend(s, t));
    char *s1 = "Hello, An";
    printf("Result: %s, %s = %d\n", s1, t, my_strend(s1, t));
    return 0;
}
