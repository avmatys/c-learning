#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void squeeze(char *s, char *template) {
    bool remove[256];
    char ch;
    while(template != NULL && (ch = *template) != '\0') { 
        remove[ch] = true;
        template++;
    }
    int i, j;
    for (i = j = 0; s[i] != '\0'; i++)
        if (!remove[s[i]]) 
            s[j++] = s[i];
    s[j] = '\0';
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s <source> <template>\n", argv[0]);
        exit(1);
    }
    char* source = argv[1];
    char* template = argv[2];
    squeeze(source, template);
    printf("Remove string: %s\nResult string: %s\n", template, source); 
    return 1;
}
