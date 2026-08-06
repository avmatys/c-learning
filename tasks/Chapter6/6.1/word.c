#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100

typedef struct {
    char *word;
    int count;
} Key_t;

Key_t keytab[] = {
    "auto", 0,
    "break", 0,
    "case", 0,
    "char", 0,
    "const", 0,
    "continue", 0,
    "default", 0,
    "unsigned", 0,
    "void", 0,
    "volatile", 0,
    "while", 0,
};

enum State { NORMAL = 0, STRING, SINGLECOMMENT, MULTICOMMENT, PREPROCESSOR };
int mode = NORMAL;
int prev;

void handlemode(char *);

int nkeys;

int getword(char *, int);
int binsearch(char *, Key_t *, int);

#define CHBUFSIZE 100
static int chbuf[CHBUFSIZE];
int chbufp = 0;

int getch(void);
void ungetch(int);

int main() 
{
    int n;
	char word[MAXWORD];
	nkeys = (sizeof keytab / sizeof(Key_t));
    mode = NORMAL;

	while (getword(word, MAXWORD) != EOF) {
        // printf("%s\n", word);
        handlemode(word);
        if (mode == NORMAL && isalpha(word[0]))
			if ((n = binsearch(word, keytab, nkeys)) >= 0)
				keytab[n].count++;
    }
	for (n = 0; n < nkeys; n++)
		if (keytab[n].count > 0)
			printf("%4d %s\n", keytab[n].count, keytab[n].word);
	return 0;
}

int binsearch(char *word, Key_t tab[], int n)
{
	int cond;
	int low, high, mid;

	low = 0;
	high = n - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		if ((cond = strcmp(word, tab[mid].word)) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return mid;
	}
	return -1;
}

int getword(char *word, int lim)
{
    int c;
    char *w = word;

    while ((c = getch())  == '\t' || c == ' ')
       ; 
    if (c != EOF) 
        *w++ = c;
    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }
    for (; --lim > 0; w++) {
        if(!isalnum(*w = getch())) {
            ungetch(*w);
            break;
        }
    }
    *w = '\0';
    return word[0];
}

void handlemode(char *word) 
{
    char curr = *word;
    // Possible options
    // 1 single line comment
    // 2 multi line comment
    // 3 escape in the string
    if (curr == '/') {
        if (mode == NORMAL && prev == '/') {
            mode = SINGLECOMMENT; 
        } else if (mode == MULTICOMMENT && prev == '*') {
            mode = NORMAL;
        }
    } 
    else if (curr == '"') {
        if (mode == NORMAL && prev == '\'') {
            mode = STRING;
        } else if (mode == STRING && prev != '/') {
            mode = NORMAL; // string ended
        }
    }
    // Possible options
    // 1 multi line comment
    else if (curr == '*') {
        if (mode == NORMAL && prev == '/') {
            mode = MULTICOMMENT;
        }
    }
    // Possible options
    // 1 Preprocessor
    else if (curr == '#') {
        if (prev == '\n') {
            mode = PREPROCESSOR;
        }
    }
    // Possible optioins
    // 1 Go to normal mode
    // 2 Continue multiline comment
    else if (curr == '\n') {
        if (mode == SINGLECOMMENT || mode == PREPROCESSOR) {
            mode = NORMAL;
        }
    }
    prev = curr;
}

int getch() 
{
    if (chbufp > 0) {
        return chbuf[--chbufp];
    } else {
        return getchar();
    }
}

void ungetch(int c) 
{
    if (chbufp >= CHBUFSIZE) {
        printf("error: buffer is full\n");
    } else {
        chbuf[chbufp++] = c;
    }
}
