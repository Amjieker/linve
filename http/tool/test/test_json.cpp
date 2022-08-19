#include <bits/stdc++.h>
#include <dirent.h>
#include <unistd.h>

#include "../json.cpp"
using namespace std;
struct person {
  std::string name;
  std::string detail;
  int age;
  void load(const lins::Json& json) {
    name = json["name"].STRING();
    detail = json["detail"].STRING();
    age = json["age"].NUMBER();
  }
  BEGIN_DUMP_JSON
  MAPPING(name, "name");
  MAPPING(detail, "detail");
  MAPPING(age, "age");
  END_DUMP_JSON
};
struct student {
  int stuNo;    // 学号
  int classNo;  // 班级
  person info;
  void load(const lins::Json& json) {
    stuNo = json["stuNo"].NUMBER();
    classNo = json["classNo"].NUMBER();
    info.load(json["info"]);
  }
  BEGIN_DUMP_JSON
  MAPPING(stuNo, "stuNo");
  MAPPING(classNo, "classNo");
  MAPPING(info, "info");
  END_DUMP_JSON
};

struct theacher {
  int studentNo;
  person info;
  void load(const lins::Json& json) {
    studentNo = json["studentNo"].NUMBER();
    info.load(json["info"]);
  }
  BEGIN_DUMP_JSON
  MAPPING(studentNo, "studentNo");
  MAPPING(info, "info");
  END_DUMP_JSON
};

struct classroom {
  std::vector<student> stu;
  std::vector<theacher> tch;
  std::map<string, int> mp;
  void load(const lins::Json& json) {
    for (auto& v : json["stu"].LIST()) {
      auto t = student();
      t.load(v);
      stu.push_back(t);
    }
    for (auto& v : json["theacher"].LIST()) {
      auto t = theacher();
      t.load(v);
      tch.push_back(t);
    }
  }
  BEGIN_DUMP_JSON
  MAPPING(stu, "stu");
  MAPPING(tch, "tch");
  MAPPING(mp, "mp");
  END_DUMP_JSON
};

int main() {
  classroom room = \ 
  {
      .stu{
          student{1001, 5, {"张三", "一个学生", 15}},
          student{1002, 5, {"李四", "一个学生", 16}},
          student{1003, 5, {"王五", "学生一个", 19}},
          student{1004, 5, {"坤坤", "唱跳rap篮球", 18}},
          student{1005, 5, {"老六", "一个学生", 19}},
          student{1006, 5, {"青霞", "一个学生", 19}},
      },
      .tch{
          theacher{10086, {"李老师", "语文", 45}},
          theacher{10087, {"张老师", "数学", 35}},
          theacher{10081, {"结衣老师", "老师", 25}},
      },
  };
  auto res = room.dump();
  auto str = res.toString(true);
  cout << lins::JsonParse(str).parse().toString() << endl;

  // std::ifstream fin("json.data");

  // string s((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
  // cout << s << endl;

  // auto t = chrono::high_resolution_clock().now();
  // auto json = lins::JsonParse(s).parse();
  // auto res = chrono::high_resolution_clock().now();
  // cout << std::chrono::duration<double, std::milli>(res - t).count() << endl;

  return 0;
}