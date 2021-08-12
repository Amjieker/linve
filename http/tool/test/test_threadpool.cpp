#include <time.h>

#include <atomic>
#include <iostream>

#include "../threadpool.h"
int main() {
  const int N = 1e5;
  {
    lins::Threadpool pools(8);
    std::atomic<long long> sum{0};
    long long rs = 0;
    auto t = clock();
    [&]() -> void {
      for (int i = 1; i <= N; i++) {
        rs += i;
        pools.addTask([k = i, &sum = sum]() -> void {
          // int oldsum = sum;
          sum.fetch_add(k);
        });
      }
    }();
    // while (!pools.pool_->tasks.empty())
    //   ;
    // std::cout << clock() - t << "\n";
    // std::cout << rs << " " << sum << " : sum \n";
  }
  {
    long long rs = 0;
    auto t = clock();
    [&]() -> void {
      for (int i = 1; i <= N; i++) {
        rs += i;
      }
    }();
    std::cout << clock() - t << "\n";
    std::cout << rs << " : sum \n";
  }
  return 0;
}