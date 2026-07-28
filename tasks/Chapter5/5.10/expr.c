#include <stdio.h>
#include "stack.h"
#include <ctype.h>

#define NUM 1
#define ADD 2
#define SUB 3
#define MUL 4
#define DIV 5

int getop(char* ptr, double* num); 

int main(int argc, char* argv[]) 
{
    int i = 0;
    double num, tmp;
    while (++i < argc) {
        switch(getop(argv[i], &num)) {
            case NUM:
                push(num);
                break;
            case ADD:
                push(pop() + pop());
                break;
            case SUB:
                tmp = pop();
                push(pop() - tmp);
                break;
            case MUL:
                push(pop() * pop());
                break;
            case DIV:
                tmp = pop();
                if (tmp == 0.0){
                    printf("Error: div by zero\n");
                    return 1;
                }
                push(pop() / tmp);
                break;
            default:
                printf("Command %s is unknown\n", argv[i]);
        }
    }
    printf("Execution result: %f\n", pop());
    return 0;
}

int getop(char *ptr, double* num)
{   
    if (*ptr == '-') {
        if (isdigit(*(ptr + 1))) {
            getop(ptr + 1, num);
            *num = -*num;
            return NUM;
        } else 
            return SUB;
    } else if (*ptr == '+') {
        return ADD;
    } else if (*ptr == '*') {
        return MUL;
    } else if (*ptr == '/') {
        return DIV;
    } else if (isdigit(*ptr)) {
        double tmp = 0.0;
        char c;
        while(isdigit(c = *ptr++)) 
            tmp = 10.0 * tmp + (c - '0'); 
        *num = tmp;
        return NUM;
    } else return -1;
}
