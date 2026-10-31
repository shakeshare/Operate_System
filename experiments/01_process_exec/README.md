# 进程创建与 exec

```bash
g++ -std=c++11 src/child.cpp -o bin/child
g++ -std=c++11 src/parent.cpp -o bin/parent
./bin/parent
```

父进程通过 `fork` 创建子进程，子进程再使用 `execlp` 执行 `child`。