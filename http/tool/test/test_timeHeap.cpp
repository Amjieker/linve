#include <bits/stdc++.h>

#include "../timeHeap.h"

using namespace std;

int main() {
  lins::TimeHeap time;
  time.add(1, 5000, [&]() { cout << 1 << endl; });
  time.add(2, 4000, [&]() {
    cout << 2 << endl;
    // time.add(2, 2000, [&]() { cout << 2 << endl; });
  });
  time.add(3, 1000, [&]() {
    cout << 3 << endl;
    time.adjust(1, 2000);
    // time.add(3, 3000, [&]() { cout << 3 << endl; });
  });
  time.add(4, 2000, [&]() {
    cout << 4 << endl;
    time.adjust(1, 2000);
    // time.add(4, 4000, [&]() { cout << 4 << endl; });
  });
  time.add(5, 3000, [&]() {
    cout << 5 << endl;
    // time.add(5, 5000, [&]() { cout << 5 << endl; });
  });
  while (1) {
    time.nextTick();
  }
  return 0;
}