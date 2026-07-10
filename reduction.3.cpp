#include <iostream>
#include <omp.h>

// Fortran 原意：
//   MAX = HUGE(0)          → 声明了一个叫 MAX 的变量
//   REDUCTION(MAX: M)      → 想用 MAX 做归约操作
//   但 MAX 已经不是内置的 max 函数了，被变量名覆盖了！
//   Fortran 中这不符合规范

// C++ 中 `max` 是 std::max，不像 Fortran 那样容易被变量名遮蔽
// 所以用 C++ 很难复现这个错误。这里仅作概念演示：

void sub(int &m, int i) {
    m = std::max(m, i);  // 正常使用 max 函数
}

int main() {
    // Fortran 的等价坑：
    // 如果 Fortran 中声明了变量 MAX，那 MAX 这个名字就不再指代内置函数
    // 以下展示正确的 C++ 写法（C++ 中不存在这个问题）

    int m = 0;

    #pragma omp parallel for reduction(max: m)
    for (int i = 1; i <= 100; i++) {
        m = std::max(m, i);  // reduction(max:) 正常工作
    }

    std::cout << "max m = " << m << "  (应该 = 100)" << std::endl;
    return 0;
}
