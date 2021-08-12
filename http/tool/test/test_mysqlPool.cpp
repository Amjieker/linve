#include <bits/stdc++.h>

#include <iostream>

#include "../mysqlPool.h"

using namespace std;

int main() {
  lins::MysqlPool::instance()->init("localhost", 3306, "root", "3306066", "web",
                                    10);
  {
    auto sql = lins::MysqlPool::instance()->getConn();
    std::string sql_query = "select * from user";
    if (mysql_query(sql.get(), sql_query.data())) {
      std::cout << "line: " << __LINE__ << ";" << mysql_error(sql.get())
                << mysql_errno(sql.get()) << std::endl;
    } else {
      MYSQL_RES* result = mysql_use_result(sql.get());
      if (result != nullptr) {
        MYSQL_ROW row;
        int num_filed = mysql_num_fields(result);
        while (row = mysql_fetch_row(result)) {
          for (int i = 0; i < num_filed; i++) std::cout << row[i] << " ";
          std::cout << std::endl;
        }
      }
    }
  }
  {
    auto sql = lins::MysqlPool::instance()->getConn();
    std::string sql_insert =
        "insert into user(username, password) values(\"[1]\", \"[2]\")";
    sql_insert.replace(sql_insert.find("[1]"), 3, "jake");
    sql_insert.replace(sql_insert.find("[2]"), 3, "114154");
    if (mysql_query(sql.get(), sql_insert.data())) {
      std::cout << "line: " << __LINE__ << ";" << mysql_error(sql.get())
                << mysql_errno(sql.get()) << std::endl;
    } else {
      std::cout << "is_ok" << std::endl;
    }
  }
  return 0;
}