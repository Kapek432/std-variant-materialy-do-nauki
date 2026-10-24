# Warto przeczytać - std::variant na przykładach

## 1. Wprowadzenie i praktyczne zastosowania

[Everything You Need to Know About std::variant from C++17 - C++ Stories](https://www.cppstories.com/2018/06/variant/)

Przekrojowy przewodnik: tworzenie wariantu, zmiana wartości, odczyt i odwiedzanie. Dalej pokazuje zastosowania, m.in. obsługę błędów i maszynę stanów drzwi. Przy przykładach są wyniki oraz linki do uruchomienia kodu w przeglądarce. Dobry punkt wyjścia do części A i B naszego materiału.

## 2. Skąd się bierze pomocnik overloaded?

[2 Lines Of Code and 3 C++17 Features - The overload Pattern - C++ Stories](https://www.cppstories.com/2019/02/2lines3featuresoverload.html/)

Rozkłada dwuwierszowy pomocnik na zrozumiałe elementy: lambdy, dziedziczenie, zapis `using` i dedukcję argumentów szablonu. Pokazuje, jak kilka lambd zastępuje osobno napisaną klasę odwiedzającą. Uzupełnienie części B.5, jeśli sam zapis `overloaded` nadal wygląda zagadkowo.

## 3. Dwa warianty naraz: przedmioty i pudełka

[How To Use std::visit With Multiple Variants and Parameters - C++ Stories](https://www.cppstories.com/2018/09/visit-variants/)

Konkretny przykład dobierania pudełka do rodzaju przedmiotu pokazuje, jak `visit` obsługuje kombinacje dwóch typów. Artykuł zawiera ilustrację przykładu, kod do uruchomienia w Compiler Explorer i ogólną gałąź dla pozostałych kombinacji. Pomaga zrozumieć mechanizm użyty w naszej maszynie stanów z części C.12.

## 4. Dalsza lektura: variant zamiast hierarchii klas

[Runtime Polymorphism with std::variant and std::visit - C++ Stories](https://www.cppstories.com/2020/04/variant-virtual-polymorphism.html/)

Porównuje funkcje wirtualne z rozwiązaniem opartym na `variant` i `visit`. Praktyczny przykład buduje HTML z różnych rodzajów etykiet. Warto zajrzeć po części C.13, żeby zobaczyć różnice w projektowaniu programu oraz zalety i ograniczenia obu podejść.

Nasz materiał pozostaje przy C++17. Artykuły zawierają też osobno oznaczone rozszerzenia z C++20 i C++23, które można pominąć przy pierwszym czytaniu.
