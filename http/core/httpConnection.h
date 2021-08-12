#ifndef HTTPCONNETION_H
#define HTTPCONNETION_H
#include <arpa/inet.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

#include <atomic>
#include <mutex>
#include <string>

#include "../tool/buffer.h"
#include "../tool/log.h"
#include "epoller.h"
#include "httpRequest.h"
#include "httpResponse.h"
#include "router.h"
namespace lins {
class HttpConnection {
 private:
  /* data */
  bool isClose;
  int socketfd_;
  sockaddr_in addr_;
  struct iovec iov_[2];
  int iovSiz_;
  Buffer readbuf_;
  Buffer writebuf_;
  HttpRequest request_;
  HttpResponse response_;

 public:
  static bool isET;
  static const char* srcDir;
  static std::atomic<int> connCount;

  HttpConnection(/* args */);

  ~HttpConnection();

  bool init(int fd, const sockaddr_in& addr);

  bool close();

  int sockfd() { return socketfd_; }

  int port() { return ntohs(addr_.sin_port); }

  auto ip() -> decltype(addr_.sin_addr) { return addr_.sin_addr; };

  const char* ipStr() { return inet_ntoa(addr_.sin_addr); }

  int toWriteBytes() { return iov_[0].iov_len + iov_[1].iov_len; }

  bool isKeepLive() { return request_.isKeepAlive(); }

  ssize_t read(int* saveError, bool isET);

  ssize_t write(int* saveError, bool isET);

  bool process(Router& router);
};

}  // namespace lins

#endif