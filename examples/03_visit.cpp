// Cel: jedna operacja dla każdej alternatywy oraz rozgałęzienie w kompilacji.
#include <iostream>
#include <string>
#include <type_traits>
#include <variant>

int main() {
    std::variant<int, std::string> value{12};
    const auto print = [](const auto& item) {
        std::cout << "wartosc: " << item << '\n';
    };
    const auto describe = [](const auto& item) {
        using T = std::decay_t<decltype(item)>;
        if constexpr (std::is_same_v<T, std::string>) {
            std::cout << "dlugosc: " << item.size() << '\n';
        } else {
            std::cout << "liczba + 1: " << item + 1 << '\n';
        }
    };
    std::visit(print, value);
    std::visit(describe, value);
    value = std::string{"kot"};
    std::visit(print, value);
    std::visit(describe, value);
}
