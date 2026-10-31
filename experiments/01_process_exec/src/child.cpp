#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
    cout << "【child】子进程程序开始执行，PID: " << getpid() << endl;
    cout << "【child】子进程程序执行完毕，准备退出。" << endl;
    return 0;
}
