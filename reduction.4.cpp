#include <iostream>
#include <omp.h>

// Fortran 原意：
//   MODULE M
//     INTRINSIC MAX           → 把内置 MAX 函数导出到模块 M
//   END MODULE M
//
//   USE M, REN => MAX         → 导入模块 M 的 MAX，改名叫 REN
//   REDUCTION(REN: N)         → 用 REN 做归约，效果等同于 MAX
//   N = MAX(N,I)              → 调用真正的 MAX 内置函数
//
// 这个技巧在 C++ 中不需要：
//   - C++ 用 std::max 带命名空间，不会被变量名遮蔽
//   - reduction(max: m) 中 max 是关键字，不是函数调用

int main() {
    int n = 0;

    // C++ 中直接写 reduction(max: n)，简洁且不会有名字冲突
    #pragma omp parallel for reduction(max: n)
    for (int i = 1; i <= 100; i++) {
        n = std::max(n, i);
    }

    std::cout << "max n = " << n << "  (应该 = 100)" << std::endl;
    return 0;
}
