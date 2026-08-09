#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAXWORD 100

char *noize[] = { "a", "an", "and", "are", "for", "from", "in", "is", "it", 
                "not", "of", "on", "or", "that", "the", "this", "to", "was", "with" };

typedef struct List_t {
    int line;
    struct List_t *next;
} List_t;

List_t *addlist(List_t *, int); 
void printlist(List_t *);

typedef struct Node_t {
    char *word;
    List_t *head;
    List_t *tail;
    struct Node_t *left;
    struct Node_t *right;
} Node_t;

Node_t* addtree(Node_t *, char* , int);
void printtree(Node_t *);

int nnoize;

int getword(char *, int);
int binsearch(char *, char *[], int);

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
	nnoize = (sizeof noize / sizeof(noize[0]));
    int linenum = 0;

    Node_t* root = NULL;

	while (getword(word, MAXWORD) != EOF) {
        if (word[0] == '\n') {
            linenum++;
            continue;
        }
        if (isalpha((unsigned char) word[0]) && binsearch(word, noize, nnoize) == -1) {
            root = addtree(root, word, linenum);
        }
    }
    
    printtree(root);
	return 0;
}

// Binary tree
Node_t *addtree(Node_t *p, char *w, int line) 
{
    int cond;
    if (p == NULL) {
        p = malloc(sizeof(Node_t));
        p->head = NULL;
        p->tail = NULL;
        p->left = NULL;
        p->right = NULL;
        p->word = strdup(w);
        p->head = addlist(p->head, line);
        p->tail = p->head;
    } else if ((cond = strcasecmp(w, p->word)) == 0) {
        p->tail = addlist(p->tail, line);
    } else if (cond < 0) {
        p->left = addtree(p->left, w, line);
    } else {
        p->right = addtree(p->right, w, line);
    }
    return p;
}

void printtree(Node_t *p) 
{
    if (p == NULL)
        return;
    printtree(p->left);
    printf("Word: %s\n", p->word);
    printf("Words: ");
    printlist(p->head);
    printf("\n\n");
    printtree(p->right);
}

List_t *addlist(List_t *p, int l)
{
    List_t *new = (List_t*) malloc(sizeof(List_t));
    new->line = l;
    new->next = NULL;
    if (p != NULL)
        p->next = new;
    return new;
}

void printlist(List_t *p) 
{
    List_t *curr = p;
    while (curr != NULL) {
        printf("%d", curr->line);
        if (curr->next != NULL) 
            printf(",");
        curr = curr->next;
    }
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
