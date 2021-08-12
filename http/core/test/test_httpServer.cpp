#include <bits/stdc++.h>

#include "../httpServer.h"

using namespace std;
int main() {
  lins::Log::instance()->init(-1, false);
  lins::HttpServer server(10000, 3, 2000, false);
  server.addPage("/admin/hello",
                 [](lins::HttpRequest& req, lins::HttpResponse& res) -> bool {
                   res.setBody("this is my page");
                   return true;
                 });
  server.addPage(
      "/api/json", [](lins::HttpRequest& req, lins::HttpResponse& res) -> bool {
        res.addHeader("Content-Type", "application/json");
        res.setBody(
            "{\"paging\": {\"is_end\": true, \"totals\": 0, \"previous\": "
            "\"http://www.zhihu.com/api/v4/questions/32297466/"
            "concerned_followers?limit=7&offset=0\", \"is_start\": true, "
            "\"next\": "
            "\"http://www.zhihu.com/api/v4/questions/32297466/"
            "concerned_followers?limit=7&offset=7\"}, \"data\": []}");
        return true;
      });
  server.Start();
  return 0;
}