# 命名管道通信

```bash
g++ -std=c++11 src/Ex_communication.cpp -o bin/Ex_communication
./bin/Ex_communication
```

程序使用 `/tmp/fifoserver` 进行进程间通信，运行结束后会删除该管道。