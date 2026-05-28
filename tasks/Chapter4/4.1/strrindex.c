#include <stdio.h>
#include <string.h>

int strrindex(char s[], char t[])
{
    int slen, tlen, i, j, k;
    slen = strlen(s);
    tlen = strlen(t);
    for (i = slen - 1; i >= tlen - 1; i--) {
        for (j = i, k = tlen - 1; k >= 0 && s[j] == t[k]; j--, k--) ;
        if (k == -1) return i - tlen + 1;
    }
    return -1;
}

int main()
{
    char *s = "sssddddzzzzzz";
    int slen = strlen(s);
    for(int i = 0; i < slen; i++)
        printf("%c", s[i]);
    printf("\n");
    for(int i = 0; i < slen; i++)
        printf("%d", i);
    printf("\n\n");
    char *t = "zzz";
    printf("String: %s\nTarget: %s\nThe rightmost index: %d\n\n", s, t, strrindex(s, t));
    char *tt = "dddd";
    printf("String: %s\nTarget: %s\nThe rightmost index: %d\n\n", s, tt, strrindex(s, t));
    char *ttt = "sss";
    printf("String: %s\nTarget: %s\nThe rightmost index: %d\n\n", s, ttt, strrindex(s, t));
    return 0;
}
