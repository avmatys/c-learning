#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };
enum { OK, ERROR };

int dcl(void);
int dirdcl(void);

int gettoken(void);
int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];

#define BUFSIZE 100

int getch(void);
void ungetch(int);
void skipline();

int main() 
{
    int result = 0;
    while (gettoken() != EOF) {
        strcpy(datatype, token);
        out[0] = '\0';
        result = dcl();
        if (result == OK && tokentype == '\n') {
            printf("%s: %s %s\n", name, out, datatype);
        } else {
            if (tokentype != '\n')
                printf("syntax error\n");
            skipline();
        }
    }
    return 0;
}

int dcl(void)
{
    int ns;
    int result;
    for (ns = 0; gettoken() == '*'; )
        ns++;
    result = dirdcl();
    if (result != OK) 
        return result; // bubble up the error
    while (ns-- > 0) 
        strcat(out, " pointer to ");
    return OK;
}

int dirdcl(void) 
{
    int type;
    int result;
    if (tokentype == '(') {
        result = dcl();
        if (result != OK)
            return result;
        if (tokentype != ')') {
            printf("error: missing )\n");
            return ERROR;
        }
    } else if (tokentype == NAME)
        strcpy(name, token);
    else { 
        printf("error: expected name of (dcl)\n");
        return ERROR;
    }
    
    while ((type = gettoken()) == PARENS || type == BRACKETS) {
        if (type == PARENS)
            strcat(out, " function returning");
        else {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
    }
    return OK;
}

int gettoken(void) 
{
    int c;
    char *p = token;
    while ((c = getch()) == ' ' || c == '\t') ;
    // Define a token type and get it
    if (c == '(') {
        if ((c = getch()) == ')') {
            strcpy(token, "()");
            return tokentype = PARENS;
        } else {
            ungetch(c);
            return tokentype = '(';
        }
    } else if (c == '[') {
        for (*p++ = c; (*p++ = getch()) != ']'; ) ;
        *p = '\0';
        return tokentype = BRACKETS;
    } else if (isalpha(c)) {
        for (*p++ = c; isalnum(c = getch()); )
            *p++ = c;
        *p = '\0';
        ungetch(c);
        return tokentype = NAME;
    } else {
        return tokentype = c;
    }
}

static int buf[BUFSIZE];
static int bufp = 0;

int getch()
{
    return bufp > 0 ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
    if (bufp >= BUFSIZE) {
        printf("ungetch failed -> buffer is full\n");
    } else {
        buf[bufp++] = c;
    }
}

void skipline()
{
    int c;
    while((c = getch()) != EOF && c != '\n') ;
    if (c == EOF) 
        ungetch(c); // return back for other functions to finish properly
}

