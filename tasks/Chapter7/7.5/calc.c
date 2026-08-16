#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double pop(void);
void push(double);

#define MAXOP 100

int main()
{
    char c;
    char s[MAXOP];
    double val2, num; 
    while (scanf("%s%c", s, &c) == 2) {
        if (sscanf(s, "%lf", &num) == 1) {
            push(num);
        } else if (strlen(s) == 1) {
            switch(*s) {
                case '+' : 
                    push(pop() + pop());
                    break;
                case '-' : 
                    val2 = pop();
                    push(pop() - val2);
                    break;
                case '*' :
                    push(pop() * pop());
                    break;
                case '/' : 
                    val2 = pop();
                    if (val2 != 0.0)
                        push(pop() / val2);
                    break;
                default :
                    printf("unknown operation\n");
                    break;
            }
        } else {
            printf("unsupported operation\n");
        }
        if (c == '\n') 
            printf("\t%.8g\n", pop());
    }
    return 0;
}

#define BUFSIZE 100
static int bufptr = 0;
static double buf[BUFSIZE];

double pop()
{
    if (bufptr > 0) {
        return buf[--bufptr];
    } else {
        printf("Buffer is empty\n");
        return 0.0;
    }
}

void push(double val)
{
    if (bufptr < BUFSIZE) {
        buf[bufptr++] = val;
    } else {
        printf("Buffer is full\n");
    }
}
