#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int sharedData = 10;

void* modifySharedData(void*) {
    cout << "【线程】读取到共享数据 sharedData = " << sharedData << endl;
    sharedData += 20;
    cout << "【线程】修改后共享数据 sharedData = " << sharedData << endl;
    return nullptr;
}

int main() {
    pthread_t tid;

    cout << "【主线程】创建线程前 sharedData = " << sharedData << endl;

    int createResult = pthread_create(&tid, nullptr, modifySharedData, nullptr);
    if (createResult != 0) {
        cerr << "【主线程】错误：pthread_create 失败，原因："
             << strerror(createResult) << endl;
        return 1;
    }

    int joinResult = pthread_join(tid, nullptr);
    if (joinResult != 0) {
        cerr << "【主线程】错误：pthread_join 失败，原因："
             << strerror(joinResult) << endl;
        return 1;
    }

    cout << "【主线程】线程结束后 sharedData = " << sharedData << endl;
    return 0;
}
