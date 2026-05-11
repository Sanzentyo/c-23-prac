import std;
import practice_support;

// 複数のラムダを1つの callable として受け渡すためのユーティリティ。
// ADT の各ケースを同じ形で処理する visit に使う。
template <typename... Ts>
struct overloaded : Ts... {
  using Ts::operator()...;
};

// クラス template の推論補助。`overloaded{...}` で書けるようにする。
template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// ここから下は「積」（product）型に相当する構造体。
struct Circle {
  double radius;
};

struct Rectangle {
  double width;
  double height;
};

struct Triangle {
  double base;
  double height;
};

struct Point {
  double x;
  double y;
};

// ここが「和」（sum）型。Circle / Rectangle / Triangle / Point のどれかを保持する。
using Shape = std::variant<Circle, Rectangle, Triangle, Point>;

// どのケースでも実行される処理を visit で分岐させる。
// ADT的には「場合分け」をコンパイラに見せる部分。
double area(const Shape &shape) {
  return std::visit(overloaded{
                        [](const Circle &circle) { return circle.radius * circle.radius * std::numbers::pi; },
                        [](const Rectangle &rectangle) { return rectangle.width * rectangle.height; },
                        [](const Triangle &triangle) { return 0.5 * triangle.base * triangle.height; },
                        [](const Point &) { return 0.0; },
                    },
                    shape);
}

// 型判定を文字列に変換してデバッグしやすくするユーティリティ。
std::string_view kind(const Shape &shape) {
  return std::visit(
      overloaded{
          [](const Circle &) { return "Circle"; },
          [](const Rectangle &) { return "Rectangle"; },
          [](const Triangle &) { return "Triangle"; },
          [](const Point &) { return "Point"; },
      },
      shape);
}

// ケースごとの詳細表示。visit はコンパイル時に網羅性を担保しやすい。
void describe(const Shape &shape) {
  std::visit(
      overloaded{
          [](const Circle &circle) { std::println("  Circle(r={})", circle.radius); },
          [](const Rectangle &rectangle) {
            std::println("  Rectangle({}x{})", rectangle.width, rectangle.height);
          },
          [](const Triangle &triangle) {
            std::println("  Triangle(base={}, height={})", triangle.base, triangle.height);
          },
          [](const Point &point) { std::println("  Point({}, {})", point.x, point.y); },
      },
      shape);
}

int main() {
  practice::print_heading("adt");
  std::println("ADTデモ: std::variant で和型、struct で積型を表現");

  // 配列に格納するときも、要素はすべて Shape という ADT として統一される。
  auto shapes = std::array<Shape, 4>{
      Circle{2.0},
      Rectangle{3.0, 4.0},
      Triangle{5.0, 3.0},
      Point{1.0, 2.0},
  };

  // それぞれの値を判別して、面積と内容を表示する。
  for (const auto &shape : shapes) {
    std::println("shape={} area={:.2f}", kind(shape), area(shape));
    describe(shape);
  }
}
