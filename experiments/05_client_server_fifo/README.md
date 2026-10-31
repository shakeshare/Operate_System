# 客户端/服务器 FIFO 通信

先启动服务器，再启动客户端：

```bash
g++ -std=c++11 src/server.cpp -o bin/server
g++ -std=c++11 src/client.cpp -o bin/client
./bin/server
./bin/client 1
```

程序使用 `/tmp/csfifo` 和客户端 FIFO 进行通信。