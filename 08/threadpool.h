#include <iostream>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <functional>
#include <vector>
#include <utility>
#include <queue>
#include <unistd.h> 




class ThreadPool {
    std::queue<std::function<void()>> tasks;
    std::condition_variable new_task;
    std::mutex m;
    std::vector<std::thread> workers;
    std::atomic<bool> finish;
    size_t size;
    

public:
    void thread_f() {
        while(!finish) {
            std::unique_lock<std::mutex> lock(m);
            if(tasks.size() == 0) {
                new_task.wait(lock);
            } else {
                auto task = std::move(tasks.front());
                tasks.pop();
                lock.unlock();
                task();
            }
        }
    }
    explicit ThreadPool(size_t s) : finish(false) , size(s){
        for(int i = 0; i < s; i++) {
            workers.push_back(std::thread([this] {
                this->thread_f();
            }));
        }
    }
    ~ThreadPool() {
        finish = true;
        new_task.notify_all();
        for(int i = 0; i < size; i++) {
            workers[i].join();
        }
    }
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))> {
        auto task = std::make_shared<std::packaged_task<decltype(func(args...))()> >([func, args...]() { 
            return func(args...); 
        });

        auto res = task->get_future();
        std::unique_lock<std::mutex> lock(m);
        tasks.push([task]() {
            (*task)();
        });
        new_task.notify_one();
        return res;
    }

};