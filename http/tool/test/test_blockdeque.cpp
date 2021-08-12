#include <iostream>
#include <thread>

#include "../blockDeque.h"

int main() {
  struct E {
    int a, b;
  };
  lins::BlockDeque<E> que;
  // que.push_back({1, 2});
  // que.push_back({1, 2});
  // que.push_back({1, 2});
  // que.push_back({1, 2});
  const int N = 1e5;
  std::thread([&que, &N]() {
    for (int i = 1; i <= N; i++) {
      que.push_back({i, i});
      // std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }).detach();
  for (int i = 1; i <= 4; i++)
    std::thread([k = i, &que, &N]() {
      int s = 0;
      while (1) {
        auto v = que.pop_front();
        s++;
        std::cout << k << " " << s << "\n";
        // std::cout << k << " " << v.a << " " << v.b << "\n";
        // std::this_thread::sleep_for(std::chrono::seconds(1));
      }
    }).detach();
  getchar();
  return 0;
}