#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

typedef enum { LOWER = 0, UPPER } Mode;

int main(int argc, char *argv[]) 
{
    Mode mode = LOWER;
    if (argc > 1) {
        // Change the approach - compare the flag -u or -l
        if (argv[1][0] == '-' && (argv[1][1] == 'u' || argv[1][1] == 'U'))
            mode = UPPER;
    }
    // Define an array of pointers to function
    int (*casefunc[2])(int) = { tolower, toupper };
    int c;
    while((c = getchar()) != EOF)
        putchar(casefunc[mode](c));
    return 0;
}
