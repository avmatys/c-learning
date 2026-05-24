#include <stdio.h>
#include <stdlib.h>

unsigned invert(unsigned x, int p, int n) 
{
    unsigned shift = p + 1 - n;
    unsigned ones = (unsigned) ((1ULL << n) - 1);
    return x ^ (ones << shift);
}

void run_test(int id, unsigned x, int p, int n, unsigned expected) 
{
    unsigned result = invert(x, p, n);
    if (result == expected) {
        printf("[PASS] Test TC-%02d\n", id);
    } else {
        printf("[FAIL] Test TC-%02d: Expected %u, Got %u\n", id, result, expected);
    }
}

int main() {
    // Positive
    run_test(1, 255, 4, 3, 227);
    run_test(2, 170, 4, 3, 182);
    run_test(3, 240, 2, 3, 247);
    // Boundary 
    run_test(4, 0, 3, 1, 8);
    run_test(5, 0, 31, 32, 4294967295);
    run_test(6, 4294967295, 31, 32, 0);
    run_test(7, 0, 31, 4, 4026531840);
    return 0;
}
