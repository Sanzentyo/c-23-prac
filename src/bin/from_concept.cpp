// RustのFrom<T> Traitのようなものを実装してみる
import std;
#include "../lib/practice_support.hpp"

template <typename T, typename U>
concept From = requires(T t, U u) {
  { T::from(u) } -> std::same_as<T>;
};

template <typename T, typename U>
  requires(
      From<
          T,
          U>) // requiresでFrom<T,
              // U>を満たすことを要求(こちらのtemplateの方ではなく、呼び出し側でエラーが出るようになる)
auto into(U u) -> T {
  // static_assert(From<T, U>, "T must implement From<U>");
  // これでもできるが、エラーがわかりにくい
  return T::from(u);
}

struct MyInt {
  int value;

  static MyInt from(int v) { return MyInt{v}; }
};

// formatterをMyIntへ特殊化
template <> struct std::formatter<MyInt> {
  constexpr std::format_parse_context::iterator parse(
      std::format_parse_context &ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  typename FormatContext::iterator format(const MyInt &myInt,
                                          FormatContext &ctx) const {
    return std::format_to(ctx.out(), "{}", myInt.value);
  }
};


auto main() -> int {
  practice::print_heading("from_concept");
  static_assert(From<MyInt, int>, "MyInt should implement From<int>");

  auto myInt = into<MyInt>(42);
  // auto int_value = into<int>(myInt);
  
  std::println("MyInt {}", myInt);
}
