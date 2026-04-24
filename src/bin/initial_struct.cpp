// 初期化時のコンストラクタの実行順序を確認

import std;
#include "../lib/practice_support.hpp"

struct PrintOnConstruct {
  PrintOnConstruct(std::string &&message) {
    std::println("Constructed with message: {}", message);
  }
};

struct Hoge {
  PrintOnConstruct printOnConstruct;
  Hoge(std::function<PrintOnConstruct(std::string)> fn,
       std::string&& id)
      : printOnConstruct(fn("from Hoge: id: " + id)) {}
};

auto main() -> int {
  practice::print_heading("initial_struct");
  auto hoge = Hoge{[](std::string &&message) {
    return PrintOnConstruct{std::move(message)};
  }, "Hoge"};
}
