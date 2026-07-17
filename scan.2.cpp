#include <stdio.h>
#define N 100

int main(void)
{
    int a[N], b[N];
    int x = 0;

    for (int k = 0; k < N; k++)
        a[k] = k + 1;
    #pragma omp parallel for simd reduction(inscan, + : x)
    for (int k = 0; k < N; k++){
        b[k] =  x;
        #pragma omp scan exclusive(x)
        x += a[k];
    }

    printf("x = %d, b[0:3] = %d %d %d\n", x, b[0], b[1], b[2]);
    return 0;
}