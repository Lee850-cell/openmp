#include <iostream>
#include <omp.h>
using namespace std;

int x, y, z[1000];
#pragma omp threadprivate(x)

/*
 * ===== 原始 OpenMP 示例（含错误，无法编译）=====
 * default(none) 要求所有变量必须显式指定数据共享属性
 *
void default_none_err(int a) {
    const int c = 1;
    int i = 0;

    #pragma omp parallel default(none) private(a) shared(z, c)
    {
        int j = omp_get_num_threads();
        // OK - j 在区域内声明，自动private
        a = z[j];
        // OK - a 是 private, z 和 c 是 shared
        x = c;
        // OK - x 是 threadprivate
        z[i] = y;
*       // 错误! i 和 y 没有指定属性
*
*       #pragma omp for firstprivate(y)
*       // 错误! y 没有在 parallel 层指定属性
        for (i=0; i<10 ; i++) {
            z[i] = i;
        }
        z[i] = y;
*       // 错误! i 和 y 仍然没有指定
    }
}
*/

// ===== 修正版：补齐所有变量的属性声明 =====
void default_none(int a)
{
    const int c = 1;
    int i = 0;

    #pragma omp parallel default(none) \
        private(a) shared(z, c, y, i)  /* ← y 和 i 都补齐了 */
    {
        int j = omp_get_num_threads();
        /* j 在区域内声明 → 自动 private */

        a = z[j];
        x = c;           /* x 是 threadprivate */
        z[i] = y;        /* i, y 已声明为 shared */

        #pragma omp for firstprivate(y)  /* y 已在外层 parallel 中声明 */
        for (i = 0; i < 10; i++) {
            z[i] = i;    /* i 是循环变量 → 自动 private */
        }

        z[i] = y;
    }
}

int main()
{
    for (int k = 0; k < 10; k++) z[k] = 0;
    y = 100;

    default_none(42);

    cout << "After default_none:" << endl;
    cout << "x = " << x << " (threadprivate, master thread copy)" << endl;
    cout << "y = " << y << endl;
    cout << "z[0..9] = ";
    for (int k = 0; k < 10; k++) cout << z[k] << " ";
    cout << endl;

    return 0;
}
