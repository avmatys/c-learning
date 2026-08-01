#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLINES 5000
#define MAXFIELDS 20

#define BUF_LEN 1000

#define MODE_STR  1
#define MODE_NUM  2
#define MODE_DIR  3 

#define SORT_ASC  1
#define SORT_DESC 2

#define CASE_SENS   1
#define CASE_INSENS 2

#define STATE_SKIP 0
#define STATE_STORE_FLAG 1

typedef int (*Comparator) (void *, void *);

// Buffer to store compare options
typedef struct {
    int number;
    Comparator comp;
} Field;

static Field fields[MAXFIELDS];
int nfields = 0;

// Buffer for the read lines
static char *lineptr[MAXLINES];

int readlines(char *lineptr[], int nlines);
int readlinebuf(char *buf, int lim);
void writelines(char *lineptr[], int nlines);

void myqsort(void *lineptr[], int left, int right, Comparator comp); 
// Compare numbers
int numcmp(const char *, const char *);
int rnumcmp(const char *, const char *);
// Compare string case sensetive
int rstrcmp(const char *, const char *); // standard strcmp is used
// Compare string case insensetive
int rstrcasecmp(const char *, const char *);
// Compare chars, letters and spaces only
static const char* nextdirchar(const char *);
int dircmp(const char *, const char *);
int rdircmp(const char *, const char *);
int dircasecmp(const char *, const char *);
int rdircasecmp(const char *, const char *);
// Factory method to return a comparator
int getcomparator(Comparator *comp, int sortmode, int ordermode, int casemode);
int getcolumn(const char *s, int field, char *buf, int lim);
int maincomp(const char *, const char *);

// Main runner
int main(int argc, char *argv[]) 
{
    int nlines;
    int sortmode = MODE_STR;
    int ordermode = SORT_ASC;
    int casemode = CASE_SENS; 
    int fnum = 0;
    int state = STATE_SKIP;
    for (int i = 1; i < argc; i++) {
        if (*argv[i] == '+') {
            // Store the comparator
            if (state == STATE_STORE_FLAG) {
                if (getcomparator(&fields[nfields].comp, sortmode, ordermode, casemode) == 0) {
                    nfields++;
                } else {
                    printf("Can't define a comporator for the column %d\n", fields[nfields].number);
                }
            }
            if (nfields >= MAXFIELDS) { 
                printf("Max number of fields reached: %d. Other params are skipped\n", MAXFIELDS);
                break;
            }
            sortmode = MODE_STR;
            ordermode = SORT_ASC;
            casemode = CASE_SENS;
            fnum = atoi(++argv[i]);
            if (fnum < 0 || fnum >= MAXFIELDS) {
                printf("incorrect number of column -> skipped");
                state = STATE_SKIP;
            } else {
                state = STATE_STORE_FLAG;
                fields[nfields].number = fnum;
            }
        }
        // Skip definition of the sort flags
        if (state == STATE_SKIP) 
            continue;
        // If all goor -> accumulate flags to define a comporator
        if (strcmp(argv[i], "-n") == 0) 
            sortmode = MODE_NUM;
        else if (strcmp(argv[i], "-d") == 0) 
            sortmode = MODE_DIR; 
        else if (strcmp(argv[i], "-r") == 0)
            ordermode = SORT_DESC;
        else if (strcmp(argv[i], "-f") == 0)
            casemode  = CASE_INSENS;
    }
    // Process case for the last portion of fields
    if (state == STATE_STORE_FLAG) {
        if (getcomparator(&fields[nfields].comp, sortmode, ordermode, casemode) == 0)
            nfields++;
        else 
            printf("Can't define a comporator for the column %d\n", fields[nfields].number);
    }

    // Chech that we have at least one comporator
    if (nfields == 0) {
        printf("Zero comporators are defined. Stop the program");
        return 1;
    }

    Comparator func = (Comparator) maincomp;
    // Execute the quick sort
    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        myqsort((void **) lineptr, 0, nlines - 1, func);
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("input too big to sort");
        return 1;
    }
}

// Sorting and comparison section
int getcomparator(Comparator *comp, int sortmode, int ordermode, int casemode) 
{
    if (sortmode == MODE_NUM) {
        *comp = (Comparator) (ordermode == SORT_DESC ? rnumcmp : numcmp);
        return 0;
    } else if (sortmode == MODE_STR) {
        if (casemode == CASE_INSENS) 
            *comp = (Comparator) (ordermode == SORT_DESC ? rstrcasecmp : strcasecmp);
        else 
            *comp = (Comparator) (ordermode == SORT_DESC ? rstrcmp : strcmp);
        return 0;
    } else if (sortmode == MODE_DIR) {
        if (casemode == CASE_INSENS) 
            *comp = (Comparator) (ordermode == SORT_DESC ? rdircasecmp : dircasecmp);
        else 
            *comp = (Comparator) (ordermode == SORT_DESC ? rdircmp : dircmp);
        return 0;
    }
    return -1;
}

// Main comparator for sorting
int getcolumn(const char *s, int field, char *buf, int lim)
{
    if (field < 0) 
        return -1;
    while (*s && field) {
        // skip the word
        while(*s && !isspace((unsigned char)*s)) s++;
        // skip the space
        while(*s && isspace((unsigned char)*s)) s++;
        field--;
    }
    int i = 0;
    while (*s && !isspace((unsigned char)*s) && i < lim - 1) 
        buf[i++] = *s++;
    buf[i] = '\0';
    return i;
}

int maincomp(const char *a, const char *b) 
{
    int len1 = 0;
    int len2 = 0;
    int result = 0;
    char buf1[BUF_LEN], buf2[BUF_LEN];
    for (int i= 0; i < nfields; i++) {
        len1 = getcolumn(a, fields[i].number, buf1, BUF_LEN);
        len2 = getcolumn(b, fields[i].number, buf2, BUF_LEN);
        if (len1 <= 0)
            buf1[0] = '\0';
        if (len2 <= 0)
            buf2[0] = '\0';
        result = fields[i].comp(buf1, buf2);
        if (result != 0)
            return result;
    }
    return 0;
}

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

int rstrcasecmp(const char *a, const char *b) 
{
    return -strcasecmp(a, b);
}

static const char* nextdirchar(const char *s) 
{
    unsigned char c;
    while(*s) {
        c = *s;
        if (isalnum(c) || c == ' ') 
            break;
        s++;
    }
    return s;
}

int dircmp(const char *a, const char *b) 
{
    unsigned char c1, c2;
    while(1) {
        a = nextdirchar(a);
        b = nextdirchar(b);
        c1 = *a;
        c2 = *b;
        if (c1 != c2)
            return c1 - c2; 
        if (c1 == '\0')
            return 0;
        a++;
        b++;
    }
}

int rdircmp(const char *a, const char *b) 
{
    return -dircmp(a, b);
}

int dircasecmp(const char *a, const char *b)
{
    unsigned char c1, c2;
    while(1) {
        a = nextdirchar(a);
        b = nextdirchar(b);
        c1 = tolower((unsigned char) *a);
        c2 = tolower((unsigned char) *b);
        if (c1 != c2)
            return c1 - c2;
        if (c1 == '\0')
            return 0;
        a++;
        b++;
    }
}

int rdircasecmp(const char *a, const char *b) 
{
    return -dircasecmp(a, b);
}

// Section to work with lines


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
        ptr = malloc(sizeof(char) * (len + 1));
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
