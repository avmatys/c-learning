#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 5000

char *lineptr[MAXLINES];

int readlines(char *lineptr[], int nlines);
int readlinebuf(char *buf, int lim);
void writelines(char *lineptr[], int nlines);

void myqsort(void *lineptr[], int left, int right, 
           int (*comp) (void *, void *));
int numcmp(const char *, const char *);
int rnumcmp(const char *, const char *);
int rstrcmp(const char *, const char *); // standard strcmp is used

// Main runner
int main(int argc, char *argv[]) 
{
    int nlines;
    int numeric = 0;
    int reverse = 0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0) 
            numeric = 1;
        if (strcmp(argv[i], "-r") == 0)
            reverse = 1;
    }
    // Execute the quick sort
    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        if (numeric) {
            myqsort((void **) lineptr, 0, nlines - 1, 
            (int (*) (void *, void *)) (reverse ? rnumcmp : numcmp));
        } else {
            myqsort((void **) lineptr, 0, nlines - 1, 
            (int (*) (void *, void *)) (reverse ? rstrcmp : strcmp));
        }
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("intput too big to sort");
        return 1;
    }
}

// Sorting and comparison section
void myqsort(void *v[], int left, int right, 
           int (*comp) (void*, void*)) 
{
    int i, last;
    void swap(void *v[], int i, int j);

    if (left >= right)
        return;
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++)
        if ((*comp)(v[i], v[left]) < 0)
            swap(v, ++last, i);
    swap(v, left, last);
    myqsort(v,left, last-1, comp);
    myqsort(v, last+1, right, comp);
}

void swap(void *v[], int i, int j)
{
    void *tmp;
    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}

int numcmp(const char *a, const char *b) 
{
    double v1 = atof(a);
    double v2 = atof(b);
    if (v1 > v2) return 1;
    else if (v1 < v2) return -1;
    else return 0;
}

int rnumcmp(const char *a, const char *b)
{
    return -numcmp(a, b);
}

int rstrcmp(const char *a, const char *b)
{
    return -strcmp(a, b);
}

// Section to work with lines

#define BUF_LEN 1000

int readlinebuf(char *buf, int lim)
{
    int c, i;
    i = 0;
    while (i < lim - 1 && (c = getchar()) != EOF && c != '\n') 
        buf[i++] = c;
    if (c == '\n') 
        buf[i++] = '\n';
    buf[i] = '\0';
    return i;
}

int readlines(char *lineptr[], int nlines) 
{
    char buf[BUF_LEN];
    char *ptr;
    int len;
    int clines = 0;
    while (clines < nlines && (len = readlinebuf(buf, BUF_LEN)) > 0) {
        if (buf[len - 1] == '\n' && len == 1) 
            break;
        if (buf[len - 1] == '\n') {
            buf[--len] = '\0'; // Remove the new line from the input buffer
        }
        ptr = malloc(sizeof(char) * len);
        if (!ptr) break;
        strcpy(ptr, buf);
        lineptr[clines++] = ptr;
    }
    return clines;
}

void writelines(char *lineptr[], int nlines) 
{
    for (int i = 0; i < nlines; i++) 
        printf("%s\n", lineptr[i]);
}
