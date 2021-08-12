#include <bits/stdc++.h>

#include "../httpRequest.h"
/*
GET /nccommon/token/login-other-place?token=&_=1659065785740 HTTP/1.1\r\n
Host: www.nowcoder.com\r\n
Connection: keep-alive\r\n
sec-ch-ua: "Google Chrome";v="103", "Chromium";v="103"\r\n
Accept: text/plain, *\/*; q=0.01\r\n
X-Requested-With: XMLHttpRequest\r\n
sec-ch-ua-mobile: ?0\r\n
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML,
like Gecko) Chrome/103.0.0.0 Safari/537.36 sec-ch-ua-platform: "Windows"\r\n
Sec-Fetch-Site: same-origin\r\n
Sec-Fetch-Mode: cors\r\n
Sec-Fetch-Dest: empty\r\n
Referer: https://www.nowcoder.com/study/live/504/5/7?autoplay=1\r\n
Accept-Encoding: gzip, deflate, br\r\n
Accept-Language: zh-CN,zh;q=0.9\r\n
*/
int main() {
  lins::HttpRequest request;
  lins::Buffer buf;
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
  request.parse(buf);
  std::cout << request.mathod() << "\n";
  std::cout << request.path() << "\n";
  std::cout << request.version() << "\n";
  std::cout << request.body() << "\n";
  std::cout << request.isKeepAlive() << " " << request.getHeader("Connection")
            << "\n";
  std::cout << buf.toString() << "\n";
  // std::string str = request.path();
  // auto t = str.find('?');
  // if (t != str.npos) {
  //   std::string old = str.substr(0, t);
  //   t++;
  //   while (t < str.size()) {
  //     int end = str.find_first_of('&', t);
  //     if (end == str.npos) {
  //       end = str.size();
  //     }
  //     auto div = str.find_first_of('=', t);
  //     std::cout << str.substr(t, div - t) << " "
  //               << str.substr(div + 1, end - div - 1) << std::endl;
  //     t = end + 1;
  //   }
  // }
  return 0;
}