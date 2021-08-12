#include "json.h"

namespace lins {

std::ostream& operator<<(std::ostream& os, const Json& rsh) { return os; }

const Json& Json::operator[](size_t idx) const {
  if (this->type_ != J_LIST_TYPE) throw std::runtime_error("is not array");
  if (idx >= this->list_.size()) throw std::runtime_error("out to range");
  return this->list_[idx];
}

const Json& Json::operator[](const std::string& key) const {
  if (this->type_ != J_DICT_TYPE) throw std::runtime_error("is not dict");
  if (!this->dict_.count(key)) throw std::runtime_error("not found this key");
  return this->dict_.find(key)->second;
}

Json& Json::operator[](size_t idx) {
  return const_cast<Json&>(static_cast<const Json&>(*this)[idx]);
}

Json& Json::operator[](const std::string& key) {
  return const_cast<Json&>(static_cast<const Json&>(*this)[key]);
}

Json::Json(Json_type type, void* data) : type_(type) {
  switch (type) {
    case J_BOOL_TYPE:
      boolean_ = std::move(*static_cast<bool*>(data));
      break;
    case J_NUMBER_TYPE:
      number_ = std::move(*static_cast<double*>(data));
      break;
    case J_STRING_TYPE:
      string_ = std::move(*static_cast<std::string*>(data));
      break;
    case J_LIST_TYPE:
      list_ = std::move(*static_cast<std::vector<Json>*>(data));
      break;
    case J_DICT_TYPE:
      dict_ = std::move(*static_cast<std::map<std::string, Json>*>(data));
      break;
  }
}

std::string Json::toString(int flag) {
  std::string str;
  switch (type_) {
    case J_NULL_TYPE:
      str.append(std::move(toNull_(flag)));
      break;
    case J_BOOL_TYPE:
      str.append(std::move(toBool_(flag)));
      break;
    case J_NUMBER_TYPE:
      str.append(std::move(toNumber_(flag)));
      break;
    case J_STRING_TYPE:
      str.append(std::move(toString_(flag)));
      break;
    case J_LIST_TYPE:
      str.append(std::move(toList_(flag)));
      break;
    case J_DICT_TYPE:
      str.append(std::move(toDict_(flag)));
      break;
  }
  return str;
}

char JsonParse::nextToken_() {
  while (idx_ < data_.size() && std::isspace(data_[idx_])) idx_++;
  if (idx_ >= data_.size()) throw std::logic_error("json grammar error");
  return data_[idx_];
}

Json JsonParse::parse() {
  char token = nextToken_();
  switch (token) {
    case 'n':
      return parseNull();
    case 'f':
    case 't':
      return parseBool();
    case '-':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      return parseNumber();
    case '\"':
      return parseString();
    case '[':
      return parseList();
    case '{':
      return parseDict();
    default:
      throw std::logic_error("json grammar error");
  }
  return Json();
}

Json JsonParse::parseNull() {
  if (!data_.compare(idx_, 4, "null")) {
    idx_ += 4;
    return Json();
  } else {
    throw std::logic_error("json grammar error");
  }
}

Json JsonParse::parseBool() {
  bool flag;
  if (data_[idx_] == 'f' && !data_.compare(idx_, 5, "false")) {
    idx_ += 5;
    flag = false;
    return Json(Json::J_BOOL_TYPE, &flag);
  } else if (data_[idx_] == 't' && !data_.compare(idx_, 4, "true")) {
    idx_ += 4;
    flag = true;
    return Json(std::move(flag));
  } else {
    throw std::logic_error("json grammar error");
  }
}

Json JsonParse::parseNumber() {
  double res = 0;
  char* end;
  res = strtod(&data_[idx_], &end);
  if (&data_[idx_] == end) throw std::logic_error("json grammar error");
  idx_ += end - &data_[idx_];
  return Json(std::move(res));
}

std::string JsonParse::parseStr() {
  auto start = ++idx_;
  auto end = data_.find_first_of('"', start);
  if (end == data_.npos) throw std::logic_error("json grammar error");
  auto check = [&](int pos) -> bool {
    int pre = pos;
    while (pre && data_[pre] == '\\') pre--;
    return (pos - pre) % 2 == 0;
  };
  while (1) {
    if (check(end)) break;
    end = data_.find_first_of('"', end + 1);
    if (end == data_.npos) throw std::logic_error("json grammar error");
  }
  idx_ = end + 1;
  return data_.substr(start, end - start);
}

Json JsonParse::parseString() {
  // std::string tmp = parseStr();
  return Json(parseStr());
}

Json JsonParse::parseList() {
  int flag = false;
  idx_++;
  std::vector<Json> list;
  while (idx_ < data_.size()) {
    char token = nextToken_();
    if (token == ',') {
      idx_++;
      continue;
    }
    if (token == ']') {
      idx_++;
      flag = true;
      break;
    }
    list.push_back(parse());
  }
  if (!flag) throw std::logic_error("json grammar error");
  return Json(std::move(list));
}

Json JsonParse::parseDict() {
  std::map<std::string, Json> dict;
  int flag = false;
  idx_++;
  while (idx_ < data_.size()) {
    char token = nextToken_();
    if (token == ',') {
      idx_++;
      continue;
    }
    if (token == '}') {
      idx_++;
      flag = true;
      break;
    }
    if (token != '\"') throw std::logic_error("json grammar error");
    std::string key = parseStr();
    token = nextToken_();
    if (token != ':') throw std::logic_error("json grammar error");
    idx_++;
    dict.insert({key, parse()});
  }
  if (!flag) throw std::logic_error("json grammar error");
  return Json(std::move(dict));
}

lins::Json json_dump(int rsh) { return lins::Json(static_cast<double>(rsh)); }

lins::Json json_dump(bool rsh) { return lins::Json(rsh); }

lins::Json json_dump(std::string rsh) { return lins::Json(std::move(rsh)); }

}  // namespace lins
