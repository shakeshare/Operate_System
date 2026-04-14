#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstring>

int sharedData = 10;
pthread_mutex_t dataMutex = PTHREAD_MUTEX_INITIALIZER;

void* modifySharedData(void* /*arg*/) {
    int before;
    int after;
    int status = pthread_mutex_lock(&dataMutex);
    if (status != 0) {
        std::cerr << "【线程】错误：pthread_mutex_lock 失败，原因："
                  << strerror(status) << std::endl;
        return nullptr;
    }
    before = sharedData;
    sharedData += 20;
    after = sharedData;
    status = pthread_mutex_unlock(&dataMutex);
    if (status != 0) {
        std::cerr << "【线程】错误：pthread_mutex_unlock 失败，原因："
                  << strerror(status) << std::endl;
        return nullptr;
    }

    std::cout << "【线程】读取到共享数据 sharedData = " << before << std::endl;
    std::cout << "【线程】修改后共享数据 sharedData = " << after << std::endl;
    return nullptr;
}

int main() {
    pthread_t tid;
    int status;

    std::cout << "【主线程】创建线程前 sharedData = " << sharedData << std::endl;

    status = pthread_create(&tid, nullptr, modifySharedData, nullptr);
    if (status != 0) {
        std::cerr << "【主线程】错误：pthread_create 失败，原因："
                  << strerror(status) << std::endl;
        return 1;
    }

    status = pthread_join(tid, nullptr);
    if (status != 0) {
        std::cerr << "【主线程】错误：pthread_join 失败，原因："
                  << strerror(status) << std::endl;
        return 1;
    }

    std::cout << "【主线程】线程结束后 sharedData = " << sharedData << std::endl;

    status = pthread_mutex_destroy(&dataMutex);
    if (status != 0) {
        std::cerr << "【主线程】错误：pthread_mutex_destroy 失败，原因："
                  << strerror(status) << std::endl;
        return 1;
    }
    return 0;
}
