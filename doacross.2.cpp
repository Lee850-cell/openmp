#include <omp.h>
#include <iostream>

float foo(int i, int j);
float bar(float a, float b, float c);
float baz(float b);

void work(int N, int M,float **A, float **B, float **C){
    int i,j;
    
    #pragma omp for ordered(2)// 声明一个 ordered(2) 循环，表示该循环包含一个顺序区域，且依赖关系的距离为 2
    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            A[i][j] = foo(i, j);
            #pragma omp ordered depend(sink: i-1, j) depend(sink: i, j-1)
            B[i][j] = bar(A[i][j],
                (i > 0 ? B[i-1][j] : 0.0f),
                (j > 0 ? B[i][j-1] : 0.0f));
            #pragma omp ordered depend(source)
            C[i][j] = baz(B[i][j]);
        }
    }


}

float foo(int i, int j) {
    return i * 1.0f + j * 2.0f;
}

float bar(float a, float b, float c) {
    return a + b + c;
}

float baz(float b) {
    return b * 2.0f;
}
int main() {
    int N = 10, M = 10;
    float **A = new float*[N];
    float **B = new float*[N];
    float **C = new float*[N];
    for (int i = 0; i < N; i++) {
        A[i] = new float[M];
        B[i] = new float[M];
        C[i] = new float[M];
    }
    #pragma omp parallel
    {
        work(N, M, A, B, C);
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            std::cout << "A[" << i << "][" << j << "] = " << A[i][j]
                      << ", B[" << i << "][" << j << "] = " << B[i][j]
                      << ", C[" << i << "][" << j << "] = " << C[i][j] << std::endl;
        }
    }
    for (int i = 0; i < N; i++) {
        delete[] A[i];
        delete[] B[i];
        delete[] C[i];
    }
    delete[] A;
    delete[] B;
    delete[] C;
    return 0;   
}