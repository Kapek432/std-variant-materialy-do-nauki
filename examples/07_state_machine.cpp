// Cel: przejścia stanów jako funkcja (stan, zdarzenie) -> nowy stan.
#include <iostream>
#include <string>
#include <variant>

struct Disconnected {};
struct Connecting {
    std::string host;
};
struct Connected {
    std::string host;
};
using State = std::variant<Disconnected, Connecting, Connected>;
struct Connect {
    std::string host;
};
struct Established {};
struct Disconnect {};
using Event = std::variant<Connect, Established, Disconnect>;

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

State transition(const State& state, const Event& event) {
    return std::visit(
        overloaded{
            [](const Disconnected&, const Connect& e) -> State {
                return Connecting{e.host};
            },
            [](const Connecting& s, const Established&) -> State {
                return Connected{s.host};
            },
            [](const Connecting&, const Disconnect&) -> State {
                return Disconnected{};
            },
            [](const Connected&, const Disconnect&) -> State {
                return Disconnected{};
            },
            [](const auto& s, const auto&) -> State {
                std::cout << "zdarzenie pominiete\n";
                return s;
            }
        },
        state,
        event
    );
}

void print_state(const State& state) {
    std::visit(
        overloaded{
            [](const Disconnected&) {
                std::cout << "stan: rozlaczony\n";
            },
            [](const Connecting& s) {
                std::cout << "stan: laczenie z " << s.host << '\n';
            },
            [](const Connected& s) {
                std::cout << "stan: polaczony z " << s.host << '\n';
            }
        },
        state
    );
}

int main() {
    State state{Disconnected{}};
    const Event events[] = {
        Established{}, Connect{"example.org"}, Established{}, Connect{"other.org"}, Disconnect{}
    };
    print_state(state);
    for (const auto& event : events) {
        // Zastępujemy stan dopiero po zakończeniu odwiedzania.
        state = transition(state, event);
        print_state(state);
    }
}
