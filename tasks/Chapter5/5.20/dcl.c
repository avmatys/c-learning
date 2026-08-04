/* Original code is stored here
https://github.com/codybartfast/knr/blob/master/ch5/ex-5-20-dcl-expanded.c#L39
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum { QUAL, TYPE, VAR, BRACKETS };
enum { OK = 0, ERROR };
enum { NO = 0, YES };

int declaration(char *dec, int isparam);
int dcl(char *name, char *out, int allowanon);
int dirdcl(char *name, char *out, int allowanon);
int args(char *out);

int gettoken(void);
void ungettoken(int ttype, char *token);

int skipspace(void);
int name(char*);
int brackets(char*);
int oparens(char*);
int contains(char **names, int count, char *name);

int getch(void);
void ungetch(int);
void skipline(void);

#define MAXTOKEN 100
int tokentype;
char token[MAXTOKEN];

#define BUFSIZE 100
static char buf[BUFSIZE];
static int bufp = 0;

#define MAXTOKENBUF 2
static int ttbuf[MAXTOKENBUF];
static char tkbuf[MAXTOKENBUF][MAXTOKEN];
static int ttbufp = 0;

char *types[] = { "void", "char", "int", "long", "short", "float", "double" };
int ntypes = 7;

char *quals[] = { "const" };
int nquals = 1;

#define MAXOUT 100000
#define MAXADD 100

int main() 
{
    char outbuf[MAXOUT];
    while (gettoken() != EOF) {
        if (declaration(outbuf, NO) != OK) 
            skipline();
    }
    return 0;
}

int declaration(char *dec, int isparam)
{
    char qual[MAXTOKEN + MAXADD];
    char name[MAXTOKEN];
    char type[MAXOUT];
    char out[MAXOUT];

    qual[0] = '\0';
    type[0] = '\0';

    if (tokentype == QUAL) {
        sprintf(qual, " %s", token);
        gettoken();
    }
    if (tokentype != TYPE) {
        printf("error: expected a type\n");
        return ERROR;
    }
    // Concat token modifiers
    do {
        if (type[0] != '\0')
            strcat(type, " ");
        strcat(type, token);
    } while(gettoken() == TYPE);
    ungettoken(tokentype, token);

    // Parse parts splitted by ,
    do {
        out[0] = '\0';
        if (dcl(name, out, isparam) != OK) {
            return ERROR;
        } else if (tokentype != ';' && tokentype != ',' && tokentype != ')') {
            printf("\nsyntax error, got %d\n", tokentype);
            return ERROR;
        } else {
            // The only valid case
            sprintf(dec, "%s:%s%s %s", name, qual, out, type);
            if (!isparam) 
                printf("\n%s\n", dec);
        }

    } while(tokentype == ',' && !isparam);
    return 0;
}

int dcl(char *name, char *out, int allowanon)
{
    int ns;
    int result;
    for (ns = 0; gettoken() == '*'; )
        ns++;
    result = dirdcl(name, out, allowanon);
    if (result != OK) 
        return result; // bubble up the error
    while (ns-- > 0) 
        strcat(out, " pointer to ");
    return OK;
}

int dirdcl(char *name, char *out, int allowanon) 
{
    int result;

    if (tokentype == VAR) {
        strcpy(name, token);
    } else if (tokentype == '(') {
        result = dcl(name, out, allowanon);
        if (result != OK)
            return result;
        if (tokentype != ')') {
            printf("error: missing )\n");
            return ERROR;
        }
    } else if (allowanon) {
        strcpy(name, "<unnamed>");
        allowanon = NO;
        ungettoken(tokentype, token);
    } else { 
        printf("error: expected name of (dcl)\n");
        return ERROR;
    }
    
    while (gettoken() == '(' || tokentype == BRACKETS) {
        if (tokentype == '(') {
            strcat(out, " function taking");
            result = args(out);
            if (result != OK) 
                return result;
        } else {
            strcat(out, " array");
            strcat(out, token);
            strcat(out, " of");
        }
    }
    return OK;
}

int args(char *out) 
{
    char dec[MAXOUT];
    int argcount = 0;
    char *sep = " argument ";
    int expectargs = YES;
    if (gettoken() == ')') {
        expectargs = NO;
    } else if (tokentype == TYPE && strcmp(token, "void") == 0){
        if (gettoken() == ')'){
            expectargs = NO;
        } else {
            ungettoken(tokentype, token);
            tokentype = TYPE;
            strcpy(token, "void");
        }
    }
    if (expectargs) {
        do {
            if (argcount++ > 0)
                gettoken();
            if (declaration(dec, YES) != OK) 
                return ERROR;
            strcat(out, sep);
            strcat(out, dec);
            sep = " and argument ";
        } while (tokentype == ',');
    }
    if (tokentype == ')') {
        if (argcount == 0) 
            strcat(out, " no arguments");    
        strcat(out, " returning");
    } else {
        printf("error: expected )");
        return ERROR;
    }
    return OK;
}

int gettoken(void)
{
	if (ttbufp > 0) {
		tokentype = ttbuf[--ttbufp];
		strcpy(token, tkbuf[ttbufp]);
	} else {
		skipspace();
		if (!(oparens(token) || brackets(token) || name(token))) {
			token[0] = tokentype = getch();
			token[1] = '\0';
		}
	}
	return tokentype;
}

void ungettoken(int ttype, char *tk)
{
	if (ttbufp >= MAXTOKENBUF) {
		printf("ungettoken: too many tokens\n");
	} else {
		ttbuf[ttbufp] = ttype;
		strcpy(tkbuf[ttbufp], tk);
		ttbufp++;
	}
}

int skipspace(void)
{
	char c;
	int result = NO;
	while (isspace(c = getch()))
		result = YES;
	ungetch(c);
	return result;
}

int name(char *p)
{
	char c, *tkn;
	int result = NO;
	tkn = p;
	if (isalpha(c = getch()) || c == '_') {
		result = YES;
		for (*p++ = c; isalnum(c = getch()) || c == '_';)
			*p++ = c;
		*p = '\0';
		if (contains(types, ntypes, tkn))
			tokentype = TYPE;
		else if (contains(quals, nquals, tkn))
			tokentype = QUAL;
		else
			tokentype = VAR;
	}
	ungetch(c);
	return result;
}

int brackets(char *p)
{
	char c;
	if ((c = getch()) == '[') {
		for (*p++ = c; (*p++ = (c = getch())) != ']';)
			;
		*p = '\0';
		tokentype = BRACKETS;
		return YES;
	}
	ungetch(c);
	return NO;
}

int oparens(char *p)
{
	char c;
	if ((c = getch()) == '(') {
		*p++ = tokentype = '(';
		*p = '\0';
		return YES;
	}
	ungetch(c);
	return NO;
}

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
    while((c = getch()) != ';' && c != EOF) ;
    if (c == EOF) 
        ungetch(c); // return back for other functions to finish properly
}

int contains(char **names, int count, char *name)
{
	int i;
	for (i = 0; i < count; i++)
		if (strcmp(name, names[i]) == 0)
			return YES;
	return NO;
}
