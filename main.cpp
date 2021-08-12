#include <bits/stdc++.h>

#include "business/userLogin.h"
#include "http/lins.h"
#include "http/tool/mysqlPool.h"
using namespace std;
int main() {
  lins::Log::instance()->init(0, false);
  lins::MysqlPool::instance()->init("localhost", 3306, "root", "3306066", "web",
                                    10);
  lins::HttpServer server(10000, 3, 2000, false);

  server.addAssertDir("/http");
  server.addAssertDir("/assert");
  server.addPage("/admin/hello",
                 [](lins::HttpRequest& req, lins::HttpResponse& res) -> bool {
                   res.setBody("this is my page");
                   return true;
                 });

  server.addPage("/page/file",
                 [](lins::HttpRequest& req, lins::HttpResponse& res) -> bool {
                   res.setFileHandle("/home/lin/nets/httpserver/main.cpp");
                   return true;
                 });
  server.addPage("/json/add",
                 [](lins::HttpRequest& req, lins::HttpResponse& res) -> bool {
                   std::cout << req.body() << endl;
                   return true;
                 });
  server.addPage(
      "/api/json", [](lins::HttpRequest& req, lins::HttpResponse& res) -> bool {
        res.addHeader("Content-Type", "application/json");
        res.setBody(
            "{\"paging\": {\"is_end\": true, \"totals\": 0, \"previous\": "
            "\"http://"
            "concerned_followers?limit=7&offset=0\", \"is_start\": true, "
            "\"next\": "
            "\"http://"
            "concerned_followers?limit=7&offset=7\"}, \"data\": []}");
        return true;
      });
  server.addPage("/api/cookie",
                 [](lins::HttpRequest& req, lins::HttpResponse& res) -> bool {
                   //  cout << req.getCookie("name") << endl;
                   //  cout << req.getCookie("password") << endl;
                   //  cout << req.getCookie("age") << endl;
                   res.setCookie("name", "\"zhangsan\"");
                   res.setCookie("pasword", "\"zhangsan\"");
                   res.setCookie("age", "\"zhangsan\"");
                   res.setBody("这是cookie test 页");
                   return true;
                 });
  new UserLogin(server);
  server.Start();
  return 0;
}