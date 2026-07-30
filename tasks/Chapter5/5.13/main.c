#include "cyclic.c"
#include <stdio.h>

#define MAX_LEN 1000 
#define DEF_N 10

int my_getline(char buf[], int lim);

int main(int argc, char* argv[]) 
{
    int n = DEF_N;
    for (int i = 1; i < argc; i++) {
        if (*argv[i] == '-') 
            n = atoi(argv[i] + 1);
    }
    if (n <= 0) {
        printf("incorrect value of n");
        return 1;
    }

    CyclicBuffer cb;
    if (init_buffer(&cb, n) == -1) {
        printf("can't allocate memory for cyclic buffer\n");
        return 1;
    }
    char buf[MAX_LEN];
    int len;
    while ((len = my_getline(buf, MAX_LEN)) > 0) 
        if (push_buffer(&cb, buf) == -1) 
            printf("can't add a line %s to the buffer", buf);

    char *tmp;
    while(poll_buffer(&cb, &tmp) == 0)
       printf("%s\n", tmp); 
    free_buffer(&cb);

    return 0;
}

int my_getline(char buf[], int lim) 
{
   int c, i;
   i = 0;
   while(i < lim - 1 && (c = getchar()) != EOF && c != '\n') 
       buf[i++] = c;
   buf[i] = '\0';
   return i;
}
