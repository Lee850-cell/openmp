#include <iostream>
#include <vector>
#include <numeric> // std::iota
#include <omp.h>

void iterator_example() {
    // 1. 初始化容器
    std::vector<int> vec(23);
    // 使用 std::iota 填充 0, 1, 2...
    std::iota(vec.begin(), vec.end(), 0); 

    // 2. 使用迭代器進行並行循環
    // 注意：在現代 C++ 中，我們直接在 for 循環裏定義 it
    #pragma omp parallel for default(none) shared(vec)
    for (auto it = vec.begin(); it < vec.end(); it++) {
        // 模擬工作：將每個元素翻倍
        *it = *it * 2;
        
        // 打印測試（注意：多線程打印會像之前一樣亂跳）
        // std::cout << "Thread " << omp_get_thread_num() << " processed " << *it << "\n";
    }

    // 3. 驗證結果
    std::cout << "Vector 最後一個元素: " << vec.back() << std::endl;
}

int main() {
    iterator_example();
    return 0;
}