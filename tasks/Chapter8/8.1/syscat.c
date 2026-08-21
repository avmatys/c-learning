#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdlib.h>

enum fileDescriptors { FDSTDIN, FDSTDOUT, FDSTDERR };

void filecopy(int fdin, int fdout);
void error(char *fmt, ...);

int main(int argc, char *argv[]) 
{
    if (argc == 1) {
        filecopy(FDSTDIN, FDSTDOUT);
    } else {
        int fd = -1;
        for (int i = 1; i < argc; i++) {
            fd = open(argv[i], O_RDONLY, 0);
            if (fd == -1) {
                error("Can't open a file %s", argv[i]);
                exit(1);
            }
            filecopy(fd, FDSTDOUT);
            close(fd);
        }
        
    }
}

void filecopy(int fdin, int fdout)
{
    int n;
    char buf[BUFSIZ];
    while((n = read(fdin, buf, BUFSIZ)) > 0) {
        if (write(fdout, buf, n) != n) 
            error("Unable to copy fdin to fdout");
    }
}

void error(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}
