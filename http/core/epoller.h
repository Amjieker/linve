#ifndef EPOLL_H
#define EPOLL_H
#include <assert.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <unistd.h>

#include <vector>
namespace lins {
class Epoller {
 private:
  /* data */
  int epollFd_;
  std::vector<epoll_event> envent_;

 public:
  Epoller(int size = 1024)
      : epollFd_(epoll_create(114514) /*magic number*/), envent_(size) {}

  ~Epoller() { close(epollFd_); }

  bool addFd(int fd, int mode);

  bool modFd(int fd, int mode);

  bool delFd(int fd);

  int wait(int timeout = -1) {
    return epoll_wait(epollFd_, &envent_[0], envent_.size(), timeout);
  }

  const epoll_event& operator[](size_t idx) const {
    assert(0 <= idx && idx < envent_.size());
    return envent_[idx];
  }
};

}  // namespace lins

#endif