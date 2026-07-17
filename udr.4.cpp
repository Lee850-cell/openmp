#include <stdio.h>
#include <omp.h>

// Fortran: type dt { r1, r2 }
struct dt {
    float r1;
    float r2;
};

// Fortran: .add. 运算符 — 交叉加法
// xresult%r1 = x1%r1 + x2%r2   ← 用对方的 r2 加自己的 r1
// xresult%r2 = x1%r2 + x2%r1   ← 用对方的 r1 加自己的 r2
dt addc(const dt& x1, const dt& x2) {
    dt result;
    result.r1 = x1.r1 + x2.r2;
    result.r2 = x1.r2 + x2.r1;
    return result;
}

// Fortran: dt_init — 初始化归约私有副本
void dt_init(dt& priv) {
    priv.r1 = 0.0f;
    priv.r2 = 0.0f;
}

// Fortran: declare reduction(.add.:dt:...)
// 用函数重载 operator+ 模拟 Fortran 的 .add. 运算符
dt operator+(const dt& a, const dt& b) {
    return addc(a, b);  // 直接用交叉加法
}

#pragma omp declare reduction(cross_add : dt : \
    omp_out = addc(omp_out, omp_in)) \
    initializer(dt_init(omp_priv))

int main() {
    dt xdt1 = {1.0f, 2.0f};
    dt xdt2 = {2.0f, 3.0f};  // xdt2 不变，每次迭代和它交叉

    printf("Initial: xdt1 = (%.1f, %.1f), xdt2 = (%.1f, %.1f)\n",
           xdt1.r1, xdt1.r2, xdt2.r1, xdt2.r2);
    printf("Custom add: r1' = r1 + r2_other, r2' = r2 + r1_other\n\n");

    // 串行验证
    dt serial = {1.0f, 2.0f};
    for (int i = 1; i <= 10; i++) {
        serial = addc(serial, xdt2);
    }
    printf("Serial result  : (%.1f, %.1f)\n", serial.r1, serial.r2);

    // 并行 UDR
    #pragma omp parallel for reduction(cross_add : xdt1)
    for (int i = 1; i <= 10; i++) {
        xdt1 = addc(xdt1, xdt2);
    }
    printf("Parallel result: (%.1f, %.1f)\n", xdt1.r1, xdt1.r2);

    return 0;
}
