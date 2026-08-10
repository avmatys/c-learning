#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct List_t {
    struct List_t *next;
    char *name;
    char *defn;
} List_t;

#define HASHSIZE 101
List_t *hashtab[HASHSIZE];

unsigned hash(const char *);
List_t *lookup(const char *);
List_t *install(const char *, const char *);
int undef(const char *);

char* mystrdup(const char *);

int main()
{
    install("x", "1");
    install("y", "2");
    undef("x");
    return 0;
}

unsigned hash(const char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++) 
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

List_t *lookup(const char *s) 
{
    List_t *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np;
    return NULL;
}

List_t *install(const char *name, const char *defn) 
{
    List_t *np;
    unsigned hashval;
    if ((np = lookup(name)) == NULL) {
        np = (List_t *) malloc(sizeof(*np));
        if (np == NULL || (np->name = mystrdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else {
        // Free memory for the definition
        free((void *) np->defn); 
    }
    // Add new value
    if ((np->defn = mystrdup(defn)) == NULL)
        return NULL;
    return np;
}

int undef(const char *s)
{
    List_t *curr, *prev;
    unsigned hashval = hash(s);
    for (curr = prev = hashtab[hashval]; curr != NULL; prev = curr, curr = curr->next) {
        if (strcmp(s, curr->name) == 0) {
            if (curr == prev) {
                hashtab[hashval] = curr->next; // remove from the head
            } else {
                prev->next = curr->next;
            }
            free(curr->name);
            free(curr->defn);
            free(curr);
            return 0;
        }
    }
    return 1;
}

char *mystrdup(const char *s){
    char *copy = (char *) malloc(strlen(s) + 1);
    if (copy != NULL)
        strcpy(copy, s);
    return copy;
}

