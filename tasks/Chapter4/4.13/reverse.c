#include <stdio.h>
#include <string.h>

void reverse(char s[]);
void reverse_i(char s[], int l, int r); 


void reverse(char s[])
{
    reverse_i(s, 0, strlen(s) - 1);
} 

void reverse_i(char s[], int l, int r)
{
    if (l < r){
        char tmp = s[l];
        s[l] = s[r];
        s[r] = tmp;
        reverse_i(s, l + 1, r - 1);
    }
}

int main() 
{
    char s[] = "Here am I!";
    printf("Initial string: %s\n", s);
    reverse(s);
    printf("Reversed string: %s\n", s);
    return 0;
}
