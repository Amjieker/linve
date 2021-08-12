#include "cookie.h"
namespace lins {

void Cookie::parse(const std::string& str) {
  size_t pos = 0;
  ;
  while (pos < str.size()) {
    auto end = str.find_first_of(';', pos);
    if (end == str.npos) end = str.size();
    auto eq = str.find_last_of('=', end);
    if (eq == str.npos) break;
    if (pos != eq && eq != end)
      setCookie(str.substr(pos, eq - pos), str.substr(eq + 1, end - eq - 1));
    pos = end + 1;
  }
}
}  // namespace lins
