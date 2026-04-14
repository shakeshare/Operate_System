#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int sharedData = 10;
pthread_mutex_t dataMutex = PTHREAD_MUTEX_INITIALIZER;

void* modifySharedData(void* /*arg*/) {
    int before;
    int after;
    {
        pthread_mutex_lock(&dataMutex);
        before = sharedData;
        sharedData += 20;
        after = sharedData;
        pthread_mutex_unlock(&dataMutex);
    }

    cout << "【线程】读取到共享数据 sharedData = " << before << endl;
    cout << "【线程】修改后共享数据 sharedData = " << after << endl;
    return nullptr;
}

int main() {
    pthread_t tid;

    int mainBefore;
    pthread_mutex_lock(&dataMutex);
    mainBefore = sharedData;
    pthread_mutex_unlock(&dataMutex);
    cout << "【主线程】创建线程前 sharedData = " << mainBefore << endl;

    int status = pthread_create(&tid, nullptr, modifySharedData, nullptr);
    if (status != 0) {
        cerr << "【主线程】错误：pthread_create 失败，原因："
             << strerror(status) << endl;
        return 1;
    }

    status = pthread_join(tid, nullptr);
    if (status != 0) {
        cerr << "【主线程】错误：pthread_join 失败，原因："
             << strerror(status) << endl;
        return 1;
    }

    int mainAfter;
    pthread_mutex_lock(&dataMutex);
    mainAfter = sharedData;
    pthread_mutex_unlock(&dataMutex);
    cout << "【主线程】线程结束后 sharedData = " << mainAfter << endl;

    status = pthread_mutex_destroy(&dataMutex);
    if (status != 0) {
        cerr << "【主线程】错误：pthread_mutex_destroy 失败，原因："
             << strerror(status) << endl;
        return 1;
    }
    return 0;
}
