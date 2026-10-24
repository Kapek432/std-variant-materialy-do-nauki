// Cel: konfiguracja z zamkniętym zbiorem typów i deterministyczną kolejnością.
#include <iostream>
#include <map>
#include <string>
#include <variant>

int main() {
    using ConfigValue = std::variant<bool, int, std::string>;
    const std::map<std::string, ConfigValue> config{
        {"debug", true}, {"retries", 3}, {"server", std::string{"localhost"}}
    };
    std::cout << std::boolalpha;
    for (const auto& entry : config) {
        std::cout << entry.first << " = ";
        std::visit(
            [](const auto& x) {
                std::cout << x;
            },
            entry.second
        );
        std::cout << '\n';
    }
}
