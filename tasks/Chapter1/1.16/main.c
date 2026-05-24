#include <stdio.h>
#include "my_types.h"

int sgetline(CharList* line) {
    int c;
    while ((c = getchar()) != EOF && c != '\n') 
        // Unable to allocate memory for a new char
        if (CharList_add(line, c) == -1) return -1;
    if (c == '\n') 
        // Unable to allocate memory for a new char
        if (CharList_add(line, c) == -1) return -1;
    return c == EOF ? 0 : 1;
}

void copy(CharList* to, CharList* from) {
    if (to == NULL || from == NULL) return;
    CharList_clear(to);
    CharList_init(to);
    for (int i = 0; i < from->size; i++)
        CharList_add(to, from->data[i]);
}

int main() {
    // Input data
    CharList word;
    CharList maxLenWord;
    CharList_init(&word);
    CharList_init(&maxLenWord);
    size_t maxlen = 0;
    size_t status = 0;
    while(1) {
        status = sgetline(&word);
        if (status == -1) {
            printf("Error allocating memory. Result is not correct");
            break;
        }
        if (maxLenWord.size < word.size) {
            CharList_clear(&maxLenWord); // Free memory
            CharList_init(&maxLenWord); // Init new memory
            copy(&maxLenWord, &word); // Deep copy of the data
        }
        if (status == 0) break;
        CharList_init(&word); // Prepare a new word
    }
    // Print the result word
    printf("\nMax length word: %.*s\n", (int) maxLenWord.size, maxLenWord.data); 
    // Clear the memory
    CharList_clear(&word);
    CharList_clear(&maxLenWord);
    return 0;
}
