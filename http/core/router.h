#ifndef ROUTER_H
#define ROUTER_H
#include <unistd.h>

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

#include "httpRequest.h"
#include "httpResponse.h"
namespace lins {
bool notFoundHandle(HttpRequest& req, HttpResponse& res);
class Router {
 public:
  using func_type = std::function<bool(HttpRequest&, HttpResponse&)>;
  using func_idx_type = size_t;
  Router(func_type&& = notFoundHandle /* args */);
  void insert(const std::string& url, func_type&& func);
  func_idx_type find(const std::string& url) const;
  func_type exec(const std::string& url) const { return funcor_[find(url)]; }
  ~Router() = default;

 private:
  /* data */
  std::vector<func_type> funcor_;
  /* 假设 url 最长 512 个字符 多则抛弃*/
  static const int MAX_LNEGTH = 128 * 512;
  std::map<char, func_idx_type> tree[MAX_LNEGTH];
  func_idx_type funcMap[MAX_LNEGTH];
  func_idx_type node;
};

class HttpBase {
 public:
  HttpBase() {}
};

};  // namespace lins

#endif