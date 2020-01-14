#include <string>
#include <iostream>
#include <future>
#include <queue>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>
#include <condition_variable>
#include <memory>



class ThreadPool {
public:
    explicit ThreadPool(size_t poolSize)
        : poolSize_(poolSize)
        , keepWorking(true) {
        for (size_t i = 0; i < poolSize; i++) {
            threads.emplace_back(&ThreadPool::threadManager, this);
        }
    };

    ~ThreadPool() {
        keepWorking = false;
        for (size_t i = 0; i < poolSize_; i++) {
            threads[i].detach();
        }
    }

    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))> {
        using retType = decltype(func(args...));
        auto task = new std::packaged_task<retType()>(std::move(std::bind(func, args...)));
        std::future<retType> future = task->get_future();
        const std::lock_guard<std::mutex> lock(queueMutex_);
        tasks_.emplace([task]() { (*task)(); delete task; });
        dataReady_.notify_one();
        return future;
    }

private:
    size_t poolSize_;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks_;
    std::mutex queueMutex_;
    std::condition_variable dataReady_;
    bool keepWorking;

    void threadManager() {
        while (keepWorking) {
            std::unique_lock<std::mutex> lock(queueMutex_);
            dataReady_.wait(lock);
            auto task = tasks_.front();
            tasks_.pop();
            task();
        }
    }
};

