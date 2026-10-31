#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

using namespace std;

#define FIFO_SERVER "/tmp/fifoserver"
#define BUFFERSIZE 80

int main()
{
    // 创建命名管道
    if (mkfifo(FIFO_SERVER, 0666) < 0 && errno != EEXIST)
    {
        /*打印“无法创建命名管道”错误提示信息*/
        cerr << "无法创建命名管道" << endl;
        /*退出*/
        exit(1);
    }

    /*打印“成功创建命名管道”提示信息*/
    cout << "成功创建命名管道" << endl;

    /*创建子进程*/
    pid_t pid = fork();

    if (pid == 0)
    { // 子进程
        /*以写方式打开命名管道*/
        int fd = open(FIFO_SERVER, O_WRONLY);
        if (fd < 0)
        {
            /*打印“写数据出错”提示信息*/
            cerr << "写数据出错" << endl;
            /*退出*/
            exit(1);
        }

        // 准备待写入的数据
        const char *msg = "Hello from Child via FIFO!";
        write(fd, msg, strlen(msg) + 1);

        /*打印“成功写入数据”提示*/
        cout << "【子进程】成功写入数据" << endl;

        /*关闭命名管道*/
        close(fd);
    }
    else if (pid > 0)
    { // 父进程
        /*以只读方式打开命名管道*/
        int fd = open(FIFO_SERVER, O_RDONLY);
        if (fd < 0)
        {
            cerr << "只读方式打开失败" << endl;
            exit(1);
        }

        char buffer[BUFFERSIZE] = {0};

        /*输出读数据前缓冲区信息*/
        cout << "【父进程】读数据前缓冲区信息: [" << buffer << "]" << endl;

        /*从命名管道读出数据到缓冲区*/
        read(fd, buffer, BUFFERSIZE);

        /*输出读数据后缓冲区信息*/
        cout << "【父进程】读数据后缓冲区信息: [" << buffer << "]" << endl;

        /*关闭命名管道*/
        close(fd);

        // 用完后删除管道文件
        unlink(FIFO_SERVER);
    }
    else
    {
        /*打印“创建进程出错”提示信息*/
        cerr << "创建进程出错" << endl;
        /*退出*/
        exit(1);
    }

    return 0;
}