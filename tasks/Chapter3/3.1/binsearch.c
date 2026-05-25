#include <stdio.h>

int binsearch(int x, int v[], int n) 
{
    int left, mid, right;
    left = 0;
    right = n - 1;
    while (left < right) {
        mid = left + (right - left) / 2;
        if (x <= v[mid]) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    return v[left] == x ? left : -1;
}

int main()
{
    int test[7] = {1, 3, 5, 7, 9, 11, 13};

	printf("%d\n", binsearch(test[0], test, 7));
	printf("%d\n", binsearch(test[1], test, 7));
	printf("%d\n", binsearch(test[2], test, 7));
	printf("%d\n", binsearch(test[3], test, 7));
	printf("%d\n", binsearch(test[4], test, 7));
	printf("%d\n", binsearch(test[5], test, 7));
	printf("%d\n", binsearch(test[6], test, 7));
	printf("%d\n", binsearch(15, test, 7));

    return 0;
}
