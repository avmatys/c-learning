#include <stdio.h>

#define MAXVAL 100

void push(double);
double pop(void);
double peek(void);
void duptop(void);
void swaptop(void);
void clear(void);

static int sp = 0 ;
static double val[MAXVAL];

void push(double x) 
{
    if (sp < MAXVAL) {
        val[sp++] = x;
    } else {
        printf("error: stack pull, can't push %g\n", x);
    }
}

double pop() 
{
    if (sp > 0) {
        return val[--sp];
    } else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

double peek() 
{
    if (sp > 0) {
        return val[sp - 1];
    } else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

void duptop(void) 
{
    if (sp > 0) {
        push(peek());
    }
}

void swaptop(void) 
{
    if (sp > 1) {
        double x1 = pop();
        double x2 = pop();
        push(x1);
        push(x2);
    }
}

void clear(void) 
{
    sp = 0;
}

