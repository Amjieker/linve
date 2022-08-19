#ifndef LINVE_JSON_H
#define LINVE_JSON_H
#include <math.h>

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
namespace lins {
class Json {
 public:
  enum Json_type {
    J_NULL_TYPE,
    J_BOOL_TYPE,
    J_NUMBER_TYPE,
    J_STRING_TYPE,
    J_LIST_TYPE,
    J_DICT_TYPE
  };

  Json(Json_type type, void* data);

  Json() : type_(J_NULL_TYPE) {}

  Json(bool&& data) : boolean_(std::move(data)), type_(J_BOOL_TYPE) {}

  Json(double&& data) : number_(std::move(data)), type_(J_NUMBER_TYPE) {}

  Json(std::string&& data) : string_(std::move(data)), type_(J_STRING_TYPE) {}

  Json(std::vector<Json>&& data) : list_(std::move(data)), type_(J_LIST_TYPE) {}

  Json(std::map<std::string, Json>&& data)
      : dict_(std::move(data)), type_(J_DICT_TYPE) {}

  Json& operator[](size_t idx);

  Json& operator[](const std::string& key);

  const Json& operator[](size_t idx) const;

  const Json& operator[](const std::string& key) const;

  bool BOOL() { return boolean_; }

  const bool BOOL() const { return boolean_; }

  double NUMBER() { return number_; }

  const double NUMBER() const { return number_; }

  std::string STRING() { return string_; }

  const std::string STRING() const { return string_; }

  std::vector<Json> LIST() { return list_; }

  const std::vector<Json> LIST() const { return list_; }

  std::map<std::string, Json> DICT() { return dict_; }

  const std::map<std::string, Json> DICT() const { return dict_; }

  std::string toString(int flag = true);

 private:
  std::string toNull_(int flag) { return "null"; }

  std::string toBool_(int flag) {
    if (boolean_) {
      return "true";
    } else {
      return "false";
    }
  }

  std::string toString_(int flag) {
    std::string tmp;
    tmp.push_back('\"');
    tmp.append(std::move(string_));
    tmp.push_back('\"');
    return tmp;
  }

  std::string toNumber_(int flag) {
    if (floor(number_) != number_) return std::to_string(number_);
    return std::to_string(static_cast<int>(number_));
  }

  std::string toList_(int flag) {
    std::string tmp;
    tmp.push_back('[');
    for (auto v : list_) {
      tmp.append(std::move(v.toString(flag)));
      tmp.push_back(',');
    }
    tmp.pop_back();
    tmp.push_back(']');
    return tmp;
  }

  std::string toDict_(int flag) {
    std::string tmp;
    tmp.push_back('{');
    for (auto& v : dict_) {
      tmp.push_back('\"');
      tmp.append(std::move(v.first));
      tmp.append("\":");
      tmp.append(std::move(v.second.toString(flag)));
      tmp.push_back(',');
    }
    if (dict_.size()) tmp.pop_back();
    tmp.push_back('}');
    return tmp;
  }

  Json_type type_;                   /* json type*/
  bool boolean_;                     /*bool */
  double number_;                    /*int double float ...*/
  std::string string_;               /*string */
  std::vector<Json> list_;           /*array*/
  std::map<std::string, Json> dict_; /*object*/
};

class JsonParse {
 public:
  JsonParse(const std::string& data) : data_(data), idx_(0) {}
  Json parse();

 private:
  char nextToken_();

  std::string parseStr();

  Json parseNull();

  Json parseBool();

  Json parseNumber();

  Json parseString();

  Json parseList();

  Json parseDict();

  const std::string& data_;
  int idx_; /*idex of data_*/
};

/*json  dump template */

#define BEGIN_DUMP_JSON \
  lins::Json dump() {   \
    std::map<std::string, lins::Json> res;

#define MAPPING(name, jsonName) res.insert({jsonName, lins::json_dump(name)});

#define END_DUMP_JSON                \
  return lins::Json(std::move(res)); \
  }

// struct JSON {
//   virtual lins::Json dump() = 0;
//   virtual void load(const lins::Json& json) = 0;
// };

template <typename T>
lins::Json json_dump(T rsh) {
  return rsh.dump();
}
lins::Json json_dump(int rsh);
lins::Json json_dump(bool rsh);
lins::Json json_dump(std::string rsh);
template <typename T>
lins::Json json_dump(std::vector<T> rhs) {
  std::vector<lins::Json> json;
  for (auto v : rhs) json.push_back(json_dump(v));
  return lins::Json(std::move(json));
}
template <typename VALUE>
lins::Json json_dump(std::map<std::string, VALUE> rhs) {
  std::map<std::string, lins::Json> json;
  for (auto v : rhs) json.insert({v.first, json_dump(v.second)});
  return lins::Json(std::move(json));
}
};  // namespace lins
#endif
