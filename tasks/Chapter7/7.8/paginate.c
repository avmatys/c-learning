#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINESPERPAGE 30
#define LINEWIDTH 120

#define MAXLINE 1000

int mygetline(char *line, int max, FILE *fp);

void printfile(FILE *fp, char *name, int* linenumptr);
void printpageheader(char *name, int page);

int main(int argc, char *argv[])
{

    // Check that usage is correct
    if (argc < 2) {
        fprintf(stderr, "Correct usage: %s [file1] [file2]...[fileN]. At least one file is mandatory\n", argv[0]);
        exit(1);
    }

    FILE* fp;
    char line[MAXLINE];
    int linenum = 0;
    for (int i = 1; i < argc; i++) {
        fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "Can't open a file %s\n", argv[i]);
            continue;
        }
        printfile(fp, argv[i], &linenum);
        fclose(fp);
    }
    return 0;
}

int mygetline(char *line, int max, FILE *fp)
{
    if (fgets(line, max, fp) == NULL) 
        return 0;
    else 
        return strlen(line);
}

void printfile(FILE *fp, char *name, int *linenumptr)
{
    int page = 0;
    int linecount = 0;
    int len = 0;
    char line[LINEWIDTH];
    while ((len = mygetline(line, LINEWIDTH, fp)) > 0) {
       if (linecount + 1 == LINESPERPAGE) 
           linecount = 0;
       if (linecount == 0)
           printpageheader(name, ++page);
       linecount++;
       printf("%03d: %s", ++(*linenumptr), line); 
       if (line[len - 1] != '\n')
           putchar('\n');
    }
    while(linecount++ < LINESPERPAGE) {
       printf("%03d:\n", ++(*linenumptr)); 
    }
}

void printpageheader(char *name, int page)
{
    putchar('\n');
    for(int i = 0; i < LINEWIDTH; i++)
        putchar('#');
    printf("\nPage: %03d \n", page);
    printf("File: %s\n", name);
    for(int i = 0; i < LINEWIDTH; i++)
        putchar('#');
    putchar('\n');
}
