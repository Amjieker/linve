#include "router.h"

namespace lins {

bool notFoundHandle(HttpRequest& req, HttpResponse& res) {
  res.setStatusCode_(HttpResponse::NotFound404);
  res.setBody("<h1> not response </h1>");
  // std::cout << "not found\n";
  return true;
}

Router::Router(func_type&& notFound /* args */) : node(0) {
  memset(funcMap, 0, sizeof funcMap);
  funcor_.emplace_back(notFound);
}

void Router::insert(const std::string& url, func_type&& func) {
  func_idx_type cur = 0;
  assert(url.size() < 512);
  for (size_t i = 0; i < url.size(); i++) {
    if (!tree[cur][url[i]]) tree[cur][url[i]] = ++node;
    cur = tree[cur][url[i]];
  }
  funcMap[cur] = funcor_.size();
  funcor_.emplace_back(func);
}

Router::func_idx_type Router::find(const std::string& url) const {
  func_idx_type cur = 0;
  assert(url.size() < 512);
  for (int i = 0; i < url.size(); i++) {
    if (!tree[cur].count(url[i])) return 0;
    cur = tree[cur].find(url[i])->second;
  }
  return funcMap[cur];
}

const std::string& rootDir() {
  static std::once_flag rootFlag_;
  static std::unique_ptr<std::string> root_;
  std::call_once(rootFlag_, [&]() {
    char buf[1024];
    getcwd(buf, sizeof buf);
    // std::cout << "call" << std::endl;
    root_.reset(new std::string(buf));
  });
  return *root_;
}

// Router::Router(/* args */) {}

}  // namespace lins
