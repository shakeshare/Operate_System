#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// 信号处理函数
void signal_handler(int sig) {
    if (sig == SIGUSR1) {
        std::cout << "父进程: 接收到了子进程发送的信号 (SIGUSR1)。\n";
    }
}

int main() {
    // 注册信号处理函数，让父进程能够捕获 SIGUSR1 信号
    signal(SIGUSR1, signal_handler);

    std::cout << "程序开始运行...\n";

    // 创建子进程
    pid_t pid = fork();

    if (pid < 0) {
        // 创建进程失败
        std::cerr << "创建子进程失败！\n";
        return 1;
    } else if (pid == 0) {
        // 这是子进程
        std::cout << "子进程: 正在运行，准备向父进程发送信号...\n";
        sleep(1); // 暂停1秒，模拟某些工作并确保父进程已准备好接收信号
        
        // 向父进程发送 SIGUSR1 信号
        kill(getppid(), SIGUSR1);
        
        std::cout << "子进程: 信号已发送，子进程退出。\n";
    } else {
        // 这是父进程
        std::cout << "父进程: 正在等待子进程发送信号...\n";
        
        // 等待子进程执行完毕
        int status;
        waitpid(pid, &status, 0); 
        
        std::cout << "父进程: 子进程已退出，父进程程序结束。\n";
    }

    return 0;
}
