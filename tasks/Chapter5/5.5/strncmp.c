#include <stdio.h>

int my_strncmp(const char *s1, const char *s2, size_t n)
{
    char c1, c2;
    while (n-- > 0) {
        c1 = *s1++;
        c2 = *s2++;
        if (c1 != c2)
            return c1 - c2;
        if (c1 == '\0')
            return 0;
    }
    return 0;
}

int main()
{
    char *s1 = "Hello, World!";
    char *s2 = "Hello, Andrei!";
    printf("Compare 5: %d\n", my_strncmp(s1, s2, 5));
    printf("Compare 10: %d\n", my_strncmp(s1, s2, 10));
    return 0;
}
