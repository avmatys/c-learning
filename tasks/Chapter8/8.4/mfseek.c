#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define EOF (-1)
#define BUFSIZ 1024
#define OPEN_MAX 20

typedef struct _iobuf {
    int cnt;
    char *ptr;
    char *base;
    struct flags {
        unsigned int _READ : 1;
        unsigned int _WRITE : 1;
        unsigned int _UNBUF : 1;
        unsigned int _EOF : 1;
        unsigned int _ERR : 1;
    } flags;
    int fd;
} FILE;
extern FILE _iob[OPEN_MAX];

#define stdin (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

int _fillbuf(FILE *);
int _flushbuf(int, FILE *);
void error(char *msg);

#define feof(p) ((p)->flags._EOF != 0)
#define ferror(p) ((p)->flags._ERR != 0)
#define fileno(p) ((p)->fd)
#define getc(p) (--(p)->cnt >= 0 ? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define putc(x, p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : _flushbuf((x), p))
#define getchar() getc(stdin)
#define putchar(x) putc((x), stdout)
#define PERMS 0666 // RW for owner, group, others

FILE _iob[OPEN_MAX] = // stdin, stdout, stderr
{
    { 0, (char *) 0, (char *) 0, { ._READ = 1 }, 0 },
    { 0, (char *) 0, (char *) 0, { ._WRITE = 1 }, 1 },
    { 0, (char *) 0, (char *) 0, { ._WRITE = 1, ._UNBUF = 1 }, 2 }
};
FILE *fopen(char *name, char *mode);
int fflush(FILE *);
int fclose(FILE *);
int fseek(FILE *, long, int);

int main(int argc, char *argv[])
{
    char msg[1000];
    FILE *fpin, *fpout;
    if (argc == 3)
    {
        if ((fpin = fopen(argv[1], "r")) == NULL) // input 
            error(strcat(strcat(msg, "error: couldn't open file "), argv[1]));
        if ((fpout = fopen(argv[2], "w")) == NULL) // output 
            error(strcat(strcat(msg, "error: couldn't write to file "), argv[2]));
    }
    else
        error("usage: ./mfseek input_file output_file");

    if (fseek(fpin, -50, SEEK_END) == EOF)
        error("failed to seek to last char of input file");
    if (fseek(fpout, 15, SEEK_CUR) == EOF)
        error("failed to seek 15 bytes past the start/end of the output file");
    
    int c;
    for (int i = 0; i < 50; i++) {
        c = getc(fpin);
        if (c == EOF)
            break;
        putc(c, fpout);
    }

    if (fseek(fpin, 0, SEEK_SET) == EOF)
        error("failed to seek to start of input");
    if (fseek(fpout, 0, SEEK_SET) == EOF)
        error("failed to seek to start of output");
    putc(getc(fpin), fpout);

    fclose(fpin); 
    fclose(fpout); 
    exit(0);
}

FILE *fopen(char *name, char *mode) 
{
    int fd;
    FILE *fp;
    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;
    for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
        if (fp->flags._READ == 0 && fp->flags._WRITE == 0)
            break;
    if (fp >= _iob + OPEN_MAX)
        return NULL;
    
    if (*mode == 'w') 
        fd = creat(name, PERMS);
    else if (*mode == 'a') {
        if ((fd = open(name, O_WRONLY, 0)) == -1) 
            fd = creat(name, PERMS);
        lseek(fd, 0L, 2);
    } else 
        fd = open(name, O_RDONLY, 0);
    if (fd == -1)
        return NULL;
    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    fp->flags._WRITE = 0;
    fp->flags._READ = 0;
    fp->flags._UNBUF = 0;
    fp->flags._EOF = 0;
    fp->flags._ERR = 0;
    if (*mode == 'r')
        fp->flags._READ = 1;
    else 
        fp->flags._WRITE = 1;
    return fp;
}

int _fillbuf(FILE *fp)
{
    int bufsize;
    if (fp->flags._READ == 0 || fp->flags._EOF == 1 || fp->flags._ERR == 1)
        return EOF;
    bufsize = (fp->flags._UNBUF == 1) ? 1 : BUFSIZ;
    if (fp->base == NULL) 
        if ((fp->base = (char *) malloc(bufsize)) == NULL)
            return EOF; 
    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufsize);
    if (--fp->cnt < 0)
    {
        if (fp->cnt == -1)
            fp->flags._EOF = 1;
        else
            fp->flags._ERR = 1;
        fp->cnt = 0;
        return EOF;
    }
    return (unsigned char) *fp->ptr++;
}


int _flushbuf(int c, FILE *fp) 
{
    if (fp == NULL) 
        return EOF;
    else if (fflush(fp) == EOF) {
        return EOF;
    }
    *fp->ptr++ = (char) c;
    fp->cnt--;
    return 0;
}

int fflush(FILE *fp)
{
    if (fp == NULL) {
        int result = 0;
        for (fp = _iob; fp < _iob + OPEN_MAX; fp++) {
            if (fp->flags._WRITE == 1 && fflush(fp) == EOF)
                result = EOF;
        }
        return result;
    } else if (fp < _iob || fp >= _iob + OPEN_MAX) {
        return EOF;
    } else if (fp->flags._WRITE == 0 || fp->flags._READ == 1 || fp->flags._ERR == 1) {
        return EOF;
    }
    int bufsize = (fp->flags._UNBUF == 1) ? 1 : BUFSIZ;
    if (fp->base == NULL)  {
        if ((fp->base = (char *) malloc(bufsize)) == NULL)
            return EOF; 
    } else {
        int n = fp->ptr - fp->base;
        if (write(fp->fd, fp->base, n) != n) {
            fp->flags._ERR = 1;
            return EOF;
        }
    }
    fp->ptr = fp->base;
    fp->cnt = bufsize;
    return 0;
}

int fclose(FILE *fp)
{
    int result = 0;
    if (fp == NULL || fp < _iob || fp >= _iob + OPEN_MAX)
        return EOF;
    if (fp->flags._WRITE == 1) 
        result = fflush(fp);
    if (fp->base != NULL)
        free(fp->base);
    if (close(fp->fd) != 0)
        result = EOF;
    fp->fd = -1;
    fp->cnt = 0;
    fp->ptr = fp->base = NULL;
    fp->flags._WRITE = 0;
    fp->flags._READ = 0;
    fp->flags._UNBUF = 0;
    fp->flags._EOF = 0;
    fp->flags._ERR = 0;
    return result;
}

int fseek(FILE *fp, long offset, int origin)
{
    if (fp == NULL || fp < _iob || fp >= _iob + OPEN_MAX)
        return EOF;
    if (origin != SEEK_CUR && origin != SEEK_SET && origin != SEEK_END)
        return EOF;
    if (fp->flags._WRITE == fp->flags._READ)
        return EOF;
    if (origin == SEEK_SET && offset < 0)
        return EOF;
    if (fp->flags._WRITE) {
        // For write simple flush the current buffer
        if (fflush(fp) == EOF)
            return EOF;
    } else {
        // For read from current position we should check
        // remaining chars count in the buffer
        if (origin == SEEK_CUR) {
            if (offset >= 0 && offset <= fp->cnt) {
                fp->cnt -= offset;
                fp->ptr += offset;
                return 0;
            }
            // Out of the buffer
            offset -= fp->cnt;
        }
        fp->cnt = 0; // Set to 0 to override existing buffer
    }
    if (lseek(fp->fd, offset, origin) == -1) {
        fp->flags._ERR = 1; // set error flag
        return EOF;
    }
    fp->flags._EOF = 0;
    return 0;
}

void error(char *msg)
{
    fflush(NULL);  // flush all streams
    while (*msg != '\0')
        putc(*msg++, stderr); 
    putc('\n', stderr); 
    fflush(stderr);
    exit(1);
}

