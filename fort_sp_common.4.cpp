#include <omp.h>

// 模拟 Fortran COMMON /C/ X,Y
struct CommonBlock {
    float X;
    float Y;
};

CommonBlock C;

void common_wrong() {
    // 错误：C 被声明为 private，但 C 的成员 X 又声明为 shared
    // 这是矛盾的 — X 不能同时是 private（属于私有 C）和 shared
    // Fortran 原文：!$OMP PARALLEL PRIVATE(/C/), SHARED(X)

    // C++ 中同样非法，以下写法在概念上等同于：
    // #pragma omp parallel private(C) shared(C.X)   // 矛盾！

    // 这段代码无法编译通过，仅作错误示范
}
