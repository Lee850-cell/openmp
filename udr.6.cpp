#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <omp.h>

// ① vector 逐元素加法：需要两个 vector 大小相同
#pragma omp declare reduction(vplus : std::vector<int> : \
    std::transform(omp_out.begin(), omp_out.end(), \
        omp_in.begin(), omp_out.begin(), std::plus<int>())) \
    initializer(omp_priv = omp_orig)

// ② vector 合并：把 B 追加到 A 末尾
#pragma omp declare reduction(merge : std::vector<int> : \
    omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))

// ③ list 合并：合并两个已排序的 list
#pragma omp declare reduction(merge : std::list<int> : \
    omp_out.merge(omp_in))

int main() {
    // === 例1: vector 逐元素加法 ===
    std::vector<int> a = {1, 2, 3, 4};
    std::vector<int> b = {10, 20, 30, 40};

    #pragma omp parallel for reduction(vplus : a)
    for (int i = 0; i < 10; i++) {
        // 每次迭代 a += b (逐元素)
        std::transform(a.begin(), a.end(), b.begin(),
                       a.begin(), std::plus<int>());
    }
    std::cout << "Vector +: ";
    for (int x : a) std::cout << x << " ";   // 101 202 303 404
    std::cout << std::endl;

    // === 例2: vector 合并 ===
    std::vector<int> va = {1, 2, 3};
    std::vector<int> vb = {10, 20};

    #pragma omp parallel for reduction(merge : va)
    for (int i = 0; i < 3; i++) {
        va.insert(va.end(), vb.begin(), vb.end());
    }
    std::cout << "Vector merge: ";
    for (int x : va) std::cout << x << " ";  // 1,2,3,10,20,10,20,10,20
    std::cout << std::endl;

    // === 例3: list 合并 ===
    std::list<int> la = {1, 3, 5};
    std::list<int> lb = {2, 4, 6};

    #pragma omp parallel for reduction(merge : la)
    for (int i = 0; i < 3; i++) {
        la.merge(lb);            // 合并已排序list
        lb = {2, 4, 6};          // 重置lb（merge吞掉了lb）
    }
    std::cout << "List merge: ";
    for (int x : la) std::cout << x << " ";  // 1,2,2,2,3,4,4,4,5,6,6,6
    std::cout << std::endl;

    return 0;
}
