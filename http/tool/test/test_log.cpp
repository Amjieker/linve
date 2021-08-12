#include <bits/stdc++.h>

#include "../log.h"
#include "../threadPool.h"
using namespace std;

int main() {
  lins::Log::instance()->init(0);
  lins::ThreadPool pool(8);
  // for (int i = 0; i < 5; i++)
  //   thread([&]() {
  for (int i = 0; i < 10000; i++)
    pool.addTask([&]() {
      lins::ERROR << 111 << 222 << 333 << lins::end;
      lins::INFO << 444 << 555 << 666 << lins::end;
    });
  //   }).detach();
  getchar();
  // cout << " ";
  // int idx = 1;
  // while (1) {
  // cout << "\b" << idx++ << endl;
  //   idx %= 10;
  //   std::this_thread::sleep_for(std::chrono::milliseconds(100));
  // }
  return 0;
}