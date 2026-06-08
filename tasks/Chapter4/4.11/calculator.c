#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "parse.h"

#define MAXOP 100

int getop(char s[]);
void push(double);
double pop(void);
double peek(void);

double getvar(int c);
void setvar(int c, double val);

int main()
{
    int type, tmptype;
    double op1, op2;
    char s[MAXOP];

    while((type = getop(s)) != EOF) {
        switch(type) {
            case NUMBER: 
                push(atof(s));
                break;
            case '+': 
                push(pop() + pop());
                break;
            case '*':
                push(pop() * pop());
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case '/': 
                op2 = pop();
                if (op2 != 0.0) 
                    push(pop() / op2);
                else 
                    printf("error: zero division\n");
                break;
            case '%':
                op2 = pop();
                if (op2 != 0.0)
                    push(pop() / op2);
                else 
                    printf("error: zero modulo\n");
                break;
            case SIN:
                push(sin(pop()));
                break;
            case EXP:
                push(exp(pop()));
                break;
            case POW:
                op2 = pop();
                op1 = pop();
                if (op1 == 0 && op2 <= 0)
                    printf("error: incorrect pow\n");
                else 
                    push(pow(op1, op2));
                break;
            case SET:
                tmptype = getop(s);
                if (tmptype == VAR)
                    setvar(s[0], peek());
                break;
            case VAR:
                push(getvar(s[0]));
                break;
            case '\n':
                printf("\t%.8g\n", pop());
                break;
            default:
                printf("error: unknown command %s\n", s);
                break;
        }
    }
    return 0;
}

