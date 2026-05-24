#include <stdio.h>
#include "my_types.h"

#define IN 1
#define OUT 0

int main() {
    
    // Input chars
    StringList words;
    StringList_init(&words);
    CharList word;
    int state = OUT;
    char c;
    while((c = getchar()) != EOF) {
        if (c == '\n' || c == '\t' || c == ' ') {
            if (state == IN) {
                CharList_add(&word, '\0'); // c string style
                StringList_add(&words, word);
            }
            state = OUT; 
        } else {
            if (state == OUT) {
                state = IN;
                CharList_init(&word);
            }
            CharList_add(&word, c);
        }

    }
    if (state == IN) {
        CharList_add(&word, '\0'); // c string style
        StringList_add(&words, word);
    }

    // Draw a vertical histogram
    size_t maxlen = 0;
    for (size_t i = 0; i < words.size; i++) {
        if (words.data[i].size - 1 > maxlen) maxlen = words.data[i].size - 1;
    }
    printf("\nVertical hitsogram\n\n");
    for (size_t h = maxlen; h > 0; h--) {
        printf("%2zu | ", h);
        for (int i = 0; i < words.size; i++) {
            if (words.data[i].size - 1 >= h) {
                printf("  #  ");
            } else {
                printf("     ");
            }
        }
        printf("\n");
    }
    printf("     ");
    for (size_t i = 0; i < words.size; i++) printf("-----");
    printf("\n");
    printf("     ");
    for (size_t i = 0; i < words.size; i++) printf(" [%zu] ", i);
    printf("\n\n");

    // Clear the memory
    for (int i = 0; i < words.size; i++) {
        CharList_clear(&words.data[i]);
    }
    StringList_clear(&words);
    return 0;
}
