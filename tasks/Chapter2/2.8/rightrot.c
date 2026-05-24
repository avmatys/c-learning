#include <stdio.h>
#include <stdlib.h>

unsigned rightrot(unsigned x, int n) 
{
    unsigned cbits = sizeof(unsigned) * 8;
    n = n & (cbits - 1);
    if (n == 0) return x;
    return (x >> n) | (x << (cbits - n));
}

void run_test(int id, unsigned x, int n, unsigned expected) 
{
    unsigned result = rightrot(x, n);
    if (result == expected) {
        printf("[PASS] Test TC-%02d\n", id);
    } else {
        printf("[FAIL] Test TC-%02d: Expected %u, Got %u\n", id, result, expected);
    }
}

int main() {
    run_test(1, 1, 1, 2147483648U);
    run_test(2, 7, 2, 3221225473U);
    run_test(3, 2863311530U, 2, 2863311530U);

    run_test(4, 12345, 0, 12345);
    run_test(5, 12345, 32, 12345);
    run_test(6, 1, 31, 2);

    run_test(7, 4294967295U, 15, 4294967295U);
    run_test(8, 0, 9, 0);
    return 0;
}
