// Cel: pusty stan dziedzinowy, duplikaty i gwarantowany przypadek utraty wartości.
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>

struct NoDefault {
    explicit NoDefault(int number) : value(number) {}
    int value;
};

struct ThrowsOnMove {
    ThrowsOnMove() = default;
    ThrowsOnMove(const ThrowsOnMove&) = default;
    ThrowsOnMove& operator=(const ThrowsOnMove&) = default;
    ThrowsOnMove(ThrowsOnMove&&) noexcept(false) {
        throw std::runtime_error{"move failed"};
    }
    ThrowsOnMove& operator=(ThrowsOnMove&&) = default;
};

int main() {
    std::variant<std::monostate, NoDefault> maybe;
    assert(std::holds_alternative<std::monostate>(maybe));
    std::cout << "monostate: " << maybe.index() << '\n';
    maybe.emplace<NoDefault>(7);
    std::cout << "NoDefault: " << std::get<NoDefault>(maybe).value << '\n';

    std::variant<int, int> repeated{std::in_place_index<1>, 9};
    std::cout << "duplikaty: index=" << repeated.index() << ", value=" << std::get<1>(repeated)
              << '\n';

    using Value = std::variant<int, ThrowsOnMove>;
    Value target{42};
    Value source{std::in_place_index<1>};
    try {
        // Różne indeksy: przypisanie przenoszące konstruuje nową alternatywę
        target = std::move(source);
    } catch (const std::runtime_error&) {
        std::cout << "wyjatek przy przenoszeniu\n";
    }
    assert(target.valueless_by_exception());
    assert(target.index() == std::variant_npos);
    assert(std::get_if<int>(&target) == nullptr);
    std::cout << std::boolalpha << "valueless: " << target.valueless_by_exception() << '\n';
    try {
        std::visit(
            [](const auto&) {
            },
            target
        );
    } catch (const std::bad_variant_access&) {
        std::cout << "visit: bad_variant_access\n";
    }
    target = 10;
    assert(!target.valueless_by_exception());
    std::cout << "po naprawie: " << std::get<int>(target) << '\n';

    using Stored = std::variant<int, double, std::string>;
    static_assert(sizeof(Stored) >= sizeof(int));
    static_assert(sizeof(Stored) >= sizeof(double));
    static_assert(sizeof(Stored) >= sizeof(std::string));
    static_assert(alignof(Stored) >= alignof(std::string));
    std::cout << "sizeof wariantu: " << sizeof(Stored) << '\n';
    std::cout << "sizeof string: " << sizeof(std::string) << '\n';
    std::cout << "alignof wariantu: " << alignof(Stored) << '\n';
}
