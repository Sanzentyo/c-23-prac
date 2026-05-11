import std;

template<typename Shape>
concept ShapeConcept = requires {
    { std::declval<Shape>().draw() } -> std::same_as<void>;
};

struct Circle {
    double radius;
    void draw() const {
        std::println("Circle: radius = {}", radius);
    }
};

struct Rectangle {
    double width;
    double height;
    void draw() const {
        std::println("Rectangle: width = {}, height = {}", width, height);
    }
};

struct CurvedRectangle {
    double width;
    double height;
    double radius;
    void draw() const {
        std::println("CurvedRectangle: width = {}, height = {}, radius = {}", width, height, radius);
    }
};

struct NonDrawable {
};


template <typename... Ts>
requires (ShapeConcept<Ts> && ...)
using ShapeVariant = std::variant<Ts...>;

using DrawableShapeVariant = ShapeVariant<Circle, Rectangle, CurvedRectangle>;


auto main() -> int {
    auto shapes = std::array<DrawableShapeVariant, 3>{
        Circle{1.0},
        Rectangle{2.0, 3.0},
        CurvedRectangle{4.0, 5.0, 6.0},
    };
    for (auto& shape : shapes) {
        std::println("index = {}", shape.index());
        std::visit([](auto&& s) { s.draw(); }, shape);
    }
    return 0;
}
