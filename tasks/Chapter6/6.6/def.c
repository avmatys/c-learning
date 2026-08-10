#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

#define MAXLINE 1000
#define MAXWORD 100

int mygetline(char *, int);

int main()
{
    char linebuf[MAXLINE];
    char namebuf[MAXWORD];
    char defnbuf[MAXWORD];
    linebuf[0] = '\0';
    namebuf[0] = '\0';
    defnbuf[0] = '\0';
    int len, i;
    while ((len = mygetline(linebuf, MAXLINE)) > 0) {
        for (int j = 0; j < len;) {
            // This is a define statement
            if (linebuf[j] == '#' && strncmp(&linebuf[j], "#define", 7) == 0) {
                j += 7;
                // Skip whitespace
                while (j < len && isspace((unsigned char) linebuf[j])) j++;
                // Get the name
                i = 0;
                while (j < len && isalnum((unsigned char) linebuf[j]) && i < MAXWORD - 1)
                    namebuf[i++] = linebuf[j++];
                namebuf[i] = '\0';
                // Skip whitespace
                while (j < len && isspace((unsigned char) linebuf[j])) j++;
                // Get the definition
                i = 0;
                while (j < len && !isspace((unsigned char) linebuf[j]) && i < MAXWORD - 1)
                    defnbuf[i++] = linebuf[j++];
                defnbuf[i] = '\0';
                // Store in the table
                install(namebuf, defnbuf);
                break;
            }
            // This can be a some idenitfier to change
            else if (isalpha((unsigned char) linebuf[j])) {
                i = 0;
                while (j < len && isalnum((unsigned char) linebuf[j]) && i < MAXWORD - 1) 
                    namebuf[i++] = linebuf[j++];
                namebuf[i] = '\0';
                List_t *np = lookup(namebuf);
                if (np != NULL) 
                    printf("%s", np->defn);
                else 
                    printf("%s", namebuf);
            }
            // Just print as is
            else {
                putchar(linebuf[j++]);
            }
        }
    }
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

int mygetline(char *buf, int len)
{
    int c;
    char *original = buf;
    while(--len > 0 && (c = getchar()) != EOF && c != '\n')
        *buf++ = c;
    //if (c == '\n')
    //   *buf++ = c;
    *buf = '\0';
    return buf - original;
}
