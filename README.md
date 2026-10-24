# `std::variant` - jedna wartość, kilka możliwych typów

- Materiały do tematu **11. std::variant**, Zaawansowany C++. 

- Wszystkie programy napisano w **C++17**. 

- Potrzebujesz podstaw funkcji, struktur i referencji.

- Lambdy oraz użyte tu elementy szablonów wyjaśniamy po drodze.

- Czytaj części A i B jako wprowadzenie, a część C jako rozszerzenie. Najwięcej daje uruchamianie przykładów i przewidywanie wyniku przed uruchomieniem. 

- Po lekturze powinieneś umieć dobrać `variant` do problemu, bezpiecznie odczytać wartość, obsłużyć alternatywy przez `visit` i rozpoznać najważniejsze ograniczenia.

## Wersje materiału

- [Notebook demonstracyjny](variant.ipynb) - te same objaśnienia, osiem uruchamialnych programów i zapisane wyniki.
- [Krótka ściąga](SCIAGA.md) - operacje, wybór sposobu odczytu i najważniejsze pułapki.

Notebook korzysta z kernela Python 3 i lokalnego kompilatora C++17. Kod C++ jest osadzony w notebooku, a każda komórka `%%cpp` tworzy osobny program z własnym `main()`. Zapisane wyniki i diagramy można czytać bez wykonywania kodu. Instrukcja uruchomienia znajduje się na początku notebooka.

## Uruchomienie

Potrzebujesz kompilatora i biblioteki standardowej obsługujących C++17. W terminalu przejdź do katalogu repozytorium, a potem wykonaj:

```bash
c++ -std=c++17 -Wall -Wextra -Wpedantic examples/01_basics.cpp -o /tmp/variant_basics
/tmp/variant_basics
```

Dla innego programu zmień nazwę źródła. Każdy plik ma własne `main()`: kompiluj je osobno, nie łącz wszystkich `.cpp` w jeden program. Polecenia zakładają system z katalogiem `/tmp`; w Windows wybierz własną ścieżkę pliku wykonywalnego.

Cały pakiet można sprawdzić w Bashu:

```bash
bash check.sh
CXX=clang++ bash check.sh
CXX=g++-15 bash check.sh
```

Ostatnie polecenie wymaga zainstalowanego GCC pod tą nazwą. Skrypt tworzy i usuwa tymczasowy katalog, włącza ostrzeżenia traktowane jako błędy, uruchamia programy i porównuje wyniki z plikami w [`expected/`](expected/). Nie porównuje zależnych od platformy pomiarów rozmiaru z przykładu 08.

Krótkie fragmenty kodu w tekście pokazują wybrane instrukcje i pomijają nagłówki lub otaczającą funkcję. Kompletne programy znajdują się w `examples/`.

Pliki `.clang-format` i `.clang-tidy` opisują styl i kontrolę statyczną kodu w `examples/`. Nie są potrzebne do nauki. Formatowanie: `clang-format -i examples/*.cpp`. Analiza: `clang-tidy examples/01_basics.cpp -- -std=c++17`.

## A. Podstawy: po co i jak?

### 1. Co daje `std::variant`?

Zwykła zmienna ma jeden ustalony typ. Na przykład `int wynik = 42;` przechowuje liczbę całkowitą. Nie możesz później przypisać do niej tekstu `"brak wyniku"`.

Załóżmy jednak, że piszesz program obsługujący ustawienia aplikacji. Jedno ustawienie jest liczbą, np. liczba prób połączenia `3`. Inne jest tekstem, np. nazwa serwera `"localhost"`. Chcesz mieć wspólny typ, którym możesz opisać **wartość pojedynczego ustawienia**.

`std::variant` pozwala zadeklarować taki typ:

```cpp
std::variant<int, std::string> wartosc{42};
```

Przeczytaj tę linię tak: **„Zmienna `wartosc` może przechowywać liczbę typu `int` albo tekst typu `std::string`. Na początku przechowuje liczbę 42”.**

Rozłóżmy zapis na części:

- `std::variant` to narzędzie z biblioteki standardowej C++.
- `<int, std::string>` to lista dozwolonych typów. Ustalasz ją przy deklaracji.
- `wartosc` to nazwa zmiennej.
- `{42}` podaje początkową wartość.

W takim wariancie przechowujesz **jedną wartość naraz**. Gdy zapiszesz tekst, zastąpi on poprzednią liczbę.

```cpp
std::variant<int, std::string> wartosc{42}; // teraz liczba
wartosc = std::string{"localhost"}; // teraz tekst
```

`std::string{"localhost"}` tworzy obiekt typu `std::string` z podanego tekstu. Dzięki jawnemu zapisowi widać, który typ chcemy umieścić w wariancie.

**Typ samej zmiennej `wartosc` się nie zmienia:** cały czas jest to `std::variant<int, std::string>`. Zmienia się to, który z dwóch dozwolonych typów jest w niej aktualnie przechowywany.

Każdy typ z listy nazywamy **alternatywą**. „Aktywna alternatywa” oznacza po prostu typ przechowywany w tej chwili: najpierw `int`, potem `std::string`.

#### Dlaczego nie zapisać po prostu dwóch pól?

Można stworzyć strukturę z polami `int liczba`, `std::string tekst` i dodatkową informacją, z którego pola należy korzystać. Wtedy oba pola istnieją jednocześnie, a programista musi pilnować tej dodatkowej informacji.

`variant` sam pamięta, którą wartość przechowuje, i kontroluje dostęp do niej. Nie musisz osobno utrzymywać flagi „teraz używamy tekstu”. W części B wykorzystamy to do przechowywania ustawień oraz wyniku operacji: sukcesu albo błędu.

### 2. Pierwszy program: zapis i odczyt

Poniższy program jest kompletny - możesz go wkleić do osobnego pliku `.cpp` i uruchomić w C++17:

```cpp
#include <iostream>
#include <string>
#include <variant>

int main() {
    std::variant<int, std::string> wartosc{42};
    std::cout << std::get<int>(wartosc) << '\n';

    wartosc = std::string{"localhost"};
    std::cout << std::get<std::string>(wartosc) << '\n';
}
```

Wynik:

```text
42
localhost
```

Nagłówek `<variant>` udostępnia `std::variant` i funkcje dostępu do wartości. `<string>` jest potrzebny dla `std::string`, a `<iostream>` do wypisywania.

Zwróć uwagę na dwie operacje:

1. **Zapis:** `wartosc = std::string{"localhost"};` zastępuje liczbę tekstem.
2. **Odczyt:** `std::get<std::string>(wartosc)` pobiera przechowywany tekst.

W zapisie `std::get<int>(wartosc)` część `<int>` oznacza: **„chcę odczytać liczbę typu `int`”**. Nie jest to polecenie zamiany tekstu na liczbę. Jeśli w środku jest tekst, taki odczyt się nie powiedzie. Za chwilę pokażemy, jak to sprawdzać.

`get` daje dostęp przez referencję do wartości znajdującej się w wariancie. W powyższym programie po prostu przekazujemy ją do `std::cout`.

#### Numer przechowywanego typu: `index()`

Typy na liście są ponumerowane od zera. Dla `std::variant<int, std::string>`:

- indeks **0** oznacza `int`;
- indeks **1** oznacza `std::string`.

```cpp
std::variant<int, std::string> wartosc{42};
std::cout << wartosc.index() << '\n'; // 0
wartosc = std::string{"localhost"};
std::cout << wartosc.index() << '\n'; // 1
```

Możesz też użyć numeru do odczytu: `std::get<0>(wartosc)` wybiera pierwszą alternatywę, a `std::get<1>(wartosc)` drugą. Numer oznacza **pozycję typu na liście**, nie przechowywaną liczbę. Zwykle zapis przez typ, np. `get<std::string>`, łatwiej zrozumieć.

#### Inny sposób zapisania wartości: `emplace`

Przypisanie, które już znasz, wystarczy w wielu sytuacjach. `emplace` pozwala wskazać typ i przekazać argumenty do utworzenia nowej wartości bezpośrednio w wariancie:

```cpp
std::variant<int, std::string> wartosc{42};
wartosc.emplace<std::string>("localhost");
std::cout << std::get<std::string>(wartosc) << '\n'; // localhost
```

Tutaj `emplace<std::string>("localhost")` zastępuje dotychczasową liczbę nowym tekstem. Po tej operacji poprzedniej liczby 42 już w wariancie nie ma.

#### Co się stanie bez podania wartości początkowej?

```cpp
std::variant<int, std::string> wartosc;
std::cout << std::get<int>(wartosc) << '\n'; // 0
```

Wariant zaczyna od **pierwszego typu z listy**. W tym przypadku jest to `int` o wartości 0. Nie zaczyna jako „pusty”. Jeżeli pierwszy typ nie pozwala na utworzenie obiektu bez argumentów, taka deklaracja nie będzie dostępna. Rozwiązanie tego przypadku omawiamy w części C.

Teraz uruchom [`01_basics.cpp`](examples/01_basics.cpp). Wprowadza on dodatkowo trzeci typ, `double`, czyli liczbę zmiennoprzecinkową. Mechanizm jest ten sam; lista typów jest dłuższa:

```text
int: 42, index: 0
string: tekst, index: 2
double: 3.5, index: 1
```

Tekst ma tu indeks 2, ponieważ w tym pliku deklaracja ma kolejność `int, double, std::string`.

![Zmiana aktywnej alternatywy: liczba 42 zostaje zastąpiona tekstem localhost, indeks zmienia się z 0 na 1.](diagrams/01-alternatywa.png)

Diagram dotyczy wariantu z dwoma typami z pierwszego programu w tekście. W pliku `01_basics.cpp` lista ma trzy typy, więc indeks tekstu wynosi 2.

### 3. Skąd wiedzieć, co jest w środku?

Załóżmy, że otrzymujesz wariant od innej części programu. Nie wiesz, czy przechowuje liczbę, czy tekst. Masz trzy przydatne narzędzia.

#### `holds_alternative`: sprawdź typ

`std::holds_alternative<int>(wartosc)` odpowiada na pytanie: **„Czy w tej chwili w środku jest `int`?”**. Wynikiem jest `true` albo `false`.

```cpp
std::variant<int, std::string> wartosc{42};
if (std::holds_alternative<int>(wartosc)) {
    std::cout << std::get<int>(wartosc) << '\n'; // 42
}
```

Najpierw sprawdzamy typ, a potem odczytujemy pasującą wartość. Samo `holds_alternative` niczego nie pobiera ani nie zmienia.

#### `get_if`: spróbuj uzyskać dostęp do wartości

`std::get_if<int>(&wartosc)` próbuje uzyskać dostęp do liczby. Zwraca **wskaźnik do liczby**, jeśli jest ona w środku. Jeżeli w środku jest tekst, zwraca `nullptr`, czyli pusty wskaźnik.

```cpp
std::variant<int, std::string> wartosc{42};
int* liczba = std::get_if<int>(&wartosc);

if (liczba != nullptr) {
    std::cout << *liczba << '\n'; // 42
} else {
    std::cout << "W środku nie ma liczby\n";
}
```

Znaczenie symboli w tym przykładzie:

- `&wartosc` przekazuje adres wariantu - tego wymaga `get_if`.
- `int* liczba` deklaruje wskaźnik do `int`.
- `liczba != nullptr` sprawdza, czy uzyskaliśmy dostęp do liczby.
- `*liczba` odczytuje wartość spod wskaźnika. Robimy to dopiero po sprawdzeniu, że wskaźnik nie jest pusty.

Jeżeli w pierwszej linii wpiszesz `std::string{"localhost"}` zamiast `42`, wykona się gałąź `else`. `get_if` nie rzuca wyjątku tylko dlatego, że w środku jest inny dozwolony typ.

#### `get`: odczytaj albo otrzymaj wyjątek

`get` już poznaliśmy. Gdy prosisz o typ, który jest na liście, ale nie jest teraz przechowywany, rzuca wyjątek `std::bad_variant_access`.

```cpp
std::variant<int, std::string> wartosc{42};
try {
    std::cout << std::get<std::string>(wartosc) << '\n';
} catch (const std::bad_variant_access&) {
    std::cout << "Nie mozna odczytac tekstu: w srodku jest liczba\n";
}
```

`try` obejmuje operację, która może rzucić wyjątek. `catch` przechwytuje wskazany rodzaj wyjątku, dzięki czemu ten program wypisze komunikat zamiast zakończyć się z nieobsłużonym wyjątkiem.

**Którego narzędzia użyć?** Gdy chcesz tylko sprawdzić typ, wybierz `holds_alternative`. Gdy inny typ jest spodziewaną sytuacją i potrzebujesz wartości, wygodne jest `get_if`. `get` daje bezpośredni dostęp z kontrolą przez wyjątek, jeśli wybrana alternatywa nie pasuje.

Uruchom [`02_access.cpp`](examples/02_access.cpp), który pokazuje te trzy narzędzia. Oczekiwany wynik:

```text
get<int>: 7
holds string: false
get_if<string>: nullptr
get<string>: bad_variant_access
get_if<string>: gotowe
```

#### Dwie rzeczy, na które trzeba uważać

**Typ musi być na liście.** Dla `std::variant<int, std::string>` próba użycia `std::get<double>` jest błędem kompilacji, bo `double` nie jest dozwoloną alternatywą. Tego błędu nie przechwycisz przez `catch`: program nie zostanie zbudowany. Ta sama zasada dotyczy `get_if` i `holds_alternative`.

**Nie zachowuj dostępu do starej wartości po jej zniszczeniu.** Jeśli `get_if` zwróci wskaźnik do liczby, a później zastąpisz ją tekstem, nie wolno już używać tego wskaźnika do odczytu poprzedniej liczby. Dotyczy to także referencji uzyskanej przez `get`.

W części C poznasz szczególny przypadek, w którym wyjątek podczas zmiany wartości może pozostawić wariant bez żadnej wartości. Na razie pracowaliśmy z wariantami zawierającymi liczbę lub tekst.

**Przed przejściem dalej:** wiesz już, jak zadeklarować dozwolone typy, zapisać jedną z wartości i bezpiecznie ją odczytać. W części B pokażemy `std::visit`, które pozwala opisać operację dla każdego typu bez ręcznego sprawdzania go w kolejnych instrukcjach `if`.

## B. Przetwarzanie i praktyka

### 4. `std::visit`: operacja dla aktywnej alternatywy

Seria `if (holds_alternative<...>)` szybko się rozrasta. `std::visit(f, v)` wywołuje `f` z aktualnie przechowywaną wartością. Sama funkcja nie musi ręcznie sprawdzać indeksu.

```cpp
std::visit(
    [](const auto& x) {
        std::cout << x;
    },
    v
);
```

`[]` zaczyna lambdę - niewielki obiekt zachowujący się jak funkcja. Puste nawiasy oznaczają brak przechwytywania zmiennych z otoczenia. Parametr `const auto&` oznacza referencję bez kopiowania i modyfikacji; `auto` sprawia, że operator wywołania lambdy jest szablonem dopasowywanym do typu argumentu.

Odwiedzający musi dać się wywołać dla **każdej alternatywy**, a nie tylko tej aktywnej przy konkretnym uruchomieniu. Dla powyższego kodu każdy możliwy typ musi obsługiwać wypisywanie przez `<<`.

Uruchom [`03_visit.cpp`](examples/03_visit.cpp). Drugi odwiedzający odróżnia tekst od liczby:

```cpp
using T = std::decay_t<decltype(item)>;
if constexpr (std::is_same_v<T, std::string>) {
    std::cout << item.size();
} else {
    std::cout << item + 1;
}
```

`decltype(item)` odczytuje typ parametru, np. `const std::string&`. Dla użytych typów `std::decay_t` usuwa referencję i kwalifikator `const`. `std::is_same_v` porównuje typy. `if constexpr` wybiera gałąź podczas instancjowania szablonu: gałąź z `.size()` nie musi być poprawna dla `int`. Zwykłe `if` nie daje tego efektu. W odrzucanej gałęzi nadal musi być poprawna składnia; nie jest to sposób ukrywania dowolnego błędu.

Wynik:

```text
wartosc: 12
liczba + 1: 13
wartosc: kot
dlugosc: 3
```

### 5. `overloaded`: kilka lambd jako jeden odwiedzający

Czasem czytelniej napisać oddzielną funkcję dla każdego typu. W C++17 można połączyć lambdy tak:

```cpp
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
```

`Ts...` to lista typów. Struktura dziedziczy po obiektach funkcyjnych, a `using` udostępnia wszystkie ich operatory wywołania. Ostatnia linia jest wskazówką dedukcji: pozwala kompilatorowi wywnioskować typy lambd z argumentów. Jest potrzebna w tym zapisie dla C++17. To **nasz pomocniczy szablon**, nie część biblioteki standardowej.

```cpp
std::visit(
    overloaded{
        [](int x) {
            std::cout << "liczba: " << x;
        },
        [](const std::string& x) {
            std::cout << "tekst: " << x;
        }
    },
    v
);
```

Pełny przykład [`04_overloaded.cpp`](examples/04_overloaded.cpp) obsługuje też `double`. Następnie używa odwiedzającego zwracającego `std::string`.

W C++17 wszystkie możliwe wywołania w jednym `std::visit` muszą mieć ten sam typ wyniku i tę samą kategorię wartości. `int` w jednym przypadku i `double` w drugim nie wystarczą, nawet jeśli jedno konwertuje się do drugiego. W programie 04 jawne `-> std::string` zapewnia wspólny wynik. Przy wypisywaniu wszystkie gałęzie zwracają `void`.

```text
int: 4
double: 2.5
string: kot
dlugosci opisow: 1 3 3
```

Pułapka: przeciążenia dopuszczają konwersje. Jeśli usuniesz obsługę `int`, funkcja przyjmująca `double` może nadal go przyjąć. Ogólna lambda `auto` również może obsłużyć nowo dodany typ. Kompilator sprawdza poprawność wywołania, a nie to, czy świadomie zaprojektowałeś obsługę każdego przypadku.

![std::visit wybiera przeciążenie dla std::string; gałęzie int i double nie są wykonywane.](diagrams/02-visit.png)

W przykładzie 04 wszystkie trzy gałęzie muszą być poprawne, choć pojedyncze wywołanie `visit` wybiera jedną z nich.

### 6. Zastosowanie: konfiguracja

[`05_config.cpp`](examples/05_config.cpp) przechowuje ustawienia w `std::map<std::string, ConfigValue>`. Każdy wpis ma nazwę i jedną wartość, a odwiedzający wypisuje ją niezależnie od aktywnego typu.

```text
debug = true
retries = 3
server = localhost
```

Jawne `std::string{"localhost"}` pokazuje zamierzony typ, bez polegania na szczegółach konwersji literału tekstowego. Mapa zapewnia tutaj kolejność po kluczach; `variant` nie odpowiada za kolejność wpisów.

Taki model zapobiega niezgodności ręcznej flagi i typu wartości. **Nie sprawdza jednak reguł konfiguracji:** nadal można przypisać tekst do klucza `retries`. Regułę „ten klucz wymaga liczby nieujemnej” trzeba zrealizować osobno. Dla z góry ustalonego zestawu ustawień zwykła struktura z nazwanymi polami może być lepsza.

### 7. Zastosowanie: sukces albo błąd

W [`06_result.cpp`](examples/06_result.cpp) wynik parsowania ma postać:

```cpp
struct Success {
    int value;
};
struct Error {
    std::string message;
};
using Result = std::variant<Success, Error>;
```

Zamiast utrzymywać liczbę, flagę sukcesu i komunikat w trzech niezależnych polach, zwracamy jednoznaczny przypadek. Odwiedzający dostaje wartość sukcesu albo opis błędu.

Program używa `std::stoi`: przechwytuje brak poprawnej liczby oraz przekroczenie zakresu, a dodatkowo sprawdza liczbę zużytych znaków. Dzięki temu `12x` nie zostaje zaakceptowane jako pełny zapis liczby 12.

```text
sukces: 42
blad: brak cyfr
blad: znaki po liczbie
blad: liczba poza zakresem int
```

To celowo niewielki parser: dziedziczy akceptowanie początkowych białych znaków i znaku `+`/`-` z `stoi`; końcowe znaki, również spacje, odrzuca. Nie służy do definiowania uniwersalnego formatu liczb.

Zwracanie `Result` opisuje przewidywane wyniki operacji, ale nie daje gwarancji braku wszelkich wyjątków - np. tworzenie tekstu błędu może wymagać alokacji. W C++23 istnieje `std::expected<T, E>` przeznaczone do modelu wartość albo błąd; w tym pakiecie pozostajemy przy C++17.

## C. Dla dociekliwych

Ta część rozwija podstawy z sekcji A i B. Zaczniemy od dwóch pytań: jak zapisać „jeszcze nie mam wartości” i co się dzieje, gdy ten sam typ występuje na liście dwa razy. Później przejdziemy do wyjątków, pamięci i maszyny stanów.

### 8. `monostate` i powtarzające się typy

#### Jak zapisać „jeszcze nic nie wybrano”?

Przypomnijmy: `std::variant<int, std::string> wartosc;` zaczyna od liczby 0. Czasem potrzebujesz jednak odróżnić „użytkownik podał zero” od „użytkownik jeszcze nic nie podał”.

Możesz dodać `std::monostate` jako pierwszą możliwość:

```cpp
std::variant<std::monostate, int, std::string> wartosc;
std::cout << std::boolalpha << std::holds_alternative<std::monostate>(wartosc) << '\n'; // true

wartosc = 0;
std::cout << std::holds_alternative<int>(wartosc) << '\n'; // true
```

`std::monostate` to prosty typ bez danych, którym możesz oznaczyć „brak podanej wartości”. Wybierasz to znaczenie w swoim programie. W pierwszej chwili wariant przechowuje właśnie `monostate`; po przypisaniu zera przechowuje `int`.

**Wariant z `monostate` nadal ma aktywną alternatywę.** Jest nią `monostate`, w tym przykładzie o indeksie 0. To ważne rozróżnienie względem stanu po wyjątku z następnego podrozdziału.

#### Po co `monostate` w przykładzie 08?

W [`08_edge_cases.cpp`](examples/08_edge_cases.cpp) występuje struktura `NoDefault`. Jej obiektu nie można utworzyć bez podania liczby:

```cpp
struct NoDefault {
    explicit NoDefault(int number) : value(number) {}
    int value;
};

std::variant<std::monostate, NoDefault> wartosc;
wartosc.emplace<NoDefault>(7);
std::cout << std::get<NoDefault>(wartosc).value << '\n'; // 7
```

Konstruktor `NoDefault(int number)` zapisuje przekazaną liczbę w polu `value`. `explicit` wymaga jawnego użycia tego konstruktora przy konwersji do `NoDefault`.

Deklaracja wariantu bez wartości początkowej działa, ponieważ pierwszym typem jest `monostate`, który można utworzyć bez argumentów. Dopiero `emplace<NoDefault>(7)` tworzy właściwy obiekt z liczbą 7 i zastępuje nim `monostate`.

Gdyby `NoDefault` był pierwszym typem, trzeba byłoby od razu podać argument jego konstruktora, np. przez `std::in_place_type<NoDefault>`:

```cpp
std::variant<NoDefault, std::string> wartosc{std::in_place_type<NoDefault>, 7};
std::cout << std::get<NoDefault>(wartosc).value << '\n'; // 7
```

#### Czy ten sam typ może wystąpić dwa razy?

Tak, ale wtedy sama nazwa typu nie wystarcza do wybrania miejsca na liście:

```cpp
std::variant<int, int> wartosc{std::in_place_index<1>, 9};
std::cout << wartosc.index() << '\n';      // 1
std::cout << std::get<1>(wartosc) << '\n'; // 9
```

`std::in_place_index<1>` oznacza: „utwórz wartość w alternatywie o indeksie 1”, czyli w drugim `int`. Liczba 9 jest wartością, którą tam zapisujemy.

Próba `std::get<int>(wartosc)` byłaby błędem kompilacji: kompilator nie wie, o który z dwóch `int` chodzi. Taki sam problem dotyczy `get_if<int>` i `holds_alternative<int>`. Można używać `get<0>` i `get<1>`, a do sprawdzania wybranej pozycji - `index()`.

Jeśli dwie liczby mają różne znaczenie, lepiej wyrazić je w nazwach typów:

```cpp
struct UserId {
    int value;
};
struct OrderId {
    int value;
};

std::variant<UserId, OrderId> identyfikator{OrderId{9}};
std::cout << std::get<OrderId>(identyfikator).value << '\n'; // 9
```

Teraz widać, że odczytujemy identyfikator zamówienia, a nie dowolną liczbę z pozycji 1.

### 9. Co oznacza `valueless_by_exception()`?

Zwykle wariant zawiera jedną z dozwolonych wartości. Istnieje jednak szczególny przypadek: podczas zmiany wartości może wystąpić wyjątek, po którym **wariant nie zawiera żadnej alternatywy**.

Wyobraź sobie następujący przebieg:

1. Wariant przechowuje liczbę 42.
2. Program chce zastąpić ją obiektem innego typu.
3. Dotychczasowa liczba zostaje usunięta.
4. Tworzenie nowego obiektu kończy się wyjątkiem.
5. Nowego obiektu nie udało się utworzyć, a starej liczby już nie ma.

Metoda `valueless_by_exception()` pozwala sprawdzić, czy wariant jest w takim stanie. Zwraca wtedy `true`.

**Nie każdy wyjątek powoduje utratę wartości.** Powyższy przebieg tłumaczy problem; dokładny skutek zależy od wykonywanej operacji i typów. Samo wywołanie `get` z niewłaściwym typem rzuca wyjątek, ale nie usuwa przechowywanej wartości.

#### Jak wymuszamy ten przypadek w programie 08?

Typ `ThrowsOnMove` w pliku [`08_edge_cases.cpp`](examples/08_edge_cases.cpp) ma konstruktor przenoszący, który celowo rzuca wyjątek. Konstruktor przenoszący służy do tworzenia obiektu na podstawie innego obiektu, którego zasoby można przejąć. Zapis `noexcept(false)` jawnie zaznacza, że ten konstruktor może rzucać wyjątki.

Program przygotowuje dwa warianty:

```cpp
using Value = std::variant<int, ThrowsOnMove>;
Value target{42};
Value source{std::in_place_index<1>};

try {
    target = std::move(source);
} catch (const std::runtime_error&) {
    std::cout << "wyjatek przy przenoszeniu\n";
}
std::cout << std::boolalpha << target.valueless_by_exception() << '\n'; // true
```

Ten fragment korzysta z definicji `ThrowsOnMove` z programu 08. `target` początkowo zawiera liczbę, a `source` - obiekt `ThrowsOnMove`. `in_place_index<1>` tworzy obiekt źródłowy bezpośrednio w drugiej alternatywie, bez wcześniejszego przenoszenia go z innego miejsca.

`std::move(source)` pozwala wybrać operację przenoszącą; samo wywołanie `std::move` jeszcze niczego nie przenosi. Przenoszenie odbywa się w przypisaniu. Ponieważ warianty zawierają różne typy, `target` musi utworzyć u siebie obiekt `ThrowsOnMove`. Jego konstruktor rzuca wyjątek.

**Dla tego konkretnego przypadku standard gwarantuje, że `target` zostanie bez wartości:** chodzi o wyjątek z konstrukcji przenoszącej podczas przypisania przenoszącego między wariantami o różnych aktywnych indeksach.

#### Co można wtedy zrobić?

Obiekt wariantu nadal istnieje, ale nie ma w nim wartości, którą można odczytać:

- `index()` zwraca specjalną stałą `std::variant_npos`, zamiast indeksu typu.
- `get_if` zwraca `nullptr`, a `holds_alternative` zwraca `false` dla każdego typu z listy.
- `get` i `visit` rzucają `std::bad_variant_access`.

Możesz nadać mu nową wartość:

```cpp
target = 10;
std::cout << std::get<int>(target) << '\n'; // 10
std::cout << std::boolalpha << target.valueless_by_exception() << '\n'; // false
```

Ten fragment jest kontynuacją poprzedniego. Udane przypisanie przywraca zwykły stan wariantu. Można do tego użyć również udanego `emplace`.

Różnica do zapamiętania: **`monostate` oznacza wybraną przez programistę wartość „brak danych”, a `valueless_by_exception` oznacza brak jakiejkolwiek przechowywanej wartości po nieudanej operacji.**

Program 08 sprawdza te warunki także przez `assert`. To instrukcja kontrolna: jeśli warunek jest fałszywy, program zostaje przerwany. Flaga `-DNDEBUG` wyłącza takie kontrole, więc nie używaj jej przy sprawdzaniu tego przykładu.

### 10. Skąd wariant wie, który typ wybrać?

Przy prostym przypisaniu wybór wygląda naturalnie:

```cpp
std::variant<int, double> liczba{3};
std::cout << liczba.index() << '\n'; // 0: int
liczba = 3.5;
std::cout << liczba.index() << '\n'; // 1: double
```

Literał `3` ma typ `int`, a `3.5` ma typ `double`. W tych przypadkach jest jednoznaczne dopasowanie do typu z listy.

Trudniej jest wtedy, gdy podana wartość wymaga konwersji. Konwersja to zamiana wartości jednego typu na inny, np. liczby całkowitej na zmiennoprzecinkową. Wariant korzysta z reguł wyboru przeciążenia i ograniczeń konstruktora, aby ustalić, którą alternatywę utworzyć. Nie wybiera po prostu pierwszego typu z listy. Jeśli nie ma poprawnego, jednoznacznego wyboru, kod nie zostanie skompilowany.

Możesz wskazać typ bezpośrednio:

```cpp
std::variant<int, double> liczba{std::in_place_type<double>, 3};
std::cout << liczba.index() << '\n'; // 1
std::cout << std::get<double>(liczba) << '\n'; // 3
```

`std::in_place_type<double>` oznacza: „utwórz alternatywę `double`, używając dalszych argumentów”. Zapisana wartość to `3.0` typu `double`, chociaż `std::cout` domyślnie wypisuje ją jako `3`.

#### Dlaczego w przykładach tekst zapisujemy jawnie?

Literał `"abc"` i obiekt `std::string{"abc"}` to różne typy. W drugim zapisie jawnie tworzysz obiekt tekstowy:

```cpp
std::variant<bool, std::string> tekst{std::string{"abc"}};
std::cout << std::get<std::string>(tekst) << '\n'; // abc
```

Ma to znaczenie m.in. dla historycznej pułapki: przy inicjalizacji wariantu z alternatywami `bool` i `std::string` stara biblioteka mogła wybrać `bool` dla literału tekstowego. Adres tekstu można bowiem przekształcić na wartość logiczną. Nowsze reguły ograniczyły takie niepożądane konwersje.

Szczegóły opisuje dokument [P0608R3](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0608r3.html) oraz [opis konstruktora z listą poprawek](https://en.cppreference.com/w/cpp/utility/variant/variant). Poprawki trafiają również do bibliotek pracujących w trybie C++17, więc sama flaga `-std=c++17` nie oznacza identycznego zachowania każdej historycznej wersji biblioteki.

Praktyczna zasada: jeśli chcesz uniknąć wątpliwości, podaj wartość zamierzonego typu albo użyj `in_place_type`.

### 11. Ile pamięci zajmuje wariant i czy można go kopiować?

#### Miejsce musi wystarczyć na każdy dozwolony typ

Dla `std::variant<int, double, std::string>` ten sam obiekt musi umieć pomieścić każdy z trzech typów. Nie musi przechowywać ich jednocześnie, ale potrzebuje dość miejsca na największy z nich oraz informacji, który jest aktualnie wybrany.

Dlatego wariant zawierający małe `int` nie staje się mniejszy tylko dlatego, że obecnie nie przechowuje tekstu. Rozmiar obiektu jest ustalony przez jego typ.

W przykładzie 08 wypisujemy:

- `sizeof(...)` - rozmiar obiektu w bajtach;
- `alignof(...)` - wymagane wyrównanie, czyli ograniczenie dotyczące adresów, pod którymi obiekt może być umieszczony w pamięci.

Przy wyrównaniu 8 adres obiektu musi być wielokrotnością 8. Wyrównanie może powodować pozostawienie dodatkowych bajtów w układzie obiektu. Z tego powodu nie wystarczy dodać „rozmiar największego typu + jeden bajt na numer”. Standard nie narzuca konkretnego układu ani rozmiaru znacznika.

W naszym sprawdzeniu wariant `int, double, std::string` zajmował 32 bajty w libc++ i 40 w libstdc++. To wyniki konkretnych środowisk, a nie wartości wymagane na każdym komputerze.

#### Czy wariant używa pamięci dynamicznej?

Przechowywana alternatywa znajduje się wewnątrz obiektu wariantu. Sam wariant nie potrzebuje osobnej alokacji na tę alternatywę. Jednak jej własne dane mogą korzystać z pamięci dynamicznej - np. obiekt `std::string` może przechowywać długi tekst poza swoim obiektem.

Dlatego „obiekt tekstowy mieści się wewnątrz wariantu” nie oznacza, że wszystkie znaki tekstu również muszą się tam mieścić.

#### Od czego zależy kopiowanie?

Aby skopiować wariant, trzeba móc skopiować każdą z jego alternatyw. Kompilator musi dopuścić kopiowanie całego typu niezależnie od tego, która alternatywa jest akurat aktywna.

Jeśli na liście znajduje się `std::unique_ptr<int>`, wariantu nie można kopiować, ponieważ `unique_ptr` nie pozwala kopiować własności zasobu. Taki wariant może natomiast pozwalać na przenoszenie. Podobnie dostępność przypisania zależy od operacji udostępnianych przez typy alternatyw.

Jako alternatywy nie można użyć bezpośrednio referencji, np. `int&`, typu `void` ani surowej tablicy. Gdy potrzebujesz odwołania do istniejącego obiektu, jednym z rozwiązań jest `std::reference_wrapper`. Nie przejmuje on jednak własności obiektu: trzeba dopilnować, aby wskazywany obiekt nadal istniał.

Duże alternatywy zwiększają rozmiar każdego wariantu danego typu. Nie zakładaj też, że `variant` zawsze będzie szybszy od funkcji wirtualnych - wydajność należy sprawdzić dla konkretnego programu.

### 12. Maszyna stanów: reakcja zależy od stanu i zdarzenia

Maszyna stanów to sposób opisania programu, którego zachowanie zależy od tego, **w jakim stanie jest teraz** oraz **co właśnie się wydarzyło**.

Przykład [`07_state_machine.cpp`](examples/07_state_machine.cpp) symuluje połączenie z serwerem. Ma trzy stany:

- `Disconnected` - nie jesteśmy połączeni.
- `Connecting` - trwa nawiązywanie połączenia; pamiętamy nazwę hosta.
- `Connected` - połączenie zostało potwierdzone; nadal pamiętamy nazwę hosta.

Osobno opisujemy zdarzenia:

- `Connect` - polecenie rozpoczęcia łączenia z podanym hostem.
- `Established` - informacja, że połączenie się udało.
- `Disconnect` - polecenie rozłączenia.

Stan i zdarzenie są dwoma różnymi wariantami. W danej chwili mamy jeden konkretny stan i przetwarzamy jedno konkretne zdarzenie.

![Przejścia połączenia: Connect prowadzi do Connecting, Established do Connected, a Disconnect wraca do Disconnected.](diagrams/03-stany.png)

Strzałki pokazują cztery jawne reguły z `transition`. Pozostałe kombinacje obsługuje ostatnia, ogólna lambda, która zachowuje dotychczasowy stan.

#### Ta sama informacja może mieć różny sens w różnych stanach

Jeśli jesteśmy w `Connecting` i przychodzi `Established`, przechodzimy do `Connected`. Jeśli jesteśmy w `Disconnected` i przychodzi `Established`, ignorujemy to zdarzenie: nie rozpoczęliśmy wcześniej łączenia.

Funkcja `transition` przyjmuje stan i zdarzenie, a zwraca nowy stan. Z jej kodu pochodzi taka reguła:

```cpp
[](const Connecting& stan, const Established&) -> State {
    return Connected{stan.host};
}
```

To fragment odwiedzającego, nie cały program. Pierwszy argument mówi, w jakim stanie jesteśmy. Drugi mówi, jakie zdarzenie obsługujemy; nie ma nazwy, ponieważ nie potrzebujemy z niego żadnych danych. `-> State` określa wspólny typ wyniku. Zwracamy stan połączenia z tym samym hostem.

W `std::visit(visitor, state, event)` odwiedzający dostaje **dwa argumenty**: wartość z wariantu stanu i wartość z wariantu zdarzenia. Ponieważ każdy wariant ma trzy możliwości, istnieje `3 × 3 = 9` kombinacji typów argumentów. Kod musi umieć obsłużyć każdą, chociaż jedno wywołanie przetwarza tylko aktualną parę.

#### Co z kombinacjami bez własnej reguły?

Program ma ogólną lambdę przyjmującą `const auto&` dla obu argumentów. Obsługuje ona pozostałe kombinacje: wypisuje `zdarzenie pominiete` i zwraca dotychczasowy stan.

Wszystkie gałęzie zwracają `State`. Dzięki temu `visit` ma jeden typ wyniku, mimo że poszczególne reguły tworzą różne struktury stanów.

#### Jak przeczytać wynik programu?

Program wykonuje kolejno:

1. Startuje w stanie `Disconnected`.
2. Dostaje `Established` za wcześnie - pomija zdarzenie.
3. Dostaje `Connect` z hostem `example.org` - przechodzi do `Connecting`.
4. Dostaje `Established` - przechodzi do `Connected`.
5. Dostaje kolejne `Connect` podczas istniejącego połączenia - pomija zdarzenie.
6. Dostaje `Disconnect` - wraca do `Disconnected`.

Stąd wynik:

```text
stan: rozlaczony
zdarzenie pominiete
stan: rozlaczony
stan: laczenie z example.org
stan: polaczony z example.org
zdarzenie pominiete
stan: polaczony z example.org
stan: rozlaczony
```

Instrukcja `state = transition(state, event)` najpierw oblicza nowy stan, a dopiero potem zastępuje nim dotychczasowy. Dzięki temu odwiedzający może korzystać ze starego stanu przez cały czas obliczania przejścia.

Przy dodawaniu stanów lub zdarzeń trzeba przejrzeć reguły ponownie. Ogólna lambda zapewnia poprawność wywołania dla pozostałych par, ale nie oceni za nas, czy pominięcie nowego zdarzenia ma sens. Więcej alternatyw oznacza więcej kombinacji do rozważenia i może zwiększyć czas kompilacji.

Ten program jest symulacją reguł. Nie nawiązuje rzeczywistego połączenia sieciowego.

### 13. Kiedy wybrać inne rozwiązanie i czego uczyć się dalej?

#### Dobierz narzędzie do tego, co chcesz opisać

`variant` przydaje się, gdy znasz listę różnych możliwości, np. „liczba albo tekst” lub „rozłączony albo łączenie albo połączony”. Wybór jednej z możliwości nazywa się też *sum type* - to nazwa modelu danych, a nie dodatkowa funkcja C++.

Inne sytuacje mogą wymagać prostszego lub innego narzędzia:

- **Liczba albo brak liczby:** `std::optional<int>`. Wszystkie obecne wartości mają jeden typ `int`.
- **Imię i wiek jednocześnie:** zwykła struktura z dwoma polami. Obie informacje są potrzebne razem.
- **Przechowanie wartości bez wpisywania listy typów w deklaracji pojemnika:** `std::any`. Obsługuje typy kopiowalne; aby odzyskać wartość, musisz znać jej typ.
- **Niskopoziomowe współdzielenie pamięci przez składowe:** `union`. Sam nie pamięta, która składowa jest aktywna, i nie zapewnia kontroli odczytu takiej jak `variant`.
- **Rodzina obiektów rozszerzana o kolejne typy w niezależnych częściach programu:** warto rozważyć wspólny interfejs i funkcje wirtualne. Wariant wymaga podania listy alternatyw w jego typie.

#### Co zmieniają nowsze standardy?

Cały uruchamialny kod w tych materiałach korzysta z C++17. Od C++20 można m.in. użyć `std::visit<R>`, aby wskazać typ wyniku odwiedzania. `R` oznacza tutaj wybrany przez programistę typ, np. `double`. W przykładach pozostajemy przy zapisie dostępnym w C++17. Przed użyciem rozszerzeń sprawdź obsługę w swojej bibliotece i kompilatorze.

#### Dalszy temat: drzewo wyrażeń

Wyrażenie `2 + 3` można opisać jako węzeł dodawania z dwoma podwyrażeniami. Każde podwyrażenie może być liczbą albo kolejnym działaniem. `variant` pasuje do tego wyboru, ale pojawia się problem rekurencji: węzeł nie może zawierać w sobie kolejnego pełnego obiektu tego samego typu bez końca.

Jednym z rozwiązań jest przechowywanie podwęzłów przez inteligentne wskaźniki. Węzeł przechowuje wtedy wskaźniki do osobnych obiektów zamiast umieszczać całą rekurencyjną strukturę bezpośrednio w swoim wnętrzu. To temat do dalszej nauki, wymagający także zrozumienia własności i czasu życia obiektów.

## Dobre praktyki do zapamiętania

- Wybieraj alternatywy opisujące znaczenie danych; nazwane struktury pomagają odróżnić podobne przypadki.
- Używaj `get_if`, gdy niezgodna alternatywa jest oczekiwaną sytuacją; `get`, gdy chcesz odczytu z kontrolą przez wyjątek.
- Stosuj `visit` do wspólnej operacji na alternatywach. Przejrzyj wszystkie przypadki po zmianie listy typów.
- Nie przechowuj wskaźników do alternatywy dłużej niż trwa jej życie.
- Odróżniaj pusty stan modelu (`monostate`) od braku wartości po wyjątku.
- Nie dodawaj alternatyw „na zapas”. Zbyt szeroki wariant utrudnia opis reguł programu.

## Dodatek: rozpoznawanie błędów kompilacji

Poniższe dwa programy są **celowo błędne**. Nie należą do ośmiu przykładów do uruchomienia. Ich odrzucenie przez kompilator jest oczekiwanym wynikiem.

Pierwszy: typ `int` występuje dwa razy, więc nie można wybrać go przez sam typ. Poprawka: użyć `std::get<1>(value)`.

```cpp
#include <variant>
int main() {
    std::variant<int, int> value{std::in_place_index<1>, 9};
    (void)std::get<int>(value); // BLAD: typ nie jest jednoznaczny
}
```

Drugi: gałęzie odwiedzającego mają różne typy wyniku. Poprawka: dopisać `-> double` do lambdy, aby obie instancje zwracały `double`.

```cpp
#include <type_traits>
#include <variant>
int main() {
    std::variant<int, double> value{1};
    (void)std::visit(
        [](auto x) {
            if constexpr (std::is_same_v<decltype(x), int>) {
                return 1;
            } else {
                return 1.5;
            }
        },
        value
    ); // BŁAD: wynik int albo double
}
```