#include "stack.h"

#include <stdio.h>

static double stack[STACK_CAPACITY];
static int ptr = 0;

void push(double x)
{
    if (ptr < STACK_CAPACITY) {
        stack[ptr++] = x;
    } else {
        printf("error: stack is full\n");
    }
}

double pop() 
{
    if (ptr > 0) {
        return stack[--ptr];
    } else {
        printf("error: stack is empty\n");
        return 0;
    }
}

