#include <iostream>
#include <cmath>
#include <omp.h>

// Fortran 原意（一个故意迷惑人的例子）：
//
//   MODULE MOD
//     INTRINSIC MAX, MIN          → 导入内置 MAX 和 MIN
//   END MODULE MOD
//
//   USE MOD, MIN=>MAX, MAX=>MIN   → 关键！名字对调：
//       MAX(内置) 改名叫 MIN
//       MIN(内置) 改名叫 MAX
//       现在 MIN 这个名字实际调用的是 MAX 函数！
//
//   REDUCTION(MIN: R)             → 表面上做 MIN 归约
//                                   实际上做 MAX 归约！
//   R = MIN(R, SIN(...))           → 表面上取 MIN
//                                   实际上取 MAX！
//
//   结果：代码全篇写着 MIN，最终求的是最大值
//   这是反面教材，不要在实际代码中这样用！

int main() {
    float r = -HUGE_VALF;

    // C++ 正经写法：清楚明白，是什么就写什么
    #pragma omp parallel for reduction(max: r)
    for (int i = 1; i <= 1000; i++) {
        r = std::fmax(r, std::sin(static_cast<float>(i)));
    }

    std::cout << "max(sin(i)) for i=1..1000 = " << r << std::endl;
    std::cout << std::endl;
    std::cout << "注意：Fortran 原例中所有 MIN 实际执行的是 MAX" << std::endl;
    std::cout << "因为 USE MOD, MIN=>MAX 把名字对调了" << std::endl;
    return 0;
}
