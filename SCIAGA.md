# std::variant - krótka ściąga (C++17)

- **Jedna wartość naraz, spośród ustalonej listy typów.** 

- Nagłówki: `<variant>`, dla tekstu `<string>`. 

- Typ zmiennej się nie zmienia, zmienia się przechowywana alternatywa.

```cpp
using Value = std::variant<int, std::string>;
Value v{42}; // int, indeks 0
v = std::string{"kot"}; // tekst, indeks 1
v.emplace<int>(7); // zastąpienie tekstu liczbą
```

## Sprawdzanie i odczyt

- `v.index()` - numer aktywnej alternatywy, liczony od zera; w stanie bez wartości zwraca `std::variant_npos`.

- `std::holds_alternative<int>(v)` - sprawdza typ, zwraca `bool`.

- `std::get<int>(v)` - dostęp przez referencję; niezgodna aktywna alternatywa powoduje `std::bad_variant_access`.

- `std::get_if<int>(&v)` - wskaźnik do wartości lub `nullptr`; przed dereferencją sprawdź wynik.

- `std::get<0>(v)` - dostęp przez indeks; przydatny przy powtórzonych typach.

```cpp
if (const auto* x = std::get_if<int>(&v)) {
    std::cout << *x;
}
```

**Wybór:** `get_if`, gdy inny typ jest oczekiwaną sytuacją; `get`, gdy chcesz odczytać konkretną alternatywę z kontrolą przez wyjątek. Odczyt nie konwertuje wartości na żądany typ.

## Przetwarzanie przez visit

```cpp
std::visit(
    [](const auto& x) {
        std::cout << x;
    },
    v
);
```

Odwiedzający musi obsłużyć każdy dopuszczalny typ; przy kilku wariantach - każdą kombinację typów. W C++17 wszystkie wywołania muszą mieć zgodny typ i kategorię wyniku. Osobne lambdy można połączyć pomocniczym `overloaded` z [materiału głównego](PRZEWODNIK.md#5-overloaded-kilka-lambd-jako-jeden-odwiedzający).

## Najważniejsze pułapki

- Domyślnie powstaje pierwsza alternatywa, np. `int` o wartości 0. Pierwszy typ musi pozwalać na domyślną konstrukcję.

- `std::monostate` jako pierwsza alternatywa pozwala jawnie modelować pusty stan.

- `std::variant<int, int>` jest dozwolony; konstrukcję i odczyt rozróżniaj indeksami. `get<int>` wymaga dokładnie jednego wystąpienia typu.

- `valueless_by_exception()` oznacza brak wartości po niektórych wyjątkach podczas zmiany alternatywy. To inna sytuacja niż `monostate`; `visit` rzuca wtedy `std::bad_variant_access`.

- Nie używaj wskaźnika lub referencji do alternatywy po zakończeniu życia wskazywanego obiektu, np. po zmianie na inny typ.

- Rozmiar wariantu zależy od typów i implementacji. Sam wariant przechowuje alternatywę wewnątrz obiektu; np. przechowywany `std::string` może osobno alokować pamięć.

**Typowe zastosowania:** ustawienia o kilku typach, sukces albo błąd, zamknięty zbiór stanów. 

**Wybór narzędzia:** 
 
- brak lub jedna wartość -> `optional`

- jedna z kilku wartości o różnych typach -> `variant`

- rozszerzalna hierarchia obiektów -> rozważ polimorfizm.

[Pełne objaśnienia](PRZEWODNIK.md) · [Notebooki z wynikami](notebooks/00_start.ipynb) · [Źródła](references.md)
