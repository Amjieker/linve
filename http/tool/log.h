#ifndef LOG_H
#define LOG_H
#include <assert.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <thread>

#include "blockDeque.h"
#include "buffer.h"
namespace lins {
class Log final {
 public:
  void init(int level, bool echo = true, const char* path = "./log",
            const char* suffix = ".log", int maxCapacity = 1024);

  static Log* instance() {
    static Log log;
    return &log;
  }

  static void flushLogThread() { Log::instance()->asynWrite_(); }

  void write(int level, const char* format, ...);

  void flush() {
    if (isAsync_) deque_->flush();
    ::fflush(filePtr_);
  }

  int getLevel() {
    std::lock_guard<std::mutex> lock(mtx_);
    return level_;
  }

  void setLevel(int level) {
    std::lock_guard<std::mutex> lock(mtx_);
    level_ = level;
  }

  bool isOpen() { return isOpen_; }

 private:
  Log(/* args */);
  ~Log();

  void appendLevelTitle(int level) {
    std::string str;
    std::string echoStr;
    switch (level) {
      case 0:
        str = "[DEBUG]:";
        if (echo_) echoStr = "\033[36m";
        break;
      case 1:
        str = "[INFO] :";
        if (echo_) echoStr = "\033[37m";
        break;
      case 2:
        str = "[WARN] :";
        if (echo_) echoStr = "\033[33m";
        break;
      case 3:
        str = "[ERROR]:";
        if (echo_) echoStr = "\033[31m";
        break;
      default:
        str = "[INFO] :";
        if (echo_) echoStr = "\033[37m";
        break;
    }
    if (echo_) std::cout << echoStr;
    buf_.append(str.data(), str.size());
  }

  void asynWrite_() {
    std::string str(" ");
    while (deque_->pop_front(str)) {
      std::lock_guard<std::mutex> lock(mtx_);
      fputs(str.c_str(), filePtr_);
    }
  }

  static const int LOG_PATH_LEN = 256;
  static const int LOG_NAME_LEN = 256;
  static const int MAX_LINES = 50000;

  /* data */
  const char* path_;
  const char* suffix_;
  int lineCount_; /*统计日志行数*/
  int toDay_;

  bool echo_; /*是否回显*/
  bool isOpen_;
  bool isAsync_; /* 异步 */
  int level_;
  Buffer buf_;

  FILE* filePtr_;

  std::mutex mtx_;
  std::unique_ptr<BlockDeque<std::string>> deque_;
  std::unique_ptr<std::thread> writeThread_;
};

struct LOGEND {};
/**
 *  no thread self
 *
 **/
class LOG {
 private:
  /* data */
  int level_;
  lins::Log* log;
  Buffer buf_;

 public:
  LOG(int level = 0 /* args */) : level_(level) { log = Log::instance(); };
  ~LOG() {
    log = nullptr;
    level_ = 0;
  }
  template <typename T>
  LOG& operator<<(const T& rsh) {
    if (level_ >= log->getLevel()) {
      std::ostringstream os;
      os << rsh;
      const std::string& s = os.str();
      buf_.append(s.data(), s.size());
    }
    return *this;
  }
};

// extern LOG DEBUG(0), INFO(1), WARN(2), ERROR(3);
// extern LOGEND end;
/*
#define LOG_BASE(level, format, ...)                 \
  do {                                               \
    lins::Log* log = lins::Log::instance();          \
    if (log->isOpen() && log->getLevel() <= level) { \
      log->write(level, format, ##__VA_ARGS__);      \
      log->flush();                                  \
    }                                                \
  } while (0);
*/
#define LOG_BASE(level, format, ...)

#define DEBUG(format, ...)             \
  do {                                 \
    LOG_BASE(0, format, ##__VA_ARGS__) \
  } while (0);
#define INFO(format, ...)              \
  do {                                 \
    LOG_BASE(1, format, ##__VA_ARGS__) \
  } while (0);
#define WARN(format, ...)              \
  do {                                 \
    LOG_BASE(2, format, ##__VA_ARGS__) \
  } while (0);
#define ERROR(format, ...)             \
  do {                                 \
    LOG_BASE(3, format, ##__VA_ARGS__) \
  } while (0);
}  // namespace lins

#endif