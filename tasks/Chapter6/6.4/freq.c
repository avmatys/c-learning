#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100
#define MAXNODECOUNT 10000

typedef struct Node_t {
    char *word;
    int count;
    struct Node_t *left;
    struct Node_t *right;
} Node_t;

Node_t* addtree(Node_t *, char*);
void printtree(Node_t *);

char *mystrdup(char *);

static Node_t* nodes[MAXNODECOUNT];
static int nodeptr = 0;

void qsortn(Node_t *nodes[], int left, int right);
void swapn(Node_t *nodes[], int i, int j);

int getword(char *, int);

#define CHBUFSIZE 100
static int chbuf[CHBUFSIZE];
int chbufp = 0;

int getch(void);
void ungetch(int);

int main(int argc, char *argv[]) 
{
    int n;
	char word[MAXWORD];
    word[0] = '\0';

    // To avoid search by the same word -> use binary tree
    Node_t* root = NULL;
	while (getword(word, MAXWORD) != EOF) 
        if (isalpha((unsigned char) word[0])) 
            root = addtree(root, word);
    
    // Sort and show
    qsortn(nodes, 0, nodeptr - 1);
    for (int i = 0; i < nodeptr; i++) 
        printf("Word: %s Frequency: %d\n", nodes[i]->word, nodes[i]->count);
	return 0;
}

// Binary tree
Node_t *addtree(Node_t *p, char *w) 
{
    int cond;
    if (p == NULL) {
        p = malloc(sizeof(Node_t));
        p->left = NULL;
        p->right = NULL;
        p->word = mystrdup(w);
        p->count = 1;
        nodes[nodeptr++] = p;
    } else if ((cond = strcasecmp(w, p->word)) == 0) {
        p->count++;
    } else if (cond < 0) {
        p->left = addtree(p->left, w);
    } else {
        p->right = addtree(p->right, w);
    }
    return p;
}

void qsortn(Node_t *nodes[], int left, int right) 
{
    int i, last;
    if (left >= right)
        return;
    swapn(nodes, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++)  {
        if (nodes[i]->count > nodes[left]->count) {
            swapn(nodes, i, ++last);
        }
    }
    swapn(nodes, left, last);
    qsortn(nodes, left, last - 1);
    qsortn(nodes, last + 1, right);
}

void swapn(Node_t *nodes[], int i, int j) 
{
    Node_t *tmp = nodes[i];
    nodes[i] = nodes[j];
    nodes[j] = tmp;
}

char *mystrdup(char *w) 
{
    if (w == NULL) 
        return NULL;
    char *new = malloc(strlen(w) + 1);
    if (new == NULL)
        return NULL;
    strcpy(new, w);
    return new;
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
