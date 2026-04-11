/// Conceptを使って色々書いてみる
/// https://cpprefjp.github.io/lang/cpp20/concepts.html

#include <cstdint>
import std;
#include <concepts>

template <typename T>
concept Drawable = requires(T a) {
  { a.draw() } -> std::same_as<void>;
};

struct Circle {
  uint32_t radius;
  void draw() { std::println("Drawing a circle, radius: {}", radius); }
};

struct Rectanlge {
  uint32_t width;
  uint32_t height;
  void draw() {
    std::println("Drawing a rectangle, width: {}, height: {}", width, height);
  }
};

struct NotDrawableCircle {
  int draw() {
    std::println("This is not a drawable circle");
    return 0;
  }
};

template <Drawable T> struct DrawableShape {
  T shape;

  void draw() {
    std::print("Calling draw on DrawableShape: ");
    shape.draw();
  }
};

int main() {
  auto circle = Circle{.radius = 5};
  auto square = Rectanlge{.width = 10, .height = 15};
  auto notDrawableCircle = NotDrawableCircle{};

  // Rustみたい(Box<Trait>)に動的dispatchをするのは難しそう
  // auto drawableShapes = std::vector<&Drawable>{circle, square};

  static_assert(Drawable<Circle>, "Circle should be drawable");
  static_assert(Drawable<Rectanlge>, "Square should be drawable");
  static_assert(!Drawable<NotDrawableCircle>,
                "NotDrawableCircle should not be drawable");

  auto drawableShapesCircle(DrawableShape<Circle>{circle});
  auto drawableShapesSquare(DrawableShape<Rectanlge>{square});
  // auto
  // drawableShapesNotDrawableCircle(DrawableShape<NotDrawableCircle>{notDrawableCircle});

  drawableShapesCircle.draw();
  drawableShapesSquare.draw();

  if constexpr (Drawable<Circle>) {
    circle.draw();
  }
  if constexpr (Drawable<Rectanlge>) {
    square.draw();
  }
  if constexpr (!Drawable<NotDrawableCircle>) {
    std::println("NotDrawableCircle is not drawable");
  }
}
