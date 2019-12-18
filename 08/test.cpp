#include <chrono>
#include <iostream>

#include "threadpool.h"

int ff1(int dd) {
    sleep(3);
    std::cout << dd << std::endl;
    return 11;
}
int ff2() {
    std::cout << "dd" << std::endl;
    return 2;
}
int main() {
    ThreadPool pool(3);
    auto start = std::chrono::high_resolution_clock::now();
    auto task1 = pool.exec(ff1, 55);
    auto task3 = pool.exec(ff1, 55);
    auto task4 = pool.exec(ff1, 55);
    auto task2 = pool.exec(ff2);
    std::cout << task1.get() << ' ' << task2.get() << std::endl;
    const auto finish = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count() << " us" << std::endl;
}