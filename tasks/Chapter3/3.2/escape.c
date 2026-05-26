#include <stdio.h>
#include <stdbool.h>

#define MAX_LEN 1024

void escape(char *from, char *to) 
{
    char c;
    while((c = *from++) != '\0') {
        switch(c) {
            case '\t': 
                *to++ = '\\';
                *to++ = 't';
                break;
            case '\n' : 
                *to++ = '\\';
                *to++ = 'n';
                break;
            default:
                *to++ = c;
                break;
        }
    }
}

void unescape(char *from, char *to) 
{
    char c;
    bool slash;
    while((c = *from++) != '\0') {
        if (slash) {
            slash = false;
            switch(c) {
                case 'n' : 
                    *to++ = '\n';
                    break;
                case 't' : 
                    *to++ = '\t';
                    break;
                default:
                    *to++ = '\\';
                    *to++ = c;
                    break;
            }
        } else {
            switch(c) {
                case '\\' :
                    slash = true;
                    break;
                default: 
                    *to++ = c;
                    break;
            }
        }
    }
}

int main() 
{
    char *from = "This is \n a multiline \n string";
    char to[MAX_LEN];
    char to2[MAX_LEN];
    escape(from, to);
    printf("Old stirng: \n%s\nNew string: \n%s\n", from, to);
    unescape(to, to2);
    printf("Escaped stirng: \n%s\nUnescaped string: \n%s\n", to, to2);
    return 0;
}
