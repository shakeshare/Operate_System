#include "csfifo.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[])
{
    // 根据题目所给框架
    const char *clientfifo = "/tmp/cfifo";
    int fd;
    char buf[BUFSIZ]={0};
    int nread;

    // 这里假设服务器从约定好的 SERVER_FIFO 读取数据
    if (mkfifo(SERVER_FIFO, 0666) < 0 && errno != EEXIST)
    {
        cerr << "创建服务器端命名管道失败" << endl;
        return -1;
    }

    while (1)
    {
        if (access(clientfifo, F_OK) == -1)
        {
            // 检查客户端管道是否创建
            cout << "客户端尚未创建命名管道" << endl;
            sleep(1);
            continue;
        }

        // 以只读方式打开命名管道
        fd = open(SERVER_FIFO, O_RDONLY);
        if (fd < 0)
        {
            cout << "进程无法从命名管道读取数据" << endl;
            return -1;
        }
        else
        {
            nread = read(fd, buf, sizeof(buf));
            // 输出从管道获得的数据
            cout << "从管道获得的数据: " << buf << endl;

            // 关闭管道
            close(fd);

            // 统计读取的字符个数
            int count = strlen(buf);

            // 分配一个长度等于字符总数的字符串空间
            char *msg = (char *)malloc(count + 1);

            // 将读取的字符串写入分配的字符串空间
            strcpy(msg, buf);

            // 将新分配字符串空间字符(这里提取前面的ID部分)转换成数字
            int num = atoi(msg);
            if (num == 0)
            {
                cout << "未从客户端获取到数据" << endl;
                free(msg);
                return -1;
            }

            // 输出转换后的数字
            cout << "转换后的数字: " << num << endl;

            // 将命名管道名及接收到的字符数写入客户端管道，发送给客户端 (注意题目原文写发送给服务器应为笔误)
            int fd_client = open(clientfifo, O_WRONLY);
            if (fd_client != -1)
            {
                string reply = "FIFO: " + string(SERVER_FIFO) + ", 接收到的字符数: " + to_string(count);
                write(fd_client, reply.c_str(), reply.length());
                close(fd_client);
            }

            free(msg);
        }
    }
    return 0;
}
