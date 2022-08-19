#include "userLogin.h"
struct temp {
  std::vector<int> ve;
  BEGIN_DUMP_JSON
  MAPPING(ve, "password");
  END_DUMP_JSON
};
struct user {
  std::string name;
  std::string password;
  void load(const lins::Json& json) {
    name = json["name"].STRING();
    password = json["password"].STRING();
  }
  BEGIN_DUMP_JSON
  MAPPING(name, "name");
  MAPPING(password, "password");
  END_DUMP_JSON
};

struct code {
  std::string code;
  std::string msg;
  void load(const lins::Json& json) {
    code = json["code"].STRING();
    msg = json["msg"].STRING();
  }
  BEGIN_DUMP_JSON
  MAPPING(code, "code");
  MAPPING(msg, "msg");
  END_DUMP_JSON
};

bool login(lins::HttpRequest& req, lins::HttpResponse& res) {
  if (!strcasecmp(req.method().c_str(), "get")) {
    res.setBody("not found this page");
    res.setStatusCode_(res.BadRequest400);
    return false;
  }
  res.addHeader("content-type", "application/json");
  if (req.getHeader("Content-Type") != "application/json") {
    res.setBody(
        code{.code = "404", .msg = "is not json data"}.dump().toString());
    return false;
  }
  user users;
  users.load(lins::JsonParse(req.body()).parse());
  auto sql = lins::MysqlPool::instance()->getConn();
  std::string sql_query =
      "select * from user where username = \"[1]\" and password = \"[2]\" ";
  sql_query.replace(sql_query.find("[1]"), 3, users.name);
  sql_query.replace(sql_query.find("[2]"), 3, users.password);
  if (mysql_query(sql.get(), sql_query.data())) {
    // std::cout << "line: " << __LINE__ << ";" << mysql_error(sql.get())
    //           << mysql_errno(sql.get()) << std::endl;
    res.setBody(code{.code = "500", .msg = "server error"}.dump().toString());
    return true;
  }
  MYSQL_RES* result = mysql_use_result(sql.get());
  if (result != nullptr && mysql_fetch_row(result) != nullptr) {
    res.setBody(code{.code = "200", .msg = "yes"}.dump().toString());
  } else {
    res.setBody(code{.code = "404", .msg = "no"}.dump().toString());
  }
  return true;
}

bool regist(lins::HttpRequest& req, lins::HttpResponse& res) {
  if (!strcasecmp(req.method().c_str(), "get")) {
    res.setBody("not found this page");
    res.setStatusCode_(res.BadRequest400);
    return false;
  }
  res.addHeader("content-type", "application/json");
  if (req.getHeader("Content-Type") != "application/json") {
    res.setBody(
        code{.code = "404", .msg = "is not json data"}.dump().toString());
    return false;
  }
  user users;
  users.load(lins::JsonParse(req.body()).parse());
  auto sql = lins::MysqlPool::instance()->getConn();
  std::string sql_query = "select * from user where username = \"[1]\" ";
  sql_query.replace(sql_query.find("[1]"), 3, users.name);
  if (mysql_query(sql.get(), sql_query.data())) {
    // std::cout << "line: " << __LINE__ << ";" << mysql_error(sql.get())
    //           << mysql_errno(sql.get()) << std::endl;
    res.setBody(code{.code = "500", .msg = "server error"}.dump().toString());
    return true;
  }
  MYSQL_RES* result = mysql_use_result(sql.get());
  if (result != nullptr && mysql_fetch_row(result) != nullptr) {
    res.setBody(code{.code = "400", .msg = "user has exist"}.dump().toString());
  } else {
    auto sql = lins::MysqlPool::instance()->getConn();
    std::string sql_insert =
        "insert into user(username, password) values(\"[1]\", \"[2]\")";
    sql_insert.replace(sql_insert.find("[1]"), 3, users.name);
    sql_insert.replace(sql_insert.find("[2]"), 3, users.password);
    if (mysql_query(sql.get(), sql_insert.data())) {
      // std::cout << "line: " << __LINE__ << ";" << mysql_error(sql.get())
      //           << mysql_errno(sql.get()) << std::endl;
      res.setBody(code{.code = "500", .msg = "server error"}.dump().toString());
    } else {
      res.setBody(code{.code = "200", .msg = "you successful register"}
                      .dump()
                      .toString());
    }
  }
  return true;
}
UserLogin::UserLogin(lins::HttpServer& server) {
  server.addPage("/api/user", login);
  server.addPage("/api/register", regist);
}
UserLogin::~UserLogin() {}
