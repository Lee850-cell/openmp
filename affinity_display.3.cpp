#include <iostream>
#include <vector>
#include <string>
#include <omp.h>

int main() {
    const int FORMAT_STORE = 80;
    const int BUFFER_STORE = 80;
    
    char default_format[FORMAT_STORE];
    // 自定义格式：%20H (主机名占20位), %0.4n (线程号补零占4位), %A (亲和性核心集)
    const char* my_format = "host=%20H thrd_num=%0.4n binds_to=%A";

    // --- 第一部分：获取并设置格式 ---

    // 获取系统默认的亲和性格式
    size_t nchars = omp_get_affinity_format(default_format, (size_t)FORMAT_STORE);
    std::cout << "默认亲和性格式: " << default_format << std::endl;

    // 设置我们自定义的格式
    omp_set_affinity_format(my_format);
    std::cout << "已设置为自定义格式: " << my_format << "\n" << std::endl;


    // --- 第二部分：捕获亲和性信息 ---

    int n = omp_get_num_procs();
    // 使用 vector 管理缓冲区，替代原始 C 的 malloc/free
    std::vector<std::string> buffer(n, std::string(BUFFER_STORE, '\0'));
    int max_req_store = 0;

    // 开启并行区域
    #pragma omp parallel reduction(max:max_req_store)
    {
        int thrd_num = omp_get_thread_num();
        if (omp_get_num_threads() <= n) {
            // 核心 API：将亲和性信息“抓”到 buffer 里
            size_t captured = omp_capture_affinity(&buffer[thrd_num][0], (size_t)BUFFER_STORE, NULL);
            
            // 记录最长的一个字符串长度，用于检查缓冲区是否够用
            max_req_store = (int)captured;
        }
    }

    // 打印刚才捕获到的结果
    for (int i = 0; i < n; i++) {
        std::cout << "线程 " << i << " 捕获的信息: " << buffer[i].c_str() << std::endl;
    }

    // 检查是否有截断现象
    if (max_req_store >= BUFFER_STORE) {
        std::cout << "\n警告: 缓冲区过小，建议增加 BUFFER_STORE 到 " << max_req_store + 1 << std::endl;
    }

    return 0;
}