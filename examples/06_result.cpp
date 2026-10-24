// Cel: sukces i błąd jako rozłączne wyniki; parser akceptuje całe wejscie.
#include <iostream>
#include <stdexcept>
#include <string>
#include <variant>

struct Success {
    int value;
};
struct Error {
    std::string message;
};
using Result = std::variant<Success, Error>;

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

Result parse_int(const std::string& input) {
    try {
        std::size_t consumed = 0;
        const int value = std::stoi(input, &consumed);
        if (consumed != input.size()) {
            return Error{"znaki po liczbie"};
        }
        return Success{value};
    } catch (const std::invalid_argument&) {
        return Error{"brak cyfr"};
    } catch (const std::out_of_range&) {
        return Error{"liczba poza zakresem int"};
    }
}

int main() {
    const std::string inputs[] = {"42", "abc", "12x", std::string(100, '9')};
    for (const auto& input : inputs) {
        const Result result = parse_int(input);
        std::visit(
            overloaded{
                [](const Success& s) {
                    std::cout << "sukces: " << s.value << '\n';
                },
                [](const Error& e) {
                    std::cout << "blad: " << e.message << '\n';
                }
            },
            result
        );
    }
}
