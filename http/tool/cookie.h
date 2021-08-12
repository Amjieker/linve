#ifndef COOKIE_H
#define COOKIE_H
#include <string>
#include <unordered_map>
namespace lins {
class Cookie {
 public:
  Cookie(){};

  Cookie(std::string& str) { parse(str); }

  const std::string& getCookie(const std::string& key) const {
    if (!cookie_.count(key)) throw std::runtime_error("not found this cookie");
    return cookie_.find(key)->second;
  };

  void setCookie(const std::string key, const std::string& str) {
    cookie_[key] = str;
  }

  bool hasValue() { return cookie_.size(); }

  bool hasValue(const std::string key) { return cookie_.count(key); }

  std::string getString() {
    std::string str;
    for (auto v : cookie_) {
      str.append(v.first);
      str.push_back('=');
      str.append(v.second);
      str.push_back(';');
    }
    return str;
  }

  void clear() { cookie_.clear(); }

  void parse(const std::string& str);

  std::unordered_map<std::string, std::string> cookie_;
};

}  // namespace lins

#endif