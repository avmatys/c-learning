#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 10000

typedef struct In_t {
    FILE* fp;
    char *name;
    int isfile;
} In_t;

#define MAXIN 100

int mygetline(char *line, int max, FILE *fp);

char* mystrdup(char *line);

int main(int argc, char *argv[])
{
    In_t files[MAXIN];
    int fileptr = 0;
    char line[MAXLINE];
    char *pattern;
    int linenum = 0;
    int c, except = 0, number = 0;

    // Check that usage is correct
    if (argc < 2) {
        fprintf(stderr, "Correct usage: %s [-x] [-n] [file1] [file2] ... pattern. Pattern is mandatory\n", argv[0]);
        exit(1);
    }

    // Process flags and store files
    for (int i = 1; i < argc - 1; i++) {
        // Flags: -x or -n
        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "-x") == 0) {
                except = 1;
            } else if (strcmp(argv[i], "-n")  == 0) {
                number = 1;
            } else {
                fprintf(stderr, "Unsupported flag %s\n", argv[i]);
                exit(1);
            }
        } 
        // File name
        else {
            if (fileptr == MAXIN) {
                fprintf(stderr, "Can't add a file %s because buffer is full\n", argv[i]);
                exit(1);
            }
            if ((files[fileptr].fp = fopen(argv[i], "r")) == NULL) {
                fprintf(stderr, "Can't open a file %s\n", argv[i]);
                exit(1);
            }
            if ((files[fileptr].name = mystrdup(argv[i])) == NULL) {
                fprintf(stderr, "Can't copy a file name%s\n", argv[i]);
                exit(1);
            }
            files[fileptr].isfile = 1;
            fileptr++;
        }
    }

    // Get pattern
    if ((pattern = mystrdup(argv[argc - 1])) == NULL) {
        fprintf(stderr, "Can't extract a pattern\n");
        exit(1);
    }

    // Check if we have files
    // If no - use stdin
    if (fileptr == 0) {
        files[fileptr].fp = stdin;
        files[fileptr].isfile = 0;
        fileptr++;
    }

    // Check all input one by one
    for (int i = 0; i < fileptr; i++) {
        linenum = 0;
        while (mygetline(line, MAXLINE, files[i].fp) > 0) {
            linenum++;
            if ((strstr(line, pattern) != NULL) != except) {
                if (number) printf("%d: ", linenum);
                if (files[i].isfile) printf("filename: %s ", files[i].name);
                printf("%s", line);
            }
        }
    }

    // One by one close the file and clear the memory
    for (int i = 0; i < fileptr; i++) {
        if (files[i].isfile) {
            fclose(files[i].fp);
            free(files[i].name);
        }
    }
    free(pattern);
    return 0;
}

int mygetline(char *line, int max, FILE *fp)
{
    if (fgets(line, max, fp) == NULL) 
        return 0;
    else 
        return strlen(line);
}

char* mystrdup(char *s) 
{
    char* copy = (char*) malloc((strlen(s) + 1) * sizeof(char));
    if (copy != NULL) 
        strcpy(copy, s);
    return copy;
}
