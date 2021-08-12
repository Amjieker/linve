#include "timeHeap.h"

namespace lins {

void TimeHeap::up_(int idx) {
  assert(idx >= 0 && idx < heap_.size());
  int cur = (idx - 1) / 2;
  while (cur >= 0 && heap_[idx] < heap_[cur]) {
    swap_event(cur, idx);
    idx = cur;
    cur = (idx - 1) / 2;
  }
}

bool TimeHeap::down_(int idx, size_t n) {
  assert(0 <= idx && idx < heap_.size());
  assert(0 <= n && n <= heap_.size());
  int i = idx;
  int j = idx * 2 + 1;
  while (j < n) {
    if (j + 1 < n && heap_[j + 1] < heap_[j]) j++;
    if (heap_[i] < heap_[j]) break; /* over */
    swap_event(i, j);
    i = j;
    j = i * 2 + 1;
  }
  return i > idx;
}

void TimeHeap::del_(int idx) {
  // for (auto v : heap_) std::cout << v.id << " ";
  // std::cout << "\n";
  assert(0 <= idx && idx < heap_.size());
  int i = idx;
  int n = heap_.size() - 1;
  if (i < n) {
    swap_event(i, n);
    if (!down_(i, n)) up_(i);
  }

  mapping_.erase(heap_.back().id);
  heap_.pop_back();
}

void TimeHeap::add(int id, int timeOut, const std::function<func>& callback) {
  assert(id >= 0);
  if (mapping_.count(id) == 0) {
    size_t newidx = heap_.size();
    mapping_[id] = newidx;
    heap_.push_back({id, Clock::now() + Milliseconds(timeOut), callback});
    up_(newidx);
  } else {
    size_t nowidx = mapping_[id];
    heap_[nowidx].expires = Clock::now() + Milliseconds(timeOut);
    heap_[nowidx].callback = callback;
    if (!down_(nowidx, heap_.size())) up_(nowidx);
  }
  // for (auto v : heap_) std::cout << v.id << " ";
  // std::cout << std::endl;
}

void TimeHeap::adjust(int id, int timeOut) {
  assert(heap_.size() && mapping_.count(id));
  heap_[mapping_[id]].expires = Clock::now() + Milliseconds(timeOut);
  down_(mapping_[id], heap_.size());
}

}  // namespace lins
