#include <omp.h>
#include <iostream>

double foo(int i, int j);

void work(int N, int M, double **A, double **B, double **C)
{
    int i, j;
    double alpha = 1.2;

    #pragma omp for collapse(2) ordered(2)
    for (i = 1; i < N-1; i++)
    {
        for (j = 1; j < M-1; j++)
        {
            A[i][j] = foo(i, j);
            #pragma omp ordered depend(source)

            B[i][j] = alpha * A[i][j];

            #pragma omp ordered depend(sink: i-1,j) depend(sink: i,j-1)
            C[i][j] = 0.2 * (A[i-1][j] + A[i+1][j] +
                             A[i][j-1] + A[i][j+1] + A[i][j]);
        }
    }
}

double foo(int i, int j)
{
    return i * 1.0 + j * 2.0;
}

int main()
{
    const int N = 100, M = 100;
    double **A = new double*[N];
    double **B = new double*[N];
    double **C = new double*[N];
    for (int i = 0; i < N; i++) {
        A[i] = new double[M]();
        B[i] = new double[M]();
        C[i] = new double[M]();
    }

    #pragma omp parallel
    {
        work(N, M, A, B, C);
    }

    std::cout << "A[50][50] = " << A[50][50]
              << ", B[50][50] = " << B[50][50]
              << ", C[50][50] = " << C[50][50] << std::endl;

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
