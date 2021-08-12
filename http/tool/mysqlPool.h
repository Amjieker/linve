#ifndef MYSQLPOOL_H
#define MYSQLPOOL_H
#include <assert.h>
#include <mysql/mysql.h>

#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "./semaphore.h"
namespace lins {
class MysqlPool {
 public:
  static MysqlPool* instance();
  std::shared_ptr<MYSQL> getConn();
  void freeConn(MYSQL* conn);
  void closePool();
  void init(const char* host, int port, const char* user, const char* password,
            const char* databaseName, int connSize);

 private:
  MysqlPool(/* args */);
  ~MysqlPool();
  /* data */
  int MAX_CONN_;
  int useCount_;
  int freeCount_;

  std::shared_ptr<Semaphore> sgm_;
  std::queue<MYSQL*> connQue_;
  std::mutex mtx_;
};

}  // namespace lins

#endif