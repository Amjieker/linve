#ifndef HTTPSERVER_H
#define HTTPSERVER_H
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

#include "../tool/buffer.h"
#include "../tool/defer.h"
#include "../tool/log.h"
#include "../tool/threadPool.h"
#include "../tool/timeHeap.h"
#include "httpConnection.h"
#include "router.h"
namespace lins {
class HttpServer {
 public:
  HttpServer(int port, int trigMode, int timeoutMS, bool optLinger);
  ~HttpServer();
  void Start();
  void addAssertDir(const char* file);
  void addPage(const std::string& str, Router::func_type&& type) {
    router_->insert(str, std::move(type));
  }

 private:
  void initEventMode_(int trigMode);
  bool initSocket_();
  int setnonblocking(int fd);

  void dealListen_();
  void closeConn(HttpConnection* client);
  void sendError_(int fd, const char* info);
  void addClient_(int fd, sockaddr_in addr);

  void dealRead_(HttpConnection* client);
  void dealWrite_(HttpConnection* client);
  void extentTime_(HttpConnection* client);

  void onWrite_(HttpConnection* clinet);
  void onRead_(HttpConnection* client);
  void onProcess(HttpConnection* client);

  void scan(std::string buf, std::string prefix);
  /* data */

  static const int MAX_FD = 65536;
  int port_;
  bool openLinger_;
  bool isET_;
  int timeoutMS_;
  int listenFd_;
  char* srcDir_;
  int connCount_;
  uint32_t listenEvent_;
  uint32_t connEvent_;
  std::unique_ptr<Router> router_;
  std::unique_ptr<Epoller> epoll_;
  std::unique_ptr<ThreadPool> ThreadPool_;
  std::unique_ptr<TimeHeap> timeHeap_;
  std::unordered_map<int, HttpConnection> user_;
};

}  // namespace lins

#endif