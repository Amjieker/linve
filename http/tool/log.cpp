#include "log.h"
namespace lins {
Log::Log(/* args */) {
  lineCount_ = 0;
  isAsync_ = false;
  writeThread_ = nullptr;
  deque_ = nullptr;
  toDay_ = 0;
  filePtr_ = nullptr;
}

Log::~Log() {
  if (writeThread_ && writeThread_->joinable()) {
    while (!deque_->empty()) deque_->flush();
    // close ...
    deque_->close();
    writeThread_->join();
  }
  if (filePtr_) {
    std::lock_guard<std::mutex> lock(mtx_);
    flush();
    ::fclose(filePtr_);
  }
}

void Log::init(int level, bool echo, const char* path, const char* suffix,
               int maxCapacity) {
  echo_ = echo;
  isOpen_ = true;
  level_ = level;
  if (maxCapacity > 0) {
    isAsync_ = true;
    if (!deque_) {
      deque_.reset(new BlockDeque<std::string>());
      writeThread_.reset(new std::thread(flushLogThread));
    }
  } else {
    isAsync_ = false;
  }
  lineCount_ = 0;
  time_t timer = time(nullptr);
  tm t = *localtime(&timer);
  path_ = path;
  suffix_ = suffix;
  char fileName[LOG_NAME_LEN] = {0};
  snprintf(fileName, LOG_NAME_LEN - 1, "%s/%04d_%02d_%02d%s", path_,
           t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, suffix_);
  toDay_ = t.tm_mday;
  {
    std::lock_guard<std::mutex> lock(mtx_);
    buf_.retrieveAll();
    if (filePtr_) {
      flush();
      fclose(filePtr_);
    }
    filePtr_ = fopen(fileName, "a");
    if (filePtr_ == nullptr) {
      mkdir(path_, 0777);
      filePtr_ = fopen(fileName, "a");
    }
    assert(filePtr_ != nullptr);
  }
}

void Log::write(int level, const char* format, ...) {
  timeval now = {0, 0};
  gettimeofday(&now, nullptr);
  time_t tSec = now.tv_sec;
  tm t = *localtime(&tSec);
  va_list vaList;
  if (toDay_ != t.tm_mday || (lineCount_ && (lineCount_ % MAX_LINES == 0))) {
    std::unique_lock<std::mutex> locker(mtx_);
    locker.unlock();
    char newFile[LOG_NAME_LEN];
    char tail[36] = {0};
    snprintf(tail, 36, "%04d_%02d_%02d", t.tm_year + 1900, t.tm_mon + 1,
             t.tm_mday + 1);
    if (toDay_ != t.tm_mday) {
      snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s%s", path_, tail, suffix_);
      toDay_ = t.tm_mday;
      lineCount_ = 0;
    } else {
      snprintf(newFile, LOG_NAME_LEN - 72, "%s/%s-%d%s", path_, tail,
               (lineCount_ / MAX_LINES), suffix_);
    }
    locker.lock();
    flush();
    fclose(filePtr_);
    filePtr_ = fopen(newFile, "a");
    assert(filePtr_ != nullptr);
  }
  {
    std::lock_guard<std::mutex> lock(mtx_);
    lineCount_++;
    int n =
        snprintf(buf_.beginWrite(), 128, "%d-%02d-%02d %02d:%02d:%02d.%06ld ",
                 t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min,
                 t.tm_sec, now.tv_usec);

    buf_.addWrite(n);
    appendLevelTitle(level);

    va_start(vaList, format);
    int m = vsnprintf(buf_.beginWrite(), buf_.writableByte(), format, vaList);
    va_end(vaList);

    buf_.addWrite(m);
    if (echo_) std::cout << buf_.toString() << "\033[0m\n";
    buf_.append("\n\0", 2);
    if (isAsync_ && deque_) {
      deque_->push_back(buf_.retrieveString(buf_.readableByte()));
    } else {
      fputs(buf_.peek(), filePtr_);
    }
    buf_.retrieveAll();
  }
}
template <>
LOG& LOG::operator<<(const LOGEND& rsh) {
  log->write(level_, buf_.toString().c_str());
  log->flush();
  buf_.retrieveAll();
  return *this;
}
}  // namespace lins
