#include <omp.h>

// 模拟 Fortran COMMON /C/ X,Y
struct CommonBlock {
    float X;
    float Y;
};

CommonBlock C;

void common_wrong2() {
    // 错误：同一个 COMMON 块 /C/ 不能同时被声明为 private 和 shared
    // Fortran 原文：!$OMP PARALLEL PRIVATE(/C/), SHARED(/C/)

    // C++ 中同样非法，以下写法自相矛盾：
    // #pragma omp parallel private(C) shared(C)      // 矛盾！

    // 这段代码无法编译通过，仅作错误示范
}
