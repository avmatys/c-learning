#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef enum {
    CODE,
    SLASH,
    LINE_COMMENT,
    BLOCK_COMMENT,
    STAR,
    CHAR,
    CHAR_ESCAPED,
    STRING,
    STRING_ESCAPED
} State;

#define STACK_SIZE 1000

typedef struct {
    int data[STACK_SIZE];
    int capacity;
    int size;
} Stack;

void Stack_init(Stack *st) {
    st->capacity = STACK_SIZE;
    st->size = 0;
}

int Stack_push(Stack *st, int c) {
    if (st == NULL)
        return 1;
    if (st->size >= st->capacity) {
        printf("Stack Overflow");
        exit(1);
    }
    st->data[st->size++] = c;
    return 0;
}

bool Stack_peek(Stack *st, int *v) {
    if (st == NULL || st->size <= 0) 
        return false;
    *v = st->data[st->size - 1];
    return true;
}

bool Stack_pop(Stack *st, int *v) {
    if (st == NULL || st->size <= 0) 
        return false;
    *v = st->data[--st->size];
    st->data[st->size] = 0; // remove old value
    return true;
}

bool Stack_isEmpty(Stack *st) {
    return st->size == 0;
}

int main() 
{
    FILE *in;
    in = fopen("dummy.c", "r");
    if (in == NULL) {
        printf("\nCan't open input file for reading\n");
        exit(1);
    }
    int c, v;
    int char_len = 0;
    int result = 1;
    Stack st;
    Stack_init(&st);
    State state = CODE;
    while ((c = fgetc(in)) != EOF && result == 1) {
        switch(state) {
            case CODE: 
                if (c == '/') state = SLASH;
                else if (c == '\'') {
                    state = CHAR;
                    char_len = 0;
                }
                else if (c == '"') state = STRING;
                else if (c == '{' || c == '[' || c == '(') Stack_push(&st, c);
                else if (c == ']') {
                    if (!Stack_peek(&st, &v) || v != '[') {
                        printf("\nSequence of brackets is not correct. Got ] without corresponding open bracket");
                        exit(1);
                    } else {
                        Stack_pop(&st, &v);
                    }
                } else if (c == ')') {
                    if (!Stack_peek(&st, &v) || v != '(') {
                        printf("\nSequence of brackets is not correct. Got ) without corresponding open bracket");
                        exit(1);
                    } else {
                        Stack_pop(&st, &v);
                    }
                } else if (c == '}') {
                    if (!Stack_peek(&st, &v) || v != '{') {
                        printf("\nSequence of brackets is not correct. Got } without corresponding open bracket");
                        exit(1);
                    } else {
                        Stack_pop(&st, &v);
                    }
                }
                break;

            case SLASH:
                if (c == '/') state = LINE_COMMENT;
                else if (c == '*') state = BLOCK_COMMENT;
                else {
                    if (c == '"') state = STRING;
                    else if (c == '\'') state = CHAR;
                    else state = CODE;
                }
                break;

            case LINE_COMMENT: 
                if (c == '\n') state = CODE;
                break;

            case BLOCK_COMMENT: 
                if (c == '*') state = STAR;
                break;

            case STAR:
                if (c == '/') state = CODE;
                else if (c != '*') state = BLOCK_COMMENT;
                break;

            case CHAR:
                if (c == '\\') {
                    state = CHAR_ESCAPED;
                }
                else if (c == '\'') {
                    if (char_len == 0) {
                        printf("\nChar literal can't be empty\n");
                        result = 0;
                    }
                    if (char_len > 1) {
                        printf("\nChar literal can't be more that 1 symbol or escaped sequence");
                        result = 0;
                    }   
                    state = CODE;
                }
                else if (c == '\n' || c == '\t') {
                    printf("\nNew line or tabs are not allowed in the literal");
                    result = 0;
                }
                else {
                    char_len++;
                    if (char_len > 1) {
                        printf("\nChar literal can't be more that 1 symbol or escaped sequence");
                        result = 0;
                    }
                }
                break;

            case CHAR_ESCAPED:
                if (c == '\n' || c == '\t') {
                    printf("\nNew line or tabs are not allowed in the literal");
                    result = 0;
                } else {
                    char_len++;
                    if (char_len > 1) {
                        printf("\nChar literal can't be more that 1 symbol or escaped sequence");
                        result = 0;
                    } 
                    else state = CHAR;
                }
                break;

            case STRING:
                if (c == '\\') state = STRING_ESCAPED;
                else if (c == '"') state = CODE;
                break;

            case STRING_ESCAPED:
                state = STRING;
                break;

        }
    }
    if (result == 1) {
        if (state == CHAR) {
            printf("\nThe ' is not closed in the char");
            result = 0;
        }
        else if (state == STRING) {
            printf("\nThe \" is not closed in the string");
            result = 0;
        }
        else if (!Stack_isEmpty(&st)) {
            printf("\nThe bracket sequence is not valid - open brackets left"); 
            result = 0;
        }
        else {
            printf("\nAll good\n");
        }
    }
    fclose(in);
    return result;
}
