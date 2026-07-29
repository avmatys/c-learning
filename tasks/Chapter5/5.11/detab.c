#include <stdio.h>
#include <stdlib.h>

#define DEF_TAB 8

int add_stop(int);
int next_stop_dist(int);

int main(int argc, char* argv[]) 
{
    int last = 0;
    int curr = 0;
    for (int i = 1; i < argc; i++) {
       curr = atoi(argv[i]);
       if (curr <= last) continue;
       if (add_stop(curr) == -1) break;
       last = curr;
    }

    int c;
    int col = 0;
    int needed = 0;
    while((c = getchar()) != EOF) {
        if (c == '\n') {
            col = 0;
            putchar(c);
            continue;
        }
        if (c == '\t') {
            needed = next_stop_dist(col);
            col += needed;
            while (needed-- > 0)
                putchar(' ');
            continue;
        }
        col++;
        putchar(c);
    }
    return 0;
}

#define MAX_STOPS 256
static int stops[MAX_STOPS];
static int ptr;

int add_stop(int x)
{
  if (ptr < MAX_STOPS) {
      stops[ptr++] = x;
      return 0;
  } else {
      printf("error: no more space for stops\n");
      return -1;
  }
}

int next_stop_dist(int col) 
{
    for (int i = 0; i < ptr; i++) 
        if (stops[i] > col)
            return stops[i] - col;
    int last = ptr > 0 ? stops[ptr - 1] : 0;
    while(col >= last) 
        last += DEF_TAB;
    return last - col;
}
