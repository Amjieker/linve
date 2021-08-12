#ifndef DEFER_H
#define DEFER_H
#define defer_str(x, y) x##y
#define defer_contact(x, y) defer_str(x, y)
#define defer_name(x) defer_contact(x, __COUNTER__)

#define defer(expression) \
  auto defer_name(__defer__lins__) = Defer([&]() { expression; })
namespace lins {
template <typename Function>
struct defer_t {
  Function f;
  defer_t(Function f) : f(f) {}
  ~defer_t() { f(); }
};
template <typename Function>
defer_t<Function> Defer(Function f) {
  return defer_t<Function>(f);
}

};  // namespace lins
#endif