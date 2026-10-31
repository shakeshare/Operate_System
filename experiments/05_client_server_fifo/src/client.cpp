#include "csfifo.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    char r_msg[BUFSIZ] = {0};
    const char *temp = "client message to server";
    const char *cf = "/tmp/cfifo";
    char *s_msg;
    char *cfifo;
    char mesg[100];
    int fd, wn, fd1, fd2;

    memset(mesg, 0, 100);

    if (argc != 2)
    {
        cout << "Usage: ./client n" << endl;
        return 0;
    }

    s_msg = (char *)malloc(strlen(temp) + strlen(argv[1]) + 2); // +2 for '#' and '\0'
    cfifo = (char *)malloc(strlen(cf) + strlen(argv[1]) + 1);

    strcpy(s_msg, argv[1]);
    strcpy(cfifo, cf);
    s_msg[strlen(argv[1])] = '#';
    s_msg[strlen(argv[1]) + 1] = '\0'; // 确保字符串被正确截断以便拼接
    strcat(s_msg, temp);

    // 这里如果要求特定多个客户端可以用 cfifo 加上 ID。
    // 但是服务端代码里硬编码了 /tmp/cfifo 所以这里只使用宏或者 cf 的拷贝来做
    // 按题意拼接
    strcat(cfifo, argv[1]);

    // 创建命名管道csfifo (客户端专属的命名管道 cfifo)
    if (mkfifo(cfifo, 0666) < 0 && errno != EEXIST)
    {
        cerr << "无法创建命名管道" << endl;
        free(s_msg);
        free(cfifo);
        return -1;
    }

    // 创建一个软链接使服务端的/tmp/cfifo可以通过具体client FIFO去交互，或者直接使用 /tmp/cfifo
    // (由于服务端框架硬编码，此处统一用 /tmp/cfifo 代替专属 fifo 测试)
    if (mkfifo("/tmp/cfifo", 0666) < 0 && errno != EEXIST)
    {
        // ignore EEXIST
    }

    // 以写模式打开共享命名的 SERVER_FIFO
    fd = open(SERVER_FIFO, O_WRONLY);
    if (fd < 0)
    {
        cerr << "无法打开命名管道" << endl;
        free(s_msg);
        free(cfifo);
        return -1;
    }

    // 将s_msg写入管道
    write(fd, s_msg, strlen(s_msg));
    close(fd);

    while (1)
    {
        // 此处阻塞式打开管道（去掉原来的 O_NONBLOCK 和 sleep）
        // 这样客户端会在这里乖乖死等，直到服务器真正准备好并写入管道，杜绝时间差导致的数据丢失
        fd1 = open("/tmp/cfifo", O_RDONLY);
        if (fd1 < 0)
        {
            cout << "命名管道cfifo尚未创建或者打开失败，请稍后" << endl;
            sleep(1);
            continue;
        }

        // 从命名管道读取数据
        int n = read(fd1, r_msg, sizeof(r_msg));
        if (n > 0)
        {
            cout << "从服务器收到: " << r_msg << endl;
            close(fd1);
            break; // 收到消息后退出循环
        }
        else
        {
            close(fd1);
        }
    }

    free(s_msg);
    free(cfifo);
    return 0;
}
