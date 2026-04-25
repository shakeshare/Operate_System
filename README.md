This for linux course

## 实验2：线程共享进程数据

代码文件：`thread_share_data.cpp`

编译：

`g++ -std=c++11 thread_share_data.cpp -o thread_share_data -pthread`

运行：

`./thread_share_data`

## 实验3：信号通信

代码文件：`communication.cpp`

功能：父进程创建子进程后，子进程向父进程发送 `SIGUSR1` 信号；父进程收到信号后执行相应处理并回收子进程。

编译：

`g++ -std=c++11 communication.cpp -o communication`

运行：

`./communication`
