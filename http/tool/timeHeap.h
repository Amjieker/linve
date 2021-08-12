#ifndef TIMEHEAP_H
#define TIMEHEAP_H
#include <assert.h>

#include <chrono>
#include <functional>
#include <unordered_map>
#include <vector>
namespace lins {
using Clock = std::chrono::high_resolution_clock;
using TimeStamp = Clock::time_point;
using Milliseconds = std::chrono::milliseconds;

class TimeHeap {
  using func = void();

 public:
  struct event {
    int id;
    TimeStamp expires;
    std::function<func> callback;
    bool operator<(const event& rsh) const { return expires < rsh.expires; }
  };

  TimeHeap(/* args */) { heap_.reserve(64); }

  ~TimeHeap() { clear(); }

  void adjust(int id, int newExpires);

  void add(int id, int timeOut, const std::function<func>& callback);

  void doWork(int id) {
    if (heap_.empty() || mapping_.count(id) == 0) return;
    heap_[mapping_[id]].callback();
    del_(mapping_[id]);
  }

  void clear() {
    heap_.clear();
    mapping_.clear();
  }

  void tick() {
    while (!heap_.empty()) {
      auto& now = heap_.front();
      if (std::chrono::duration_cast<Milliseconds>(now.expires - Clock::now())
              .count() > 0)
        break;
      now.callback();
      pop();
      ;
    }
  }

  void pop() {
    assert(!heap_.empty());
    del_(0);
  }

  /*-1 heap is null*/
  int nextTick() {
    tick();
    size_t res = -1;
    if (!heap_.empty()) {
      res = std::chrono::duration_cast<Milliseconds>(heap_.front().expires -
                                                     Clock::now())
                .count();
      if (res < 0) res = 0;
    }
    return res;
  }

 private:
  /* data */

  void up_(int idx);

  bool down_(int idx, size_t n);

  void del_(int idx);

  void swap_event(size_t i, size_t j) {
    ::std::swap(heap_[i], heap_[j]);
    mapping_[heap_[i].id] = i;
    mapping_[heap_[j].id] = j;
  }
  /*
    node -> left => idx * 2 + 1
    node -> right => idx * 2 + 2
    father => (idx - 1) / 2;
  */
  std::vector<event> heap_;              /* start to 0 heap*/
  std::unordered_map<int, int> mapping_; /* mapping heap event to id*/
};

}  // namespace lins

#endif