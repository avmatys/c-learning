#include <stdio.h>
#include <stdlib.h>

unsigned getbits(unsigned x, int p, int n) 
{
    return (x >> (p+1-n)) & ~(~0 << n);    
}

unsigned setbits(unsigned x, int p, int n, unsigned y) 
{
    unsigned shift = p + 1 - n;
    unsigned y_mask = getbits(y, n - 1, n) << shift;
    unsigned x_mask = ~((~(~0 << n)) << shift);
    return (x & x_mask) | y_mask;
}

void run_test(int id, unsigned x, int p, int n, unsigned y, unsigned expected) 
{
    unsigned result = setbits(x, p, n, y);
    if (result == expected) {
        printf("[PASS] Test TC-%02d\n", id);
    } else {
        printf("[FAIL] Test TC-%02d: Expected %u, Got %u\n", id, result, expected);
    }
}

int main() {
    // Positive
    run_test(1, 255, 4, 3, 2, 235);
    run_test(2, 240, 2, 3, 7, 247);
    run_test(3, 0, 5, 2, 15, 48);
    // Boundary
    run_test(4, 0, 3, 1, 1, 8);
    run_test(5, 0, 31, 4, 15, 4026531840);
    return 0;
}
