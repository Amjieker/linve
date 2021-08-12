#include "httpResponse.h"

namespace lins {

const std::unordered_map<std::string, std::string> HttpResponse::SUFFIX_TYPE = {
    {".html", "text/html"},
    {".xml", "text/xml"},
    {".xhtml", "application/xhtml+xml"},
    {".txt", "text/plain"},
    {".rtf", "application/rtf"},
    {".pdf", "application/pdf"},
    {".word", "application/nsword"},
    {".png", "image/png"},
    {".gif", "image/gif"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".au", "audio/basic"},
    {".mpeg", "video/mpeg"},
    {".mpg", "video/mpeg"},
    {".avi", "video/x-msvideo"},
    {".gz", "application/x-gzip"},
    {".tar", "application/x-tar"},
    {".css", "text/css "},
    {".js", "text/javascript "},
};

const std::unordered_map<int, std::string> HttpResponse::CODE_STATUS = {
    {200, "OK"},        {301, "MovedPermanently"}, {400, "Bad Request"},
    {403, "Forbidden"}, {404, "Not Found"},
};

const std::unordered_map<int, std::string> HttpResponse::CODE_PATH = {
    {400, "/400.html"},
    {403, "/403.html"},
    {404, "/404.html"},
};

void HttpResponse::toBuffer(Buffer& out) {
  char buf[32];
  snprintf(buf, sizeof buf, "HTTP/1.1 %d ", statusCode_);
  out.append(buf);
  out.append(statusMessage_);
  out.append("\r\n");
  if (!keepLive_) {
    out.append("Connection: close\r\n");
  } else {
    size_t len = body_.size();
    if (mmFile_ && mmFileStat_.st_size) len += mmFileStat_.st_size;
    snprintf(buf, sizeof buf, "Content-Length: %zd\r\n", len);
    out.append(buf);
    out.append("Connection: Keep-Alive\r\n");
  }
  for (const auto& head : header_) {
    out.append(head.first), out.append(": ");
    out.append(head.second), out.append("\r\n");
  }
  for (const auto& cook : cookie_.cookie_) {
    out.append("Set-Cookie"), out.append(": ");
    out.append(cook.first);
    out.append("=");
    out.append(cook.second);
    out.append(";\r\n");
  }
  out.append("\r\n");
  out.append(body_);
}

bool HttpResponse::setFileHandle(const std::string& fileName) {
  if (stat(fileName.data(), &mmFileStat_) < 0 || S_ISDIR(mmFileStat_.st_mode)) {
    setStatusCode_(NotFound404);
    return false;
  } else if (!(mmFileStat_.st_mode | S_IROTH)) {
    setStatusCode_(Forbidden403);
    return false;
  }
  int fileFd = open(fileName.data(), O_RDONLY);
  if (fileFd < 0) {
    setStatusCode_(NotFound404);
    return false;
  }
  int* ret =
      (int*)mmap(0, mmFileStat_.st_size, PROT_READ, MAP_PRIVATE, fileFd, 0);
  if (*ret == -1) {
    setStatusCode_(NotFound404);
    return false;
  }
  mmFile_ = (char*)ret;
  close(fileFd);
  return true;
}
}  // namespace lins
