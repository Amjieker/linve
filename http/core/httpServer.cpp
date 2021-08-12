#include "httpServer.h"
namespace lins {

HttpServer::HttpServer(int port, int trigMode, int timeoutMS, bool optLinger)
    : router_(new Router()),
      epoll_(new Epoller()),
      ThreadPool_(new ThreadPool()),
      timeHeap_(new TimeHeap()),
      port_(port),
      openLinger_(optLinger),
      timeoutMS_(timeoutMS) {
  initEventMode_(trigMode);
  assert(initSocket_());
}

HttpServer::~HttpServer() { ::close(listenFd_); }

void HttpServer::initEventMode_(int trigMode) {
  signal(SIGPIPE, SIG_IGN); /* 避免 退出*/
  listenEvent_ = EPOLLRDHUP;
  connEvent_ = EPOLLONESHOT | EPOLLHUP;
  switch (trigMode) {
    case 0:
      break;
    case 1:
      connEvent_ |= EPOLLET;
      break;
    case 2:
      listenEvent_ |= EPOLLET;
      break;
    case 3:
      listenEvent_ |= EPOLLET;
      connEvent_ |= EPOLLET;
      break;
    default:
      listenEvent_ |= EPOLLET;
      connEvent_ |= EPOLLET;
      break;
  }
  isET_ = (connEvent_ & EPOLLET);
}

bool HttpServer::initSocket_() {
  int ret = -1;
  sockaddr_in addr;
  if (port_ > 65535 || port_ < 1024) {
    // printf("error port\n");

    return false;
  };
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
  addr.sin_port = ::htons(port_);
  linger optLinger = {0};
  if (openLinger_) optLinger.l_onoff = optLinger.l_linger = 1;
  listenFd_ = ::socket(AF_INET, SOCK_STREAM, 0);
  if (listenFd_ < 0) {
    perror("new socket");
    return false;
  }
  ret = ::setsockopt(listenFd_, SOL_SOCKET, SO_LINGER, &optLinger,
                     sizeof(optLinger));
  if (ret < 0) {
    ::close(listenFd_);
    perror("error setsockopt 1 ");
    return false;
  }

  int optval = 1;
  ret = setsockopt(listenFd_, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval,
                   sizeof(int));
  if (ret < 0) {
    ::close(listenFd_);
    perror("error setsockopt 2");
    return false;
  }
  ret = ::bind(listenFd_, (struct sockaddr*)&addr, sizeof(addr));
  if (ret < 0) {
    ::close(listenFd_);
    perror("error bind");
    return false;
  }
  ret = listen(listenFd_, 10);
  if (ret < 0) {
    ::close(listenFd_);
    perror("error listen");
    return false;
  }
  ret = epoll_->addFd(listenFd_, listenEvent_ | EPOLLIN);
  if (!ret) {
    ::close(listenFd_);
    perror("error addFd");
    return false;
  }
  setnonblocking(listenFd_);
  // printf("server start\n");
  INFO("server started on %d", port_);
  return true;
}

int HttpServer::setnonblocking(int fd) {
  assert(fd > 0);
  return fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
}

void HttpServer::Start() {
  int timeMs = -1;
  while (true) {
    if (timeoutMS_ > 0) timeMs = timeHeap_->nextTick(); /* time task*/
    int eventSiz = epoll_->wait(timeMs);
    for (int i = 0; i < eventSiz; ++i) {
      int fd = (*epoll_)[i].data.fd;
      uint32_t events = (*epoll_)[i].events;
      if (fd == listenFd_) {
        dealListen_();
      } else {
        assert(fd != -1);
        assert(user_.count(fd) > 0);
        if (events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
          closeConn(&user_[fd]);
        } else if (events & EPOLLIN) {
          dealRead_(&user_[fd]);
        } else if (events & EPOLLOUT) {
          dealWrite_(&user_[fd]);
        } else {
          // printf("unexpected event\n");
          ERROR("unexpected event !!!");
        }
      }
    }
  }
}

void HttpServer::dealListen_() {
  sockaddr_in addr;
  socklen_t len = sizeof(addr);

  do {
    int fd = accept(listenFd_, (sockaddr*)&addr, &len);
    if (fd <= 0) {
      return;
    }
    // else if (this->connCount_ >= MAX_FD) {
    //   sendError_(fd, "Server busy!");
    //   return;
    // }
    addClient_(fd, addr);
  } while (listenEvent_ & EPOLLET);
}

void HttpServer::sendError_(int fd, const char* info) {
  int ret = send(fd, info, strlen(info), 0);
  if (ret < 0) {
    perror("send message error");
  }
  ::close(fd);
}

void HttpServer::closeConn(HttpConnection* client) {
  // printf("close %d\n", client->sockfd());

  epoll_->delFd(client->sockfd());
  client->close();
}

void HttpServer::addClient_(int fd, sockaddr_in addr) {
  assert(fd != -1);
  user_[fd].init(fd, addr);
  assert(user_[fd].sockfd() != -1);
  // fix timeout close connetion
  /* code wait ...*/
  if (timeoutMS_ > 0)
    timeHeap_->add(fd, timeoutMS_,
                   std::bind(&HttpServer::closeConn, this, &user_[fd]));
  epoll_->addFd(fd, EPOLLIN | connEvent_);
  setnonblocking(fd);

  INFO("connetion from %s %d", user_[fd].ipStr(), user_[fd].port());
  // printf("connnetion %s %d\n", user_[fd].ipStr(), user_[fd].port());
}

void HttpServer::dealWrite_(HttpConnection* client) {
  ;
  assert(client->sockfd() != -1);
  extentTime_(client);
  ThreadPool_->addTask(std::bind(&HttpServer::onWrite_, this, client));
}

void HttpServer::dealRead_(HttpConnection* client) {
  ;
  assert(client->sockfd() != -1);
  extentTime_(client);
  ThreadPool_->addTask(std::bind(&HttpServer::onRead_, this, client));
}

/*keep live*/
void HttpServer::extentTime_(HttpConnection* client) {
  assert(client);
  if (timeoutMS_ > 0) timeHeap_->adjust(client->sockfd(), timeoutMS_);
}

void HttpServer::onRead_(HttpConnection* client) {
  int ret = -1;
  int readErrno = 0;
  ret = client->read(&readErrno, this->isET_);
  if (ret <= 0 && readErrno != EAGAIN) {
    closeConn(client);
    return;
  }
  onProcess(client);
}

void HttpServer::onWrite_(HttpConnection* clinet) {
  if (!clinet) return;
  int ret = -1;
  int writeErrno = 0;
  ret = clinet->write(&writeErrno, this->isET_);
  if (clinet->toWriteBytes() == 0) {
    if (clinet->isKeepLive()) {
      onProcess(clinet);
      return;
    }
  } else if (ret < 0) {
    if (writeErrno == EAGAIN) {
      epoll_->modFd(clinet->sockfd(), connEvent_ | EPOLLOUT);
      return;
    }
  }
  closeConn(clinet);
}

void HttpServer::onProcess(HttpConnection* client) {
  if (client->process(*router_)) {
    epoll_->modFd(client->sockfd(), connEvent_ | EPOLLOUT);
  } else {
    epoll_->modFd(client->sockfd(), connEvent_ | EPOLLIN);
  }
}

void HttpServer::addAssertDir(const char* fileDIR) {
  char buf[1024];
  getcwd(buf, 1023);
  strcat(buf, fileDIR);
  scan(buf, fileDIR);
}

void HttpServer::scan(std::string buf, std::string prefix) {
  DIR* dir;
  dirent* file;
  dir = opendir(buf.data());
  buf.push_back('/');
  prefix.push_back('/');
  if (dir != nullptr) {
    while (file = readdir(dir)) {
      if (file->d_type == DT_DIR) {
        if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0)
          continue;
        scan(buf + file->d_name, prefix + file->d_name);
      } else {
        std::string fileName(file->d_name), tmpSuffix = ".html";
        int idx;
        if ((idx = fileName.find_last_of('.', fileName.size())) !=
            fileName.npos) {
          tmpSuffix = fileName.substr(idx);
        }
        addPage(prefix + file->d_name,
                [suffix = tmpSuffix, filePath = buf, name = fileName](
                    lins::HttpRequest& req, lins::HttpResponse& res) -> bool {
                  res.setFileHandle(filePath + name);
                  if (HttpResponse::SUFFIX_TYPE.count(suffix)) {
                    res.addHeader(
                        "Content-Type",
                        HttpResponse::SUFFIX_TYPE.find(suffix)->second +
                            ";charset=utf-8");
                  } else {
                    res.addHeader("Content-Type", "text/plain;charset=utf-8");
                  }
                  return true;
                });
      }
    }
  }
}
}  // namespace lins
