#include <stdio.h>
#include <stdarg.h>

int minscanf(char*, ...);

int main()
{
    char s[3], c1, c2;
    int i1, i2;
    float f1, f2;
    unsigned int u;
    void *p = NULL;
    printf("Copy and paste in the following text to test this program:\n42 -42 100 X hi 1.23e2 4.56e2 %p\n", (void *)&c1);
    if (minscanf("%d %d %u %c%c %s %e %e %p", &i1, &i2, &u, &c1, &c2, s, &f1, &f2, &p) != 9)
        printf("Not all inputs were assigned!\n");
    printf("%d %d %u %c%c %s %e %e %p\n", i1, i2, u, c1, c2, s, f1, f2, p);
    return 0;
}

int minscanf(char *fmt, ...)
{
    va_list ap;
    char *p, *sval, *cval;
    int *ival;
    float *fval;
    void **pval;
    unsigned int *uival;
    int found = 0;

    va_start(ap, fmt);
    for (p = fmt; *p; p++) {
        if (*p != '%') 
            continue;
        switch(*++p) {
            case 'd': 
                ival = va_arg(ap, int *);
                found += scanf("%d", ival);
                break;
            case 'c':
                cval = va_arg(ap, char *);
                found += scanf(" %c", cval);
                break;
            case 'e':
                fval = va_arg(ap, float *);
                found += scanf("%e", fval); // for test is fine
                break;
            case 's': 
                sval = va_arg(ap, char *);
                found += scanf("%s", sval);
                break;
            case 'p':
                pval = va_arg(ap, void**);
                found += scanf("%p", pval);
                break;
            case 'u':
                uival = va_arg(ap, unsigned int *);
                found += scanf("%u", uival);
                break;
            default:
                printf("unsupported format specifier: %%%c\n", *p);
                break;
        }
    }
    va_end(ap);
    return found;
}
