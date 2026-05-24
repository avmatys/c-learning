#include <stdio.h>
#include <stdlib.h>

unsigned long htoi(const char* s) {
    unsigned long value = 0;
    char ch = *s;
    while(ch == ' ' || ch =='\t') 
        ch = *(++s);
    if (ch == '0') {
        ch = *(++s);
        if (ch != 'x' && ch != 'X') 
            ch = *(--s);
        else 
            ch = *(++s);
    }
    for (;;) {
        if (ch >= '0' && ch <= '9') 
            value = (value << 4) + (ch - '0');
        else if (ch >= 'a' && ch <= 'f') 
            value = (value << 4) + (ch - 'a' + 10);
        else if (ch >= 'A' && ch <= 'F') 
            value = (value << 4) + (ch - 'A' + 10);
        else
            return value;
        ch = *(++s);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <hex number>\n", argv[0]);
        exit(1);
    }
    char* s = argv[1];
    unsigned long result = htoi(s);
    printf("Input value: %s Transformed value: %lu \n", s, result);
    return 1;
}
