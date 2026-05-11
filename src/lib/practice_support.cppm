export module practice_support;

import std;

export namespace practice {

void print_heading(std::string_view heading) {
    std::println("== {} ==", heading);
}

} // namespace practice