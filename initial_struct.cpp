// 初期化時のコンストラクタの実行順序を確認

#include <functional>
import std;

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
  auto hoge = Hoge{[](std::string &&message) {
    return PrintOnConstruct{std::move(message)};
  }, "Hoge"};
}
