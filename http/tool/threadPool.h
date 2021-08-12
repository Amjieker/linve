#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <condition_variable>
#include <functional>
// #include <iostream>
#include <mutex>
#include <queue>
#include <thread>
namespace lins {

class ThreadPool {
 private:
  struct task {
    /* data */
    bool isColse;
    std::mutex mtx;
    std::condition_variable cond;
    std::queue<std::function<void()>> tasks;
  };

  std::shared_ptr<task> pool_;

 public:
  ThreadPool(size_t poolSize = 4) : pool_(std::make_shared<task>()) {
    for (int i = 0; i < poolSize; ++i) {
      std::thread([idx = i, pool = pool_]() -> void {
        std::unique_lock<std::mutex> locker(pool->mtx);
        for (;;) {
          if (!pool->tasks.empty()) {
            auto task = pool->tasks.front();
            pool->tasks.pop();
            // printf("queue task: %ld \n", pool->tasks.size());
            locker.unlock();

            // printf("%d unlock\n", idx);
            task();
            // printf("%d lock\n", idx);
            locker.lock();

          } else if (pool->isColse) {
            break;
          } else {
            pool->cond.wait(locker);
          }
        }
      }).detach();
    }
  }

  ThreadPool(ThreadPool&) = delete;

  ThreadPool(ThreadPool&&) = delete;

  ~ThreadPool() {
    {
      std::lock_guard<std::mutex> locker(pool_->mtx);
      pool_->isColse = true;
    }
    pool_->cond.notify_all();
  }

  template <typename T>
  void addTask(T&& task) {
    {
      std::lock_guard<std::mutex> locker(pool_->mtx);
      pool_->tasks.emplace(std::forward<T>(task));
    }
    pool_->cond.notify_one();
  }
};
}  // namespace lins

#endif