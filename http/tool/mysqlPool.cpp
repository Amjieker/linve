#include "mysqlPool.h"
namespace lins {

MysqlPool::MysqlPool(/* args */) {
  useCount_ = 0;
  freeCount_ = 0;
}

MysqlPool::~MysqlPool() { closePool(); }

void MysqlPool::init(const char* host, int port, const char* user,
                     const char* password, const char* databaseName,
                     int connSize) {
  for (int i = 0; i < connSize; i++) {
    MYSQL* sql = nullptr;
    sql = mysql_init(sql);
    assert(sql);
    sql = mysql_real_connect(sql, host, user, password, databaseName, port,
                             nullptr, 0);
    assert(sql);
    connQue_.push(sql);
  }
  MAX_CONN_ = connSize;
  sgm_.reset(new Semaphore(connSize));
}

MysqlPool* MysqlPool::instance() {
  static MysqlPool pool;
  return &pool;
}

std::shared_ptr<MYSQL> MysqlPool::getConn() {
  std::shared_ptr<MYSQL> sql = nullptr;
  sgm_->wait();
  {
    std::lock_guard<std::mutex> locker(mtx_);
    assert(connQue_.size());
    sql = std::shared_ptr<MYSQL>(connQue_.front(),
                                 [&](MYSQL* conn) -> void { freeConn(conn); });
    connQue_.pop();
  }
  return sql;
}

void MysqlPool::freeConn(MYSQL* conn) {
  std::lock_guard<std::mutex> locker(mtx_);
  connQue_.push(conn);
  sgm_->signal();
}

void MysqlPool::closePool() {
  std::lock_guard<std::mutex> locker(mtx_);
  while (connQue_.size()) {
    auto t = connQue_.front();
    connQue_.pop();
    mysql_close(t);
  }
  mysql_library_end();
}

}  // namespace lins
