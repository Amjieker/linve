#include <iostream>

#include "../buffer.h"

int main() {
  lins::Buffer buf;
  // const int N = 1e1;
  // for (int i = 0; i < N; i++) {
  //   buf.append(std::string(N - i, (i % 10) + '0'));
  //   std::cout << buf.retrieveString(i + 1) << " \n";
  // }
  buf.append("121\r\n2121\r\n21212\r\n\n\r\n");
  while (1) {
    const char* c = buf.findCRLF();
    std::cout << c - buf.peek() << std::endl;
    std::string s(buf.peek(), c);
    std::cout << s << "\n";
    if (buf.peek() == c) break;
    buf.retrieveUntil(c + 2);
  }
  return 0;
}