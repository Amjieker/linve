#include "epoller.h"
namespace lins {
bool Epoller::addFd(int fd, int mode) {
  assert(fd >= 0);
  epoll_event ev{0};
  ev.data.fd = fd;
  ev.events = mode;
  return 0 == epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd, &ev);
}

bool Epoller::modFd(int fd, int mode) {
  assert(fd >= 0);
  epoll_event ev{0};
  ev.data.fd = fd;
  ev.events = mode;
  return 0 == epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd, &ev);
}

bool Epoller::delFd(int fd) {
  assert(fd >= 0);
  epoll_event ev{0};
  return 0 == epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd, &ev);
}

};