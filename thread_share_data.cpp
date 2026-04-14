#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

int sharedData = 10;

void* threadFunc(void*) {
    cout << "【线程】读取到共享数据 sharedData = " << sharedData << endl;
    sharedData += 20;
    cout << "【线程】修改后共享数据 sharedData = " << sharedData << endl;
    return nullptr;
}

int main() {
    pthread_t tid;

    cout << "【主线程】创建线程前 sharedData = " << sharedData << endl;

    if (pthread_create(&tid, nullptr, threadFunc, nullptr) != 0) {
        cerr << "【主线程】错误：pthread_create 失败" << endl;
        return 1;
    }

    pthread_join(tid, nullptr);

    cout << "【主线程】线程结束后 sharedData = " << sharedData << endl;
    return 0;
}
