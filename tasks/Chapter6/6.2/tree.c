#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100

char* types[] = { "boolean", "double", "float", "int" }; 

typedef struct Word_t {
    char *word;
    struct Word_t *left;
    struct Word_t *right;
    int count;
} Word_t;

Word_t *addword(Word_t *, char *);
void printword(Word_t *);

#define DEFAULT_PREFLEN 6
int prefixlen = DEFAULT_PREFLEN;

typedef struct Node_t {
    char *prefix;
    Word_t *words;
    struct Node_t *left;
    struct Node_t *right;
} Node_t;

Node_t* addtree(Node_t *, char* );
void printtree(Node_t *);

enum State { NORMAL = 0, STRING, SINGLECOMMENT, MULTICOMMENT, PREPROCESSOR };
int mode = NORMAL;
int prev;

void handlemode(char *);

int ntypes;

int getword(char *, int);
int binsearch(char *, char** , int);

#define CHBUFSIZE 100
static int chbuf[CHBUFSIZE];
int chbufp = 0;

int getch(void);
void ungetch(int);

int main(int argc, char *argv[]) 
{
    if (argc > 1 && *argv[1] == '-') {
        int tmp = atoi(argv[1] + 1);
        if (tmp > 0) 
           prefixlen = tmp; 
    }
    int n;
	char word[MAXWORD];
    char wordbuf[MAXWORD];
    word[0] = '\0';
    wordbuf[0] = '\0';
	ntypes = (sizeof types / sizeof(char *));
    mode = NORMAL;

    Node_t* root = NULL;

	while (getword(word, MAXWORD) != EOF) {
        handlemode(word);
        if (mode == NORMAL) {
            if (wordbuf[0] != '\0') {
                if (word[0] != '(')
                    root = addtree(root, wordbuf);
                wordbuf[0] = '\0';
            }
            if(isalpha(word[0]) && binsearch(word, types, ntypes) == -1) {
                strcpy(wordbuf, word);
            }
        } else {
            wordbuf[0] = '\0';
        }
    }
    if (mode == NORMAL && wordbuf[0] != '\0') 
        root = addtree(root, wordbuf);
    
    // Print the result groups
    printtree(root);
	return 0;
}

// Binary tree
Node_t *addtree(Node_t *p, char *w) 
{
    int cond;
    if (p == NULL) {
        p = (Node_t*) malloc(sizeof(Node_t));
        p->words = NULL;
        p->left = NULL;
        p->right = NULL;
        p->words = addword(p->words, w);
        p->prefix = strndup(w, prefixlen);
    } else if ((cond = strncmp(p->prefix, w, prefixlen)) == 0) {
        addword(p->words, w);
    } else if (cond > 0) {
        p->left = addtree(p->left, w);
    } else {
        p->right = addtree(p->right, w);
    }
    return p;
}

void printtree(Node_t *p) 
{
    if (p == NULL)
        return;
    printtree(p->left);
    printf("Group prefix: %s\n", p->prefix);
    printf("Words: ");
    printword(p->words);
    printf("\n\n");
    printtree(p->right);
}

Word_t *addword(Word_t *p, char *w)
{
    int cond;
    if (p == NULL) {
        p = (Word_t*) malloc(sizeof(Word_t));
        p->word = strdup(w);
        p->count = 1;
        p->left = NULL;
        p->right = NULL;
    } else if ((cond = strcmp(p->word, w)) == 0) {
        p->count++;
    } else if (cond > 0) {
        p->left = addword(p->left, w);
    } else {
        p->right = addword(p->right, w);
    }
    return p;
}

void printword(Word_t *p) 
{
    if (p == NULL)
        return;
    printword(p->left);
    printf("%s: %d ", p->word, p->count);
    printword(p->right);
}

int binsearch(char *word, char* words[], int n)
{
	int cond;
	int low, high, mid;

	low = 0;
	high = n - 1;
	while (low <= high) {
		mid = (low + high) / 2;
		if ((cond = strcmp(word, words[mid])) < 0)
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
    if (mode == NORMAL) {
        if (curr == '/' && prev == '/') {
            mode = SINGLECOMMENT;
        } else if (curr == '/' && prev == '*') {
            mode = MULTICOMMENT;
        } else if (curr == '"') {
            mode = STRING;
        } else if (curr == '#' && prev == '\n') {
            mode = PREPROCESSOR;
        }
    } else if (mode == STRING) {
        if (curr == '"' && prev != '\\') {
            mode = NORMAL;
        }
    } else if (mode == SINGLECOMMENT || mode == PREPROCESSOR) {
        if (curr == '\n') {
            mode = NORMAL;
        }
    } else if (mode == MULTICOMMENT) {
        if (curr == '/' && prev == '*') {
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
