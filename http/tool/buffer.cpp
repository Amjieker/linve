#include "buffer.h"
namespace lins {
const size_t Buffer::initPrepend;
const size_t Buffer::initSize;
const char Buffer::CRLF[] = "\r\n";
ssize_t Buffer::readFd(int fd, int* saveErrno) {
  char buf[65535];
  struct iovec iov[2];
  ssize_t writeable = writableByte();
  iov[0].iov_base = beginWrite();
  iov[0].iov_len = writeable;
  iov[1].iov_base = buf;
  iov[1].iov_len = sizeof(buf);
  ssize_t len = readv(fd, iov, 2);
  if (len < 0) {
    *saveErrno = errno;
  } else if (len <= writeable) {
    writePos_ += len;
  } else {
    writePos_ = buffer_.size();
    append(buf, len - writeable);
  }
  return len;
}
ssize_t Buffer::writeFd(int fd, int* saveErrno) {
  size_t readSize = readableByte();
  ssize_t len = write(fd, peek(), readSize);
  if (len < 0) {
    *saveErrno = errno;
  } else {
    retrieve(len);
  }
  return len;
}
}  // namespace lins
