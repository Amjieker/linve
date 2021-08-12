#ifndef COUNTDOWNLATCH_H
#define COUNTDOWNLATCH_H
#include <condition_variable>
#include <mutex>
#include <thread>
namespace lins {
class Semaphore {
 public:
  Semaphore(int size) : count_(size), wakeup_(0) {}

  ~Semaphore() {}

  void wait() {
    std::unique_lock<std::mutex> locker(mtx_);
    if (--count_ < 0) {
      cond_.wait(locker, [&]() { return wakeup_ > 0; });
      --wakeup_;
    }
  }

  void signal() {
    std::unique_lock<std::mutex> locker(mtx_);
    if (++count_ <= 0) {
      ++wakeup_;
      cond_.notify_one();
    }
  }

  int getCount() {
    std::lock_guard<std::mutex> locker(mtx_);
    return count_;
  }

 private:
  /* data */
  int count_;
  int wakeup_;
  std::mutex mtx_;
  std::condition_variable cond_;
};

}  // namespace lins

#endif