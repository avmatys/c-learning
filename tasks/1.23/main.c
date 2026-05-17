#include <stdio.h>
#include <stdlib.h>

#define STATE_CODE 0
#define STATE_SLASH 1
#define STATE_LINE_COMMENT 2
#define STATE_BLOCK_COMMENT 3
#define STATE_STAR 4
#define STATE_STRING 5
#define STATE_ESCAPED_STRING 6
#define STATE_CHAR 7
#define STATE_ESCAPED_CHAR 8

/** Multiline commment with differnt // 
 *
 *
 *
 *
 */

int main() {
    // Open the current source file
    FILE *inptr;
    FILE *outptr;
    inptr = fopen(__FILE__, "r");
    outptr = fopen("main2.c", "w");
    if (inptr == NULL || outptr == NULL) {
        printf("Can't process the file");
        exit(1);
    }
    // Read char by char until the end of the file
    int c;
    int state = STATE_CODE;
    while((c = fgetc(inptr)) != EOF) {
        switch(state) {
            case STATE_CODE:
                if (c == '/') {
                    state = STATE_SLASH;
                } else {
                    fputc(c, outptr);
                    if (c == '"') state = STATE_STRING;
                    else if (c == '\'') state = STATE_CHAR;
                }
                break;

            case STATE_SLASH: 
                if (c == '*') state = STATE_BLOCK_COMMENT;
                else if (c == '/') state = STATE_LINE_COMMENT;
                else {
                    fputc('/', outptr);
                    fputc(c, outptr);
                    if (c == '"') state = STATE_STRING;
                    else if (c == '\'') state = STATE_CHAR;
                    else state = STATE_CODE;
                }
                break;
            
            case STATE_LINE_COMMENT:
                if (c == '\n') {
                    fputc(c, outptr);
                    state = STATE_CODE;
                }
                break;
            
            case STATE_BLOCK_COMMENT:
                if (c == '*') {
                    state = STATE_STAR;
                }
                break;

            case STATE_STAR:
                if (c == '/') {
                    state = STATE_CODE;
                } else if (c != '*') {
                    state = STATE_BLOCK_COMMENT;
                } 
                break;

            case STATE_CHAR: 
                fputc(c, outptr);
                if (c == '\\') state = STATE_ESCAPED_CHAR;
                else if (c == '\'') state = STATE_CODE;
                break;

            case STATE_ESCAPED_CHAR:
                fputc(c, outptr);
                state = STATE_CHAR;
                break;

            case STATE_STRING:
                fputc(c, outptr);
                if (c == '\\') state = STATE_ESCAPED_STRING;
                else if (c == '"') state = STATE_CODE;
                break;

            case STATE_ESCAPED_STRING:
                fputc(c, outptr);
                state = STATE_STRING;
                break;
        }
    }
    fclose(inptr);
    fclose(outptr);
    return 1;
}
