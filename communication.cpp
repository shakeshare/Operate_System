#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 80

int main() {
    int fd[2];

    /* 创建一个匿名管道 */
    if (pipe(fd) == -1) {
        std::cerr << "管道创建失败！\n";
        return 1;
    }

    pid_t pid = fork();

    /* 创建子进程成功 */
    if (pid < 0) {
        std::cerr << "进程创建失败！\n";
        return 1;
    } else if (pid == 0) {
        /* 关闭写端 */
        close(fd[1]);

        /* 休眠一段时间 */
        sleep(1);

        char buffer[MAX_LINE] = {0};
        /* 从管道读端读取数据并放入缓冲区 */
        ssize_t n = read(fd[0], buffer, MAX_LINE - 1);
        if (n > 0) {
            buffer[n] = '\0';
            /* 打印“子进程读取数据成功”提示信息，并输出缓冲区数据 */
            std::cout << "子进程读取数据成功: " << buffer << "\n";
        } else {
            std::cerr << "子进程读取数据失败！\n";
        }

        /* 关闭读端 */
        close(fd[0]);

        /* 退出 */
        _exit(0);
    } else {
        /* 关闭读端 */
        close(fd[0]);

        const char *msg = "Hello from parent process.";
        /* 向管道写端写入数据 */
        if (write(fd[1], msg, strlen(msg)) > 0) {
            /* 打印“父进程写管道成功” */
            std::cout << "父进程写管道成功\n";
        } else {
            std::cerr << "父进程写管道失败！\n";
        }

        /* 关闭写管道 */
        close(fd[1]);

        /* 打印“父进程关闭写管道成功”提示信息 */
        std::cout << "父进程关闭写管道成功\n";

        /* 休眠一段时间 */
        sleep(1);

        int status = 0;
        waitpid(pid, &status, 0);
    }

    return 0;
}
