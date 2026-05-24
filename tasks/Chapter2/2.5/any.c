#include <stdio.h>
#include <stdlib.h>

int findchar(char *source, char c) {
    int idx = 0;
    char ch;
    while((ch = source[idx])) {
        if (ch == c) return idx;
        ++idx;
    }
    return -1;
}

int any(char *source, char *pattern) {
    int idx = -1;
    while(*pattern) {
       idx = findchar(source, *pattern);
       if (idx != -1) return idx;
       pattern++;
    }
    return -1;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s <source> <break>\n", argv[0]);
        exit(1);
    }
    int idx = any(argv[1], argv[2]);
    printf("Index of any char from %s in %s: %d\n", argv[2], argv[1], idx); 
    return 0;
}
