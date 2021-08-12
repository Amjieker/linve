#include "httpRequest.h"

namespace lins {

bool HttpRequest::parse(Buffer& buf) {
  if (buf.readableByte() <= 0) return false;
  int ok = true, hasMore = true;
  while (hasMore) {
    switch (state_) {
      case REQUEST_LINE: {
        const char* crlf = buf.findCRLF();
        if (crlf) {
          ok = parseRequestLine_(buf.peek(), crlf);
          if (ok) {
            // log receive timer
            state_ = HEAD;
            buf.retrieveUntil(crlf + 2);
          } else {
            hasMore = false;
          }
        } else {
          hasMore = false;
        }
        break;
      }
      case HEAD: {
        const char* crlf = buf.findCRLF();
        if (crlf) {
          const char* colon = std::find(buf.peek(), crlf, ':');
          if (colon != crlf) {
            addHeader(buf.peek(), colon, crlf);
          } else {
            if (buf.readableByte() <= 2) {
              state_ = FINISH;
            } else {
              state_ = BODY;
            }
          }
          buf.retrieveUntil(crlf + 2);
        } else {
          hasMore = false;
        }
        break;
      }
      case BODY: {
        body_ = buf.retrieveString(buf.readableByte());
        state_ = FINISH;
        hasMore = false;
        break;
      }
      default:
        hasMore = false;
        break;
    }
  }
  return ok;
}

bool HttpRequest::parseRequestLine_(const char* begin, const char* end) {
  // std::regex patten("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
  // std::smatch match;
  // std::string str(start, end);
  // if (std::regex_match(str, match, patten)) {
  //   this->method_ = match[1];
  //   this->url_ = match[2];
  //   this->version_ = match[3];
  //   state_ = HEAD;
  //   parseParameter_();
  //   return true;
  // }
  bool flag = false;
  const char* start = begin;
  const char* space = std::find(start, end, ' ');
  if (space != end) {
    method_ = std::string(start, space);
    start = space + 1;
    space = std::find(start, end, ' ');
    if (space != end) {
      url_ = std::string(start, space);
      parseParameter_();
      start = space + 1;
      flag = end - start == 8 && std::equal(start, end - 1, "HTTP/1.");
      if (flag) version_ = std::string(start, end);
    }
  }
  // printf("parse request line error\n");
  return flag;
}

void HttpRequest::parseParameter_() {
  auto start = url_.find('?');
  if (start != url_.npos) {
    std::string newUrl = url_.substr(0, start);
    start++;
    while (start < url_.size()) {
      auto end = url_.find_first_of('&', start);
      if (end == url_.npos) end = url_.size();
      auto div = url_.find_first_of('=', start);
      parameter_.insert({url_.substr(start, div - start),
                         url_.substr(div + 1, end - div - 1)});
      start = end + 1;
    }
    url_.swap(newUrl);
  }
}

void HttpRequest::init() {
  method_ = url_ = version_ = body_ = "";
  isCookie_ = false;
  state_ = HttpRequest::REQUEST_LINE;
  head_.clear();
  parameter_.clear();
}

bool HttpRequest::isKeepAlive() {
  if (version_ != "HTTP/1.1") return false;
  if (head_.count("Connection")) {
    return "keep-alive" == head_.find("Connection")->second;
  }
  return false;
}
}  // namespace lins
