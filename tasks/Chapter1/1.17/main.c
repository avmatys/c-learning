#include <stdio.h>
#include <stdbool.h>
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

bool is_whitespace(int c) {
    return c == ' ' || c == '\t';
}

void trim_right(CharList* line) {
    while(line->size > 0 && is_whitespace(line->data[line->size - 1])) 
        line->data[line->size--] = '*';
}

int main() {
    // Input data
    CharList word;
    CharList_init(&word);
    size_t status = 0;
    while(1) {
        status = sgetline(&word);
        if (status == -1) {
            printf("Error allocating memory. Result is not correct");
            break;
        }
        trim_right(&word);
        if(word.size == 0) {
            printf("Trimmed string is empty\n");
        } else {
            printf("Current trimmed string: %.*s\n", (int) word.size, word.data);
        }
        if (status == 0) break;
        CharList_init(&word); // Prepare a new word
    }
    // Clear the memory
    CharList_clear(&word);
    return 0;
}
