#include "httpConnection.h"

namespace lins {
HttpConnection::HttpConnection(/* args */) : request_(), response_() {
  addr_ = {0};
  isClose = true;
  iovSiz_ = 1;
  socketfd_ = -1;
}

HttpConnection::~HttpConnection() { close(); }

bool HttpConnection::init(int fd, const sockaddr_in& addr) {
  assert(fd > 0);
  addr_ = addr;
  socketfd_ = fd;
  writebuf_.retrieveAll();
  readbuf_.retrieveAll();
  isClose = false;
  // printf("connection from %s:%d\n", ipStr(), port());
  return true;
}

bool HttpConnection::close() {
  response_.unmapFile();
  if (!isClose) {
    isClose = true;
    ::close(socketfd_);
    // printf("close user\n");
  }
  return true;
}

ssize_t HttpConnection::read(int* saveError, bool isET) {
  ssize_t len = -1;
  do {
    len = readbuf_.readFd(socketfd_, saveError);
    if (len <= 0) break;
  } while (isET);
  return len;
}

ssize_t HttpConnection::write(int* saveError, bool isET) {
  assert(saveError);
  ssize_t len = -1;
  do {
    len = writev(socketfd_, iov_, iovSiz_);
    if (len <= 0) {
      *saveError = errno;
      break;
    }
    if (toWriteBytes() == 0) {
      break;
    } else if (len <= iov_[0].iov_len) {
      iov_[0].iov_base = (uint8_t*)iov_[0].iov_base + len;
      iov_[0].iov_len -= len;
      writebuf_.retrieve(len);
    } else {
      iov_[1].iov_base = (uint8_t*)iov_[1].iov_base + (len - iov_[0].iov_len);
      iov_[1].iov_len -= (len - iov_[0].iov_len);
      if (iov_[0].iov_len) {
        writebuf_.retrieveAll();
        iov_[0].iov_len = 0;
      }
    }
  } while (isET || toWriteBytes() > 10240);
  return len;
}

bool HttpConnection::process(Router& router) {
  request_.init();
  if (readbuf_.readableByte() <= 0) {
    return false;
  } else if (request_.parse(readbuf_)) {
    // printf("url %s\n", request_.path().c_str());
    INFO("%s %s OK %s", request_.version().c_str(), request_.method().c_str(),
         request_.path().c_str());
    response_.init(request_.isKeepAlive(), HttpResponse::Ok200);
  } else {
    WARN("%s %s NOT FOUND %s", request_.version().c_str(),
         request_.method().c_str(), request_.path().c_str());
    response_.init(false, HttpResponse::BadRequest400);
  }
  // 路由接管
  router.exec(request_.path())(request_, response_);
  response_.toBuffer(writebuf_);
  iov_[0].iov_base = const_cast<char*>(writebuf_.peek());
  iov_[0].iov_len = writebuf_.readableByte();
  iovSiz_ = 1;
  if (response_.fileHandle() != nullptr && response_.fileLength() > 0) {
    iov_[1].iov_base = response_.fileHandle();
    iov_[1].iov_len = response_.fileLength();
    iovSiz_ = 2;
  }
  return true;
}
}  // namespace lins
