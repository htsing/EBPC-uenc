#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <mutex>
#include <list>
#include <iostream>

using namespace std;

template <typename T>
class Queue
{
public:
    void put(const T &t){
        std::lock_guard<std::mutex> guard(mutex_);
        queue_.push_back(t);
    }

    void put(T &&t){
        std::lock_guard<std::mutex> guard(mutex_);
        queue_.emplace_back(t);
    }

    std::list<T> getAll(){
        std::lock_guard<std::mutex> guard(mutex_);
        std::list<T> queue;
        queue.swap(queue_);
        return queue;
    }

private:
    std::list<T> queue_;
    std::mutex mutex_;
};

std::cout << "finish work." << std::endl;

cout << "happy everyday!!!" << endl;

#endif
