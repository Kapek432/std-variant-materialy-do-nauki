// Cel: osobne przeciążenia i jawny wspólny typ wyniku odwiedzającego.
#include <iostream>
#include <sstream>
#include <string>
#include <variant>

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

int main() {
    using Value = std::variant<int, double, std::string>;
    const auto print = overloaded{
        [](int x) {
            std::cout << "int: " << x << '\n';
        },
        [](double x) {
            std::cout << "double: " << x << '\n';
        },
        [](const std::string& x) {
            std::cout << "string: " << x << '\n';
        }
    };
    const auto as_text = [](const auto& x) -> std::string {
        std::ostringstream out;
        out << x;
        return out.str();
    };
    const Value values[] = {Value{4}, Value{2.5}, Value{std::string{"kot"}}};
    for (const auto& value : values) {
        std::visit(print, value);
    }
    std::cout << "dlugosci opisow:";
    for (const auto& value : values) {
        std::cout << ' ' << std::visit(as_text, value).size();
    }
    std::cout << '\n';
}
