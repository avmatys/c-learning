#include <stdio.h>

int lowerbound(int x, int v[], int n) 
{
    int left, mid, right, result;
    left = 0;
    right = n - 1;
    result = n;
    while (left <= right) {
        mid = left + (right - left) / 2;
        if (v[mid] >= x) {
            result = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return result;
}

int main()
{
    int test[7] = {1, 3, 3, 3, 9, 11, 13};

	printf("%d\n", lowerbound(0, test, 7));
	printf("%d\n", lowerbound(3, test, 7));
	printf("%d\n", lowerbound(4, test, 7));
	printf("%d\n", lowerbound(2, test, 7));
	printf("%d\n", lowerbound(15, test, 7));

    return 0;
}
