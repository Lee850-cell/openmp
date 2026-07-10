#include <iostream>
#include <omp.h>

// Fortran 原意：
//   INTEGER A(100), B(100)
//   EQUIVALENCE (A(51), B(1))
//   → A(51..100) 和 B(1..50) 共享同一块内存！
//   Fortran 数组列优先，内存布局：

//   A(1) A(2) ... A(50) [A(51)=B(1)] [A(52)=B(2)] ... [A(100)=B(50)]
//                         ↑── 重叠区域 ────────────────↑

// C++ 没有 EQUIVALENCE，用单个大数组模拟重叠内存：
//   A: 索引 0..99,  B: 索引 50..149 (B(1) 对齐 A(51))
//   即 A[i+50] 和 B[i] 是同一个位置 (i=0..49)

#define N 100
int mem[150];  // 大数组，A=mem[0..99], B=mem[50..149]
               // A[50] 和 B[0] 是同一个位置！

int main() {
    int i, j;
    int *A = &mem[0];    // A(1..100) = mem[0..99]
    int *B = &mem[50];   // B(1..50)  = mem[50..99], B(51..100) = mem[100..149]

    // DEFAULT(PRIVATE) + PRIVATE(I,J) + LASTPRIVATE(A)
    #pragma omp parallel for private(i, j, B) lastprivate(A)
    for (i = 0; i < N; i++) {
        // 循环1: B(J) = J - 1  (J=1..100)
        for (j = 0; j < N; j++) {
            B[j] = j;  // B(j) = j-1+1
        }

        // 循环2: A(J) = J  (J=1..100)
        for (j = 0; j < N; j++) {
            A[j] = j + 1;
            // 当 j>=50 时，A[50..99] 就是 B[0..49]
            // 覆盖了循环1写入的 B 值！
        }

        // 循环3: B(J) = B(J) + 1  (J=1..50)
        for (j = 0; j < 50; j++) {
            B[j] = B[j] + 1;
            // 这里读写的 B[0..49] 就是 A[50..99]
            // 又覆盖了循环2写入的 A 值！
        }
    }
    // LASTPRIVATE 把最后一次迭代的 A 拷贝出来
    // 但 A 在循环3中被 B 覆盖了 → 结果是未定义的

    // Fortran: PRINT *,B  → 也是未定义
    std::cout << "B is undefined due to LASTPRIVATE(A) corrupting overlap"
              << std::endl;
    return 0;
}
