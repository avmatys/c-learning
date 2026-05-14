#include <stdio.h>
#include "my_types.h"

#define IN 1
#define OUT 0

int main() {
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
    printf("\nWords count: %zu, Total capacity: %zu\n", words.size, words.capacity);
    for (int i = 0; i < words.size; i++) {
       printf("Word %d, Value: %s\n", i, words.data[i].data);
    }
    for (int i = 0; i < words.size; i++) {
        CharList_clear(&words.data[i]);
    }
    StringList_clear(&words);
    return 0;
}
