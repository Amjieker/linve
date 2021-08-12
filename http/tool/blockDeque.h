#ifndef BLOCKDEQUE_H
#define BLOCKDEQUE_H
#include <condition_variable>
#include <deque>
#include <mutex>

namespace lins {
template <class T>
class BlockDeque {
 private:
  using deque_type = std::deque<T>;
  deque_type queue_;
  bool isClose;
  std::mutex mtx_;
  std::condition_variable noEmpty_;

 public:
  BlockDeque() : mtx_(), noEmpty_(), queue_(), isClose(false) {}
  BlockDeque(BlockDeque&) = delete;
  BlockDeque(BlockDeque&&) = delete;
  ~BlockDeque() { close(); }

  void close() {
    {
      std::lock_guard<std::mutex> lock(mtx_);
      queue_.clear();
      isClose = true;
    }
    noEmpty_.notify_all();
  }

  void push_back(T& t) {
    std::lock_guard<std::mutex> locker(mtx_);
    queue_.push_back(t);
    noEmpty_.notify_one();
  }

  void push_back(T&& t) {
    std::lock_guard<std::mutex> locker(mtx_);
    queue_.push_back(std::move(t));
    noEmpty_.notify_one();
  }

  void push_front(T& t) {
    std::lock_guard<std::mutex> locker(mtx_);
    queue_.push_front(t);
    noEmpty_.notify_one();
  }

  void push_front(T&& t) {
    std::lock_guard<std::mutex> locker(mtx_);
    queue_.push_front(std::move(t));
    noEmpty_.notify_one();
  }

  size_t size() {
    std::lock_guard<std::mutex> locker(mtx_);
    return queue_.size();
  }

  bool empty() {
    std::lock_guard<std::mutex> locker(mtx_);
    return queue_.empty();
  }

  void flush() { noEmpty_.notify_one(); }
  bool pop_front(T& t) {
    std::unique_lock<std::mutex> locker(mtx_);
    while (queue_.empty()) {
      if (isClose) return false;
      noEmpty_.wait(locker);
    }
    t = std::move(queue_.front());
    queue_.pop_front();
    return true;
  }

  bool pop_back(T& t) {
    std::unique_lock<std::mutex> locker(mtx_);
    while (queue_.empty()) {
      if (isClose) return false;
      noEmpty_.wait(locker);
    }
    t = std::move(queue_.back());
    queue_.pop_back();
    return true;
  }
};
};  // namespace lins

#endif