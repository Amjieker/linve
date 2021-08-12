#include <bits/stdc++.h>

#include "../httpResponse.h"

using namespace std;
/*
HTTP/1.1 200 OK
Server: Apache-Coyote/1.1
Content-Type: text/html;charset=ISO-8859-1
Content-Length: 105
Date: Tue, 27 May 2014 16:23:28 GMT

<html>
<head>
<title>Hello World JSP</title>
</head>
<body>
Hello World!
</body>
</html>
*/
template <typename T>
std::string toString(T&& res) {
  std::ostringstream os;
  os << res;
  return os.str();
}
int main() {
  lins::HttpResponse http;
  lins::Buffer buf;
  http.init(true, lins::HttpResponse::Ok200);
  http.addHeader("Server", "lins-WebServer/1.1");
  http.addHeader("Content-Type", "text/html;charset=utf-8");
  auto t =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  http.addHeader("Date", toString(std::put_time(std::localtime(&t), "%F %T")));
  http.setBody("{data:\"你好\"}");
  http.toBuffer(buf);
  cout << buf.toString() << endl;
  return 0;
}