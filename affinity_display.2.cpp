#include <stdio.h>
#include <omp.h>

void socket_work(int socket_num, int n_thrds);

int main(void) {
    int n_sockets, socket_num, n_thrds_on_socket;
    
    // 正确设置嵌套并行（只用这个）
    omp_set_max_active_levels(2);
    
    n_sockets = omp_get_num_places();
    n_thrds_on_socket = omp_get_place_num_procs(0);
    
    #pragma omp parallel num_threads(n_sockets) private(socket_num)
    {
        socket_num = omp_get_place_num();
        
        #pragma omp single
        printf("LEVEL 1: %d sockets\n", n_sockets);
        
        #pragma omp critical
        omp_display_affinity(NULL);
        
        socket_work(socket_num, n_thrds_on_socket);
    }
    
    return 0;
}

void socket_work(int socket_num, int n_thrds) {
    #pragma omp parallel num_threads(n_thrds)
    {
        #pragma omp single
        printf("\nLEVEL 2: %d threads on socket %d\n", n_thrds, socket_num);
        
        #pragma omp critical
        omp_display_affinity(NULL);
    }
}