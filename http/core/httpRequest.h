#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <regex>
#include <unordered_map>
#include <unordered_set>

#include "../tool/buffer.h"
#include "../tool/cookie.h"
namespace lins {
class HttpRequest {
 public:
  enum PARSE_STATE {
    REQUEST_LINE, /*解析请求行*/
    HEAD,         /*解析请求行*/
    BODY,         /*解析请求体*/
    FINISH,       /*解析完成*/
  };

  HttpRequest(/* args */) { init(); }

  ~HttpRequest() = default;

  void init();

  bool parse(Buffer& buf);

  bool isKeepAlive();

  const std::string& path() const { return this->url_; }

  const std::string& method() const { return this->method_; }

  const std::string& version() const { return this->version_; }

  const std::string& body() const { return this->body_; }

  const std::string& getHeader(const std::string& key) const {
    static std::string null("");
    if (head_.count(key)) return head_.find(key)->second;
    return null;
  }

  void addHeader(const char* start, const char* colon, const char* end) {
    std::string key(start, colon);
    colon++;
    while (colon < end && isspace(*colon)) colon++;
    std::string value(colon, end);
    while (value.size() && isspace(value.back()))
      value.resize(value.size() - 1);
    head_.insert({key, value});
  }

  const std::string& getParameter(const std::string& key) const {
    static std::string null("");
    if (parameter_.count(key)) return parameter_.find(key)->second;
    return null;
  }

  std::string getCookie(const std::string& key) {
    if (!isCookie_ && head_.count("cookie")) {
      cookie_.parse(head_["cookie"]);
      isCookie_ = true;
    }
    return cookie_.getCookie(key);
  }

  bool hasCookie(const std::string& key) { return cookie_.hasValue(key); }

 private:
  bool parseRequestLine_(const char* begin, const char* end);
  void parseParameter_();
  /* data */
  PARSE_STATE state_;
  Cookie cookie_;
  bool isCookie_;
  std::string method_;
  std::string url_;
  std::string version_;
  std::string body_;
  std::unordered_map<std::string, std::string> head_;
  std::unordered_map<std::string, std::string> parameter_;
};

};  // namespace lins

#endif