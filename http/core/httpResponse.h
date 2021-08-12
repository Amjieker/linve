#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <iostream>
#include <string>
#include <unordered_map>

#include "../tool/buffer.h"
#include "../tool/cookie.h"
namespace lins {

class HttpResponse {
 public:
  static const std::unordered_map<std::string, std::string> SUFFIX_TYPE;
  static const std::unordered_map<int, std::string> CODE_STATUS;
  static const std::unordered_map<int, std::string> CODE_PATH;

  enum HttpStatusCode {
    Unknown,
    Ok200 = 200,
    MovedPermanently301 = 301,
    BadRequest400 = 400,
    Forbidden403 = 403,
    NotFound404 = 404,
  };
  HttpResponse(/* args */) = default;

  ~HttpResponse() = default;

  void init(bool keepLive, HttpStatusCode code) {
    keepLive_ = keepLive;
    statusCode_ = code;
    body_ = "";
    mmFile_ = nullptr;
    mmFileStat_ = {0};
    statusMessage_ = CODE_STATUS.find(code)->second;
    header_.clear();
    header_.insert({"Server", "lins-WebServer/1.1"});
    header_.insert({"Content-Type", "text/html;charset=utf-8"});
    cookie_.clear();
  }

  void setStatusCode_(HttpStatusCode code) {
    statusCode_ = code;
    statusMessage_ = CODE_STATUS.find(code)->second;
  }

  void setStatusMessage(const std::string& statusMessage) {
    statusMessage_ = statusMessage;
  }

  void setBody(const std::string& body) { body_ = body; }

  void addHeader(const std::string& key, const std::string& value) {
    header_[key] = value;
  }

  void toBuffer(Buffer& buf);

  char* fileHandle() { return mmFile_; }

  size_t fileLength() { return mmFileStat_.st_size; }

  bool setFileHandle(const std::string& fileName);

  void unmapFile() {
    if (mmFile_) {
      munmap(mmFile_, mmFileStat_.st_size);
      mmFile_ = nullptr;
    }
  }

  void setCookie(const std::string& key, const std::string& value) {
    cookie_.setCookie(key, value);
  }

 private:
  /* data */
  bool keepLive_;
  std::unordered_map<std::string, std::string> header_;
  HttpStatusCode statusCode_;
  std::string body_;
  std::string statusMessage_;
  char* mmFile_;
  struct stat mmFileStat_;
  Cookie cookie_;
};

}  // namespace lins

#endif