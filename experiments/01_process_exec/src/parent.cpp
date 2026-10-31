#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main() {
    cout << "【父进程】开始执行，准备调用 fork 创建子进程..." << endl;

    pid_t pid = fork(); // 核心：创建一个新进程

    if (pid < 0) {
        // pid < 0 表示创建进程失败
        cerr << "【父进程】错误：fork 失败" << endl;
        return 1;
    } 
    else if (pid == 0) {
        // pid == 0 这个分支是在子进程中执行的代码
        cout << "【父进程代码的子进程分支】开始替换内存运行 child 可执行文件..." << endl;
        
        // 使用 execlp 替换当前进程镜像去执行我们编译好的子进程 "./child"
        // 注意：如果你编译出来的叫 child.exe 那么这里填 "./child.exe"
        execlp("./child", "child", nullptr);
        
        // 如果 exec 执行成功，下面的代码永远不会被执行。如果执行到了说明出错。
        cerr << "【父进程代码的子进程分支】错误：execlp 执行失败" << endl;
        return 1;
    } 
    else {
        // pid > 0 这个分支是在父进程中执行的代码，pid 保存的是子进程的 ID
        cout << "【父进程】成功创建子进程 (PID: " << pid << ")。父进程进入等待阻塞状态..." << endl;
        
        int status;
        // 父进程进入等待，直到子进程(pid)结束
        waitpid(pid, &status, 0); 

        cout << "【父进程】检测到子进程已执行完毕。父进程恢复执行并退出。" << endl;
    }

    return 0;
}