# 匿名管道通信

```bash
g++ -std=c++11 src/communication.cpp -o bin/communication
./bin/communication
```

父进程通过匿名管道向子进程发送数据。