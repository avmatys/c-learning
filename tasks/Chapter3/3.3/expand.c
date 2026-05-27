#include <stdio.h>
#include <ctype.h>

#define MAX_LEN 1024

typedef enum { INITIAL, FIRST, DELIM } State;

int expand(char *to, char *from) 
{
    State cstate = INITIAL;
    State pstate = INITIAL;
    int c, p;
    int c1, c2, k;
    while((c = *from++) != '\0') {
        if (isspace(c)) continue; // skip all white spaces
        if (cstate == INITIAL) {
            if (c == '-') { 
                 cstate = pstate == DELIM ? DELIM : INITIAL;
            } else {
                cstate = FIRST;
                p = c;
            }
        } else if (cstate == FIRST) {
            if (c == '-') {
                cstate = DELIM;
            } else {
                // incorrect sequence
                return -1;
            }

        } else if (cstate == DELIM) {
            if (c != '-') {
                // Expand here
                c1 = p; c2 = c;
                if (c1 > c2) {
                    for (k = c1; k >= c2; k--)
                        *to++ = k;
                } else {
                    for (k = c1; k <= c2; k++)
                        *to++ = k;
                }
                // Change the state
                p = c;
                pstate = DELIM;
                cstate = INITIAL;
            } else {
                // incorrect sequence
                return -1;
            }
        }
    }
    *to++ = '\0';
    return 0;
}

int main()
{
    char* from = "----a-z9-0-50-2-0.-i---------";
    char to[MAX_LEN];
    expand(to, from);
    printf("Initial string: %s\nExpanded string: %s\n", from, to);
}
