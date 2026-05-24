#include <stdio.h>
#include <stdbool.h>
#include "my_types.h"

int TAB_SPACES = 4;
int TAB_SYMBOL = '*';

int main() {
    // Input data
    CharList buffer;
    CharList_init(&buffer);
    int c;
    while((c = getchar()) != EOF) {
        if (c == '\t') {
            for(size_t i = 0; i < TAB_SPACES; i++) 
               CharList_add(&buffer, TAB_SYMBOL); 
        } else {
            CharList_add(&buffer, c);
        }
    }
    // Print the result
    printf("\nThe changed buffer: %.*s\n", (int) buffer.size, buffer.data);
    // Clear the memory
    CharList_clear(&buffer);
    return 0;
}
