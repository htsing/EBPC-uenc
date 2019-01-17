#include <iostream>
#include <thread>
#include <functional>

#include <uv>
#include "Queue.hpp"

using Task = std::function<void()>;

int main()
{
    std::cout << "主线程启动， id: " << std::this_thread::get_id() << std::endl;

	uv::Loop loop;
	uv::Async taskAsync;
    Queue<Task> taskQueue;

	loop.init();
	taskAsync.init(loop, [&taskQueue](){
        auto tasks = taskQueue.getAll();
        for (auto &task : tasks)
            task();
    });

    auto workThread = std::thread([&loop](){
        std::cout << "事件驱动线程启动， id: " << std::this_thread::get_id() << std::endl;
        loop.run();
    });

	while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));

        auto task = [](){
            std::cout << "子线程id: " << std::this_thread::get_id() << std::endl;
        };
        taskQueue.put(task);
        
        taskAsync.send();
    }
}

