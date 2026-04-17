

## 編譯方法
```bash
<<<<<<< 
# 使用 Intel C++ 編譯器 (開啟 OpenMP 優化)
icpx -O3 -qopenmp jacobi25.cpp -o jacobi25
# 或者使用 MPI 編譯器 (如果包含通信邏輯)
mpicxx -O3 -fopenmp jacobi25.cpp -o jacobi25
>>>>>>> 
執行方法
Bash
<<<<<<< 
# 設置 OpenMP 線程數 (例如 8 線程)
export OMP_NUM_THREADS=8
# 執行程序 (MPI 環境下使用 10 個進程)
mpiexec -n 10 ./jacobi25
# 直接运行编译好的文件
./jacobi25
>>>>>>>