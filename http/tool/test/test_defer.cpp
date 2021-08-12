#include <iostream>

#include "../defer.h"
using namespace lins;

int main() {
  defer({ std::cout << 1 << std::endl; });

  std::cout << 2 << std::endl;

  defer({ std::cout << 3 << std::endl; });
  defer({ std::cout << 4 << std::endl; });
  return 0;
}