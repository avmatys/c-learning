#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 10000

int mygetline(char *line, int max, FILE *fp);

int main(int argc, char *argv[])
{
    if (argc != 3){
        printf("Usage: %s file1 file2\n", argv[0]);
        exit(1);
    }
    FILE *fp1 = fopen(argv[1], "r");
    if (fp1 == NULL) {
        printf("Can't open a file: %s", argv[1]);
        exit(1);
    }
    FILE *fp2 = fopen(argv[2], "r");
    if (fp2 == NULL) {
        printf("Can't open a file: %s", argv[2]);
        exit(1);
    }
    char buf1[MAXLINE], buf2[MAXLINE];
    int res1, res2, line = 0;
    while (1) {
        line++;
        res1 = mygetline(buf1, MAXLINE, fp1);
        res2 = mygetline(buf2, MAXLINE, fp2);
        // Both files ended
        if (res1 == 0 && res2 == 0){
            printf("File %s equal to the file %s\n", argv[1], argv[2]);
            break;
        }
        // First file ended
        else if (res1 == 0 && res2 != 0) {
            printf("File %s is smaller than file %s\n", argv[1], argv[2]);
            break;
        }
        // Second file ended
        else if (res1 != 0 && res2 == 0) {
            printf("File %s is greater than file %s\n", argv[1], argv[2]);
            break;
        } 
        // Compare 2 lines
        else if(strcmp(buf1, buf2) != 0) {
            printf("File %s is not equal to the file %s on the line %d \n", argv[1], argv[2], line);
            printf("Line 1: %s", buf1);
            printf("Line 2: %s", buf2);
            break;
        }
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}

int mygetline(char *line, int max, FILE *fp)
{
    if (fgets(line, max, fp) == NULL) 
        return 0;
    else 
        return strlen(line);
}
