#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(size_t workers) {
        for (size_t i = 0; i < workers; ++i) {
            threads.emplace_back([this] { worker(); });
        }
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            stopping = true;
        }
        condition.notify_all();
        for (auto& thread : threads) thread.join();
    }

    void submit(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (stopping) throw std::runtime_error("pool is stopping");
            tasks.push(std::move(task));
        }
        condition.notify_one();
    }

private:
    void worker() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mutex);
                condition.wait(lock, [this] { return stopping || !tasks.empty(); });
                if (stopping && tasks.empty()) return;
                task = std::move(tasks.front());
                tasks.pop();
            }
            task();
        }
    }

    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable condition;
    bool stopping{false};
};

int main() {
    ThreadPool pool(4);
    for (int i = 0; i < 10; ++i) {
        pool.submit([i] { std::cout << "Processing task " << i << '\n'; });
    }
}
