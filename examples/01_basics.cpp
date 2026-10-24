// Cel: zmiana aktywnej alternatywy bez osobnej flagi typu.
#include <iostream>
#include <string>
#include <variant>

int main() {
    std::variant<int, double, std::string> value{42};
    std::cout << "int: " << std::get<int>(value) << ", index: " << value.index() << '\n';
    value = std::string{"tekst"};
    std::cout << "string: " << std::get<2>(value) << ", index: " << value.index() << '\n';
    value.emplace<double>(3.5);
    std::cout << "double: " << std::get<double>(value) << ", index: " << value.index() << '\n';
}
