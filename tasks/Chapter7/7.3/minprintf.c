#include <stdio.h>
#include <stdarg.h>

void minprintf(char*, ...);

int main()
{
    char *sval = "10";
    int ival = 111;
    double dval = 22.33;
    unsigned int uival = 444;
    char cval = 'H';
    void *pval = sval;
    printf("Test: %d %s %c %f %p %u\n", ival, sval, cval, dval, pval, uival);
    minprintf("Test: %d %s %c %f %p %u\n", ival, sval, cval, dval, pval, uival);
    return 0;
}

void minprintf(char *fmt, ...)
{
    va_list ap;
    char *p, *sval;
    int ival;
    double dval;
    void *pval;
    unsigned int uival;

    va_start(ap, fmt);
    for (p = fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }
        switch(*++p) {
            case 'd': 
                ival = va_arg(ap, int);
                printf("%d", ival);
                break;
            case 'c':
                ival = va_arg(ap, int);
                printf("%c", ival);
                break;
            case 'f':
                dval = va_arg(ap, double);
                printf("%f", dval);
                break;
            case 's': 
                for (sval = va_arg(ap, char*); *sval; sval++)
                    putchar(*sval);
                break;
            case 'p':
                pval = va_arg(ap, void*);
                printf("%p", pval);
                break;
            case 'u':
                uival = va_arg(ap, unsigned int);
                printf("%u", uival);
                break;
            default:
                putchar('%');
                putchar(*p);
                break;
        }
    }
    va_end(ap);
}
