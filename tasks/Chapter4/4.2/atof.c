#include <stdio.h>
#include <ctype.h>

double atof(char s[]) 
{
    double value, power, expmult;
    int i, sign;

    // Skip spaces
    i = 0;
    while(isspace(s[i])) i++;

    // Define sign
    sign = s[i] == '-' ? -1 : 1;
    if(s[i] == '-' || s[i] == '+') i++;

    // Extract left to the dot
    value = 0.0;
    while(isdigit(s[i]))
        value = 10.0 * value + (s[i++] - '0');

    // Extract right to the dot
    if (s[i] == '.') i++;
    power = 1.0;
    while(isdigit(s[i])) {
        value = 10.0 * value + (s[i++] -'0');
        power *= 10.0;
    }

    // Check if the exponent representation
    expmult = 1.0;
    if (s[i] == 'e' || s[i] == 'E') {
        int expsign, exp;
        
        // Skip the letter
        i++; 

        // Define the exponent sign
        expsign = s[i] == '-' ? -1 : 1;
        if (s[i] == '-' || s[i] == '+') i++;

        // Extract the exp
        exp = 0;
        while(isdigit(s[i])) 
            exp = 10 * exp + (s[i++] - '0');

        // Calculate multiply
        for (; exp > 0; exp--)
            expmult *= 10.0;
        if (expsign == -1) 
            expmult = 1 / expmult;
    }
    return sign * value * expmult / power;
}

int main()
{
    char* s1 = "123";
    printf("String: %s Number: %f\n", s1, atof(s1));
    char* s2 = "123.123";
    printf("String: %s Number: %f\n", s2, atof(s2));
    char* s3 = "123.123e10";
    printf("String: %s Number: %f\n", s3, atof(s3));
    char* s4 = "123.123e-1";
    printf("String: %s Number: %f\n", s4, atof(s4));
    return 0;
}

