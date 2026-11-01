# Deferred Work

本实验演示 Linux 内核中的 deferred work：内核线程提交普通 work，模块同时安排一次 delayed work。

## 目录

- `src/deferred_work.c`：内核模块源码
- `docs/2-stu-deferred_work.pdf`：实验资料
- `bin/`：预留的用户态编译产物目录；内核模块产物默认位于实验根目录

## 编译

需要已安装并匹配当前运行内核的内核头文件：

```bash
make
```

## 加载、查看日志和卸载

```bash
sudo insmod deferred_work.ko
dmesg | tail -n 20
sudo rmmod deferred_work
```

清理构建产物：

```bash
make clean
```