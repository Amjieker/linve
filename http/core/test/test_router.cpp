#include <bits/stdc++.h>
#include <unistd.h>

#include "../../tool/threadPool.h"
#include "../router.h"
using namespace std;
int main() {
  // lins::ThreadPool pool(8);
  // const int N = 10;
  // for (int i = 0; i < N; i++) {
  //   pool.addTask([]() {
  //     std:
  //       cout << lins::rootDir() << std::endl;
  //   });
  // }
  // getchar();
  lins::Router router;
  lins::HttpRequest req;
  lins::Buffer buf, output;
  lins::HttpResponse res;
  buf.append(
      "GET /page/add?request=\"21311\"&name=\"张三\"&sex=2121 "
      "HTTP/1.1\r\n"
      "Host: www.lins.com\r\n"
      "Connection: keep-alive\r\n"
      "sec-ch-ua: \"Google Chrome\";v=\"103\", \"Chromium\";v=\"103\"\r\n"
      "Accept: text/plain, *\\/*; q=0.01\r\n"
      "X-Requested-With: XMLHttpRequest\r\n"
      "sec-ch-ua-mobile: ?0\r\n"
      "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) \r\n"
      "Sec-Fetch-Site: same-origin\r\n"
      "Sec-Fetch-Mode: cors\r\n"
      "Sec-Fetch-Dest: empty\r\n"
      "Accept-Encoding: gzip, deflate, br\r\n"
      "Accept-Language: zh-CN,zh;q=0.9\r\n\r\n {data:1}");
  res.addHeader("Server", "lins-WebServer/1.1");
  res.addHeader("Content-Type", "text/html;charset=utf-8");
  req.parse(buf);
  router.insert("/ok1",
                [](lins::HttpRequest& req, lins::HttpResponse& res) -> bool {
                  std::cout << "ok1\n";
                  return true;
                });
  router.insert("/ok2",
                [](lins::HttpRequest& req, lins::HttpResponse& res) -> bool {
                  std::cout << "ok2\n";
                  return true;
                });
  router.insert("/ok3",
                [](lins::HttpRequest& req, lins::HttpResponse& res) -> bool {
                  std::cout << "ok3\n";
                  return true;
                });
  router.exec("/ok1")(req, res);
  router.exec("/ok2")(req, res);
  router.exec("/ok3")(req, res);
  router.exec("/ok4")(req, res);
  return 0;
}