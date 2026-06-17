#include <stdio.h>
#include <ctype.h>
#include <math.h>

// Buffer
int getch(void);
void ungetch(int c);
void bufreset(void);

// Current
int getfloat(double *x)
{
    int c, sign, e, tmp;
    double val;

    // Skip spaces
    while (isspace(c = getch()))
        ;
    // Check what this is number
    if (!isdigit(c) && c != '+' && c != '-') {
        ungetch(c);
        return 0;
    }
    // Process sign
    sign = c == '-' ? -1 : 1;
    if (c == '+' || c == '-') {
        tmp = getch();
        if (!isdigit(tmp)) {
            ungetch(tmp);
            ungetch(c);
            return 0;
        }
        c = tmp;
    }
    // Get main part
    for (val = 0; isdigit(c); c = getch())
        val = 10 * val + (c - '0');
    // Get part after the dot
    e = 0;
    if (c == '.') {
        for (;isdigit(c = getch()); e--)
            val = 10 * val + (c - '0');
    }
    // Prcoss exp part
    if (c == 'e' || c == 'E') {
        c = getch();
        int esign = c == '-' ? -1 : 1;
        if (c == '-' || c == '+') 
            c = getch();
        int exp = 0;
        for(;isdigit(c); c = getch())
            exp = 10 * exp + (c - '0');
        e += esign * exp;
    }

    if (e != 0) val *= pow(10.0, e);
    val *= sign;
    *x = val;

    if (c != EOF)
        ungetch(c);
    return c;
}


int main()
{
    int c;
    double x;
    while ((c = getfloat(&x)) != EOF){
        if (c > 0) printf("%f\n", x);
        else {
            printf("input error\n");
            getch();
        }
    }
    return 0;
}

