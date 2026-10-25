# Artykuły i dokumentacja do std::variant

Artykuły pomagają zrozumieć temat na przykładach. Dokumentacja służy do sprawdzenia konkretnej operacji. Te same odnośniki znajdziesz na końcu odpowiednich notebooków.

## 01. Podstawy i odczyt

### Artykuły

- [C++ Stories: Everything You Need to Know About std::variant](https://www.cppstories.com/2018/06/variant/) - wprowadzenie i praktyczne zastosowania.

### Dokumentacja

- [std::variant](https://en.cppreference.com/cpp/utility/variant) - operacje i wymagania wariantu.
- [std::get](https://en.cppreference.com/cpp/utility/variant/get) - odczyt przez typ lub indeks.
- [std::get_if](https://en.cppreference.com/cpp/utility/variant/get_if) - odczyt przez wskaźnik i nullptr.

## 02. Visit i overloaded

### Artykuły

- [C++ Stories: The overload Pattern](https://www.cppstories.com/2019/02/2lines3featuresoverload.html/) - wyjaśnienie łączenia lambd krok po kroku.
- [C++ Stories: Everything You Need to Know About std::variant](https://www.cppstories.com/2018/06/variant/) - wprowadzenie i praktyczne zastosowania.

### Dokumentacja

- [std::visit (funkcja wolna, od C++17)](https://en.cppreference.com/cpp/utility/variant/visit2) - wywoływanie odwiedzającego dla jednego lub wielu wariantów.
- [std::variant](https://en.cppreference.com/cpp/utility/variant) - operacje i wymagania wariantu.

## 03. Praktyczne zastosowania

### Artykuły

- [C++ Stories: Everything You Need to Know About std::variant](https://www.cppstories.com/2018/06/variant/) - wprowadzenie i praktyczne zastosowania.
- [C++ Stories: std::visit With Multiple Variants](https://www.cppstories.com/2018/09/visit-variants/) - przykład przedmiotów i pudełek, obsługa kombinacji typów.

### Dokumentacja

- [std::variant](https://en.cppreference.com/cpp/utility/variant) - operacje i wymagania wariantu.
- [std::visit (funkcja wolna, od C++17)](https://en.cppreference.com/cpp/utility/variant/visit2) - wywoływanie odwiedzającego dla jednego lub wielu wariantów.
- [std::stoi](https://en.cppreference.com/cpp/string/basic_string/stol) - konwersja tekstu na liczbę i zgłaszane wyjątki.

## 04. Pułapki i ograniczenia

### Artykuły

- [C++ Stories: Everything You Need to Know About std::variant](https://www.cppstories.com/2018/06/variant/) - wprowadzenie i praktyczne zastosowania.
- [C++ Stories: Runtime Polymorphism with std::variant and std::visit](https://www.cppstories.com/2020/04/variant-virtual-polymorphism.html/) - porównanie z funkcjami wirtualnymi.

### Dokumentacja

- [std::monostate](https://en.cppreference.com/cpp/utility/variant/monostate) - jawny pusty stan modelu.
- [valueless_by_exception](https://en.cppreference.com/cpp/utility/variant/valueless_by_exception) - brak wartości po wyjątku.
- [Konstruktory std::variant](https://en.cppreference.com/cpp/utility/variant/variant) - wybór alternatywy i konstrukcja in_place.
- [std::visit (funkcja wolna, od C++17)](https://en.cppreference.com/cpp/utility/variant/visit2) - wywoływanie odwiedzającego dla jednego lub wielu wariantów.

Przykłady materiału korzystają z C++17. Artykuły i bieżąca dokumentacja opisują także późniejsze standardy; zwracaj uwagę na oznaczenia wersji.
