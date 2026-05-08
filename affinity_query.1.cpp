#include <iostream>
#include <omp.h>

// 子函数：负责在特定的“地点”（Socket）内进行初始化
void socket_init(int socket_num) {
    // 关键查询：获取当前地点（socket_num）包含的核心数量
    int n_procs = omp_get_place_num_procs(socket_num);

    // 在插槽内部开启并行，线程数等于该插槽的核心数
    // 使用 close 策略：让线程在插槽内部尽量挨着，共享 L3 缓存
    #pragma omp parallel num_threads(n_procs) proc_bind(close)
    {
        printf("来自插槽 %d 的报道，本地线程号: %d\n", 
               socket_num, omp_get_thread_num());
    }
}

int main() {
    // 1. 环境准备
    omp_set_nested(1);                // 允许嵌套
    omp_set_max_active_levels(2);     // 最多允许两层并行

    // 2. 查询系统中有多少个“地点”（Places/Sockets）
    int n_sockets = omp_get_num_places();

    // 3. 第一层并行：每个插槽分配一个“领导线程”
    // 使用 spread 策略：确保领导线程们分别去往不同的插槽
    #pragma omp parallel num_threads(n_sockets) proc_bind(spread)
    {
        // 获取当前“领导线程”所在的地点编号
        int socket_num = omp_get_place_num();
        
        // 进入该插槽进行具体工作
        socket_init(socket_num);
    }

    return 0;
}