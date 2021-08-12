#ifndef BUFFER_H
#define BUFFER_H
#include <assert.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
namespace lins {
class Buffer {
 private:
  /* data */
  static const size_t initPrepend = 8;
  static const size_t initSize = 1024;
  std::vector<char> buffer_;
  int readPos_;
  int writePos_;
  static const char CRLF[];
  char* begin() { return &*buffer_.begin(); }

  const char* begin() const { return &*buffer_.begin(); }

  void makeSpace(int len) {
    if (prependableByte() + writableByte() < len + initPrepend) {
      buffer_.resize(writePos_ + len);
    } else {
      assert(static_cast<int>(initPrepend) < readPos_);
      size_t old_read = readableByte();
      assert(readPos_ <= writePos_);
      std::copy(begin() + readPos_, begin() + writePos_, begin() + initPrepend);
      readPos_ = initPrepend;
      writePos_ = old_read + readPos_;
      assert(old_read == readableByte());
    }
  }

 public:
  Buffer(int bufferSize = initSize)
      : buffer_(initSize), readPos_(initPrepend), writePos_(initPrepend) {}

  Buffer(Buffer&) = default;

  Buffer(Buffer&&) = default;

  size_t readableByte() { return writePos_ - readPos_; }

  size_t writableByte() { return buffer_.size() - writePos_; }

  size_t prependableByte() { return readPos_; }

  char* beginWrite() { return begin() + writePos_; }

  const char* beginWrite() const { return begin() + writePos_; }

  void addWrite(size_t len) {
    assert(len <= writableByte());
    writePos_ += len;
  }

  const char* peek() const { return begin() + readPos_; }

  void retrieve(size_t len) {
    assert(len <= readableByte());
    if (len < readableByte()) {
      readPos_ += len;
    } else {
      retrieveAll();
    }
  }

  void retrieveAll() {
    readPos_ = initPrepend;
    writePos_ = initPrepend;
  }

  void retrieveUntil(const char* end) {
    assert(peek() <= end);
    assert(end <= beginWrite());
    retrieve(end - peek());
  }

  void checkWrite(size_t len) {
    if (writableByte() < len) makeSpace(len);
    assert(writableByte() >= len);
  }

  void append(const std::string& str) { append(str.data(), str.size()); }

  void append(const void* str, size_t len) {
    append(static_cast<const char*>(str), len);
  }

  void append(const char* str, size_t len) {
    checkWrite(len);
    std::copy(str, str + len, beginWrite());
    assert(writableByte() >= len);
    writePos_ += len;
  }

  std::string retrieveString(size_t len) {
    len = std::min(len, readableByte());
    std::string str = std::string(peek(), len);
    retrieve(len);
    return str;
  }

  void swap(Buffer& rhs) {
    buffer_.swap(rhs.buffer_);
    std::swap(readPos_, rhs.readPos_);
    std::swap(writePos_, rhs.writePos_);
  }

  std::string toString() { return std::string(peek(), readableByte()); }

  void shrink(size_t length) {
    Buffer buf;
    buf.checkWrite(readableByte() + length);
    buf.append(toString());
    swap(buf);
  }

  void prepend(const void* data, size_t len) {
    assert(len <= prependableByte());
    readPos_ -= len;
    const char* c_data = static_cast<const char*>(data);
    std::copy(c_data, c_data + len, begin() + readPos_);
  }

  ssize_t readFd(int fd, int* saveErrno);

  ssize_t writeFd(int fd, int* saveErrno);

  const char* findCRLF() const { return findCRLF(peek()); }

  const char* findCRLF(const char* start) const {
    assert(peek() <= start);
    assert(start <= beginWrite());
    const char* crlf = std::search(start, beginWrite(), CRLF, CRLF + 2);
    return crlf == beginWrite() ? nullptr : crlf;
  }

  const char* findEOL() const { return findEOL(peek()); }
  const char* findEOL(const char* start) const {
    assert(peek() <= start);
    assert(start <= beginWrite());
    const void* eol = memchr(start, '\n', beginWrite() - start);
    return static_cast<const char*>(eol);
  }

  ~Buffer() = default;
};
};  // namespace lins
#endif