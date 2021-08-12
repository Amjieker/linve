#ifndef USER_LOGIN_H
#define USER_LOGIN_H
#include <functional>

#include "../http/lins.h"
#include "../http/tool/mysqlPool.h"
class UserLogin : lins::HttpBase {
 private:
  /* data */
 public:
  UserLogin();
  UserLogin(lins::HttpServer& server);
  ~UserLogin();
};

#endif