// Cel: odróżnienie odczytu rzucającego wyjątek od sprawdzania wskaźnika.
#include <iostream>
#include <string>
#include <variant>

int main() {
    std::variant<int, std::string> value{7};
    std::cout << "get<int>: " << std::get<int>(value) << '\n';
    std::cout << std::boolalpha << "holds string: " << std::holds_alternative<std::string>(value)
              << '\n';
    if (std::get_if<std::string>(&value) == nullptr) {
        std::cout << "get_if<string>: nullptr\n";
    }
    try {
        (void)std::get<std::string>(value);
    } catch (const std::bad_variant_access&) {
        std::cout << "get<string>: bad_variant_access\n";
    }
    value = std::string{"gotowe"};
    if (const auto* text = std::get_if<std::string>(&value)) {
        std::cout << "get_if<string>: " << *text << '\n';
    }
}
