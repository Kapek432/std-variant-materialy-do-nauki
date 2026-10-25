# std::variant - jedna wartość, kilka możliwych typów

Materiały do tematu **11. std::variant**, Zaawansowany C++. Wszystkie przykłady korzystają z C++17. Potrzebujesz podstaw funkcji, struktur i referencji; lambdy i użyte elementy szablonów wyjaśniamy po drodze.

## Zacznij tutaj

[Start: przygotowanie środowiska i mapa materiału](notebooks/00_start.ipynb).

1. **Podstawy:** [Podstawy i odczyt](notebooks/01_podstawy.ipynb) - jedna wartość, zmiana typu, `get`, `get_if` i porównanie z ręczną flagą.
2. **Praktyczne użycia:** [Visit i overloaded](notebooks/02_visit.ipynb) - wspólna operacja dla różnych typów i osobne lambdy.
3. **Praktyczne użycia:** [Praktyczne zastosowania](notebooks/03_zastosowania.ipynb) - konfiguracja klienta API, sukces albo błąd oraz maszyna stanów połączenia.
4. **Rozszerzenie dla dociekliwych:** [Pułapki i ograniczenia](notebooks/04_pulapki.ipynb) - `monostate`, wyjątki, konwersje, pamięć i błędy kompilacji; materiał rozszerzający.

Każdy rozdział można uruchomić samodzielnie od pierwszej komórki. Kod C++ jest wykonywalny, a wyniki i diagramy są zapisane w notebookach. Na końcu każdego znajdziesz artykuły oraz dokumentację związaną z jego tematem.

## Poziomy materiału

- **Podstawy** - po co jest `variant`, jakie problemy rozwiązuje i jak bezpiecznie odczytać wartość.
- **Praktyczne użycia** - przetwarzanie przez `visit` oraz przykłady konfiguracji, wyników operacji i maszyny stanów.
- **Rozszerzenie dla dociekliwych** - szczególne przypadki, ograniczenia i dalsze kierunki nauki.

## Uruchomienie

Potrzebujesz **Pythona 3.11-3.14**, [uv](https://docs.astral.sh/uv/getting-started/installation/) oraz kompilatora i biblioteki standardowej C++17: `c++`, `clang++` albo `g++`. Python nie zastępuje kompilatora. W katalogu repozytorium wykonaj:

```bash
uv sync
uv run jupyter lab notebooks/00_start.ipynb
```

W JupyterLab wybierz kernel **Python 3 (ipykernel)**. Pierwsza komórka techniczna każdego rozdziału rejestruje `%%cpp`; kolejne komórki kompilują osobne programy C++17. Notebook startowy sprawdza kompilator i wykonuje mały program kontrolny. Pierwsze przygotowanie środowiska wymaga internetu; później kod C++ działa lokalnie.

## Do czytania i powtórek

- [Pełny przewodnik](PRZEWODNIK.md) - objaśnienia, kod i diagramy w jednym dokumencie.
- [Krótka ściąga](SCIAGA.md) - najważniejsze operacje i pułapki.
- [Artykuły i dokumentacja](references.md) - lektury pogrupowane według tematu.
- [Kompletne programy C++](examples/) - osiem przykładów z własnymi `main()`.

Programy z `examples/` można sprawdzić poleceniem `bash check.sh`, które kompiluje je z ostrzeżeniami traktowanymi jako błędy i porównuje wyniki z `expected/`. Nie wymaga to Jupytera. Pliki `.clang-format` i `.clang-tidy` służą do utrzymania stylu i analizy kodu; nie są wymagane do nauki.

## Struktura repozytorium

```text
.
├── README.md                 # start, poziomy materiału i instrukcja uruchomienia
├── PRZEWODNIK.md             # pełne objaśnienia z oznaczeniami poziomów
├── SCIAGA.md                 # najważniejsze operacje i pułapki do powtórki
├── references.md             # artykuły i dokumentacja według tematów
├── variant.ipynb             # notebook ze spisem treści i linkami do rozdziałów
├── notebooks/
│   ├── 00_start.ipynb        # przygotowanie i sprawdzenie środowiska
│   ├── 01_podstawy.ipynb     # Podstawy: typy, zapis i bezpieczny odczyt
│   ├── 02_visit.ipynb        # Praktyczne użycia: visit i overloaded
│   ├── 03_zastosowania.ipynb # Praktyczne użycia: klient API i stany połączenia
│   └── 04_pulapki.ipynb      # Rozszerzenie dla dociekliwych: ograniczenia i wyjątki
├── examples/                # osiem samodzielnych programów C++17 z main()
├── expected/                # oczekiwane wyniki programów z examples/
├── diagrams/                # ilustracje użyte w przewodniku i notebookach
├── check.sh                 # kompilacja programów i porównanie wyników z expected/
├── pyproject.toml           # wymagania Pythona i zależności Jupytera
├── uv.lock                  # dokładne wersje zależności środowiska
├── .clang-format            # reguły formatowania kodu C++
├── .clang-tidy              # reguły analizy statycznej C++
└── .gitignore               # pliki lokalne pomijane przez Git
```
