// std::formattable コンセプトのデモ
// C++23: <format> で定義されている

import std;
import practice_support;

// ============================================================
// 1. カスタム型の定義
// ============================================================

struct Point {
    int x, y;
};

// formatter を特殊化していない型 → std::formattable を満たさない
struct RawData {
    int value;
};

// ============================================================
// 2. std::formatter を特殊化 → std::formattable を満たすようになる
// ============================================================
template <>
struct std::formatter<Point> {
    // parse: フォーマット指定子を解析する ("" のみ受け付ける)
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    // format: 実際の出力を生成する (FormatContext はテンプレートにする必要がある)
    template <typename FormatContext>
    auto format(const Point& p, FormatContext& ctx) const {
        return std::format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

// ============================================================
// 3. std::formattable を制約に使う関数テンプレート
// ============================================================

// --- パターン A: 明示的テンプレート引数 ---
// std::formattable<T, char> : T が char 向けにフォーマット可能であることを要求
template <std::formattable<char> T>
void show(std::string_view label, const T& value) {
    std::println("  {}: {}", label, value);
}

// --- パターン B: 短縮記法 (abbreviated function template) ---
// `std::formattable<char> auto` は上の template<...> と等価
void show_abbrev(std::string_view label, std::formattable<char> auto const& value) {
    std::println("  [abbrev] {}: {}", label, value);
}

// ============================================================
// 4. コンパイル時検証 (static_assert)
// ============================================================

// 標準型は最初から formattable
static_assert(std::formattable<int,         char>);
static_assert(std::formattable<double,      char>);
static_assert(std::formattable<std::string, char>);
static_assert(std::formattable<const char*, char>);

// formatter を特殊化した型も formattable になる
static_assert(std::formattable<Point, char>);

// formatter を特殊化していない型は formattable でない
static_assert(!std::formattable<RawData, char>);

// ============================================================
// main
// ============================================================
auto main() -> int {
    practice::print_heading("formattable");

    std::println("-- 標準型 (最初から formattable) --");
    show("int",    42);
    show("double", 3.14);
    show("string", std::string{"hello"});

    std::println("-- カスタム型 (formatter 特殊化済み) --");
    show("Point", Point{3, 7});
    show_abbrev("Point", Point{1, 2});

    // RawData は formattable でないのでコンパイルエラーになる:
    // show("RawData", RawData{99});  // ← コメントを外すとコンパイルエラー
}