#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 5000
#define MAXSTORAGE 10000000

char *lineptr[MAXLINES];
char storage[MAXSTORAGE];

int readlines(char *lineptr[], int nlines, char *storage);
void writelines(char *lineptr[], int nlines);
void my_qsort(char *lineptr[], int left, int right);

// Sort input data
int main() 
{
    int nlines;
    if ((nlines = readlines(lineptr, MAXLINES, storage)) >= 0) {
        my_qsort(lineptr, 0, nlines - 1);
        printf("\nSorted lines\n");
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("error: input too big to sort\n");
        return 1;
    }
}

#define MAXLEN 1000
int my_getline(char *s, int lim);

// Read lines from the input
int readlines(char *lineptr[], int maxlines, char *startptr)
{
    int len, nlines;
    char *p, line[MAXLEN], *endptr;

    endptr = startptr + MAXSTORAGE;
    nlines = 0;
    while ((len = my_getline(line, MAXLEN)) > 0) {
        if (line[len-1] == '\n') {
             line[len-1] = '\0';
             len--;
        }
        if (nlines >= maxlines || (startptr + len + 1) > endptr)
            return -1;
        else {
            strcpy(startptr, line);
            lineptr[nlines++] = startptr;
            startptr += len + 1; // additional '\0'
        }
    }
    return nlines;
}

// Write lines
void writelines(char *lineptr[], int nlines)
{
    int i;
    for (i = 0; i < nlines; i++)
        printf("%s\n", lineptr[i]);
}

// Sort the array of strings
void my_qsort(char *v[], int left, int right)
{
    int i, last;
    void swap(char *v[], int i, int j);

    if (left >= right)
        return;
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left+1; i <= right; i++)
        if (strcmp(v[i], v[left]) < 0) 
            swap(v, ++last, i);
    swap(v, left, last);
    my_qsort(v, left, last-1);
    my_qsort(v, last+1, right);
}

void swap(char *v[], int i, int j)
{
    char *tmp;
    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}

int my_getline(char *s, int lim)
{
    char *ptr = s;
    int c = 0;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
        *ptr++ = c;
    if (c == '\n')
        *ptr++ = c;
    *ptr = '\0';
    return (ptr - s);
}
