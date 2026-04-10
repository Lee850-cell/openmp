#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <vector>

void nowait_example2(int n, float *a, float *b, float *c, float *y, float *z) {
    #pragma omp parallel num_threads(2)
    {
        int tid = omp_get_thread_num();

        #pragma omp for schedule(static) nowait
        for (int i = 0; i < n; i++) {
            c[i] = (a[i] + b[i]) / 2.0f;
            if (i == n-1) printf("線程 %d 完成了循環 1 (C)\n", tid);
        }

        #pragma omp for schedule(static) nowait
        for (int i = 0; i < n; i++) {
            z[i] = sqrtf(c[i]);
            if (i == n-1) printf("線程 %d 完成了循環 2 (Z)\n", tid);
        }

        #pragma omp for schedule(static) nowait
        for (int i = 0; i < n; i++) {
            y[i] = z[i] + a[i];
            if (i == n-1) printf("線程 %d 完成了循環 3 (Y)\n", tid);
        }
    }
}

int main() {
    int n = 1000000;
    std::vector<float> a(n, 1.0f), b(n, 2.0f), c(n), y(n), z(n);
    nowait_example2(n, a.data(), b.data(), c.data(), y.data(), z.data());
    return 0;
}