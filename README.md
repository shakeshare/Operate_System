# Linux 操作系统实验

每个实验按照“`src/` 源码、`bin/` 编译产物、`README.md` 说明”组织。

| 实验 | 内容 | 目录 |
| --- | --- | --- |
| 1 | 进程创建与 `exec` | [01_process_exec](experiments/01_process_exec/) |
| 2 | 线程共享进程数据 | [02_thread_shared_data](experiments/02_thread_shared_data/) |
| 3 | 匿名管道通信 | [03_anonymous_pipe](experiments/03_anonymous_pipe/) |
| 4 | 命名管道通信 | [04_named_pipe](experiments/04_named_pipe/) |
| 5 | 客户端/服务器 FIFO 通信 | [05_client_server_fifo](experiments/05_client_server_fifo/) |
| 6 | Linux 内核模块 | [06_kernel_module](experiments/06_kernel_module/) |

C++ 实验可以直接在 VS Code 中打开 `src/` 下的源文件，使用默认构建任务；生成的程序会放入对应实验的 `bin/`。
