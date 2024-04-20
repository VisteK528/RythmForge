# RythmForge audio&signal processing library - szkielet aplikacji

## Autorzy

- Piotr Patek
    - e-mail: piotr.patek.stud@pw.edu.pl
    - Indeks 324879
- Damian Baraniak
    - e-mail: damian.baraniak.stud@pw.edu.pl
    - Indeks 324851

## Przyjęta struktura repozytorium
W repozytorium znajdują się foldery takie jak:
- `rythm_forge/` - zawierający pliki konfigurujące bibliotekę języka Python
- `src/` - zawierający podfoldery takie jak:
    - `include` z plikami nagłówkowymi dla każdego/klasy
    - `modules` z wiązaniami kodu C++ i Python,
    - `src` z implementacją funkcji i klas w C++,
    - `tests` z testami dla implementacji w języku C++
- `tests/` - zawierający testy jednostkowe dla implementacji w języku Python
- `docs/` - zawierający pliki konifiguracyjne i źródłowe dokumentacji w Sphinx

## Opis prac

1. Przygotowano ogólną strukturę repozytorium z zachowaniem modularnej struktury.
2. Stworzono wstępnaxa klasę w języku C++ umożliwiającą niezabezpieczone jednowątkowe ładowanie plików .WAV.
3. Stworzono przykładowe moduły języka Python z funkcjami, których backend jest napisany w C++.
4. Przygotowano przykładowe testy jednostowe dla kodu w C++ oraz Python.
5. Skonfigurowano GitHub Actions zapewniające poprawną budowę programu na systemach Windows kompilowanego przez MSVC 17 oraz Linux kompilowanego przez gcc 11.4.
6. Skonfigurowano podstawową stronę z dokumentacją biblioteki w Sphinx przy pomocy GitHub Pages oraz odpowiednie akcje do sprawnego budowania.

## Technologie

1. C++20
2. Python 3.10
3. Testy z Catch2 i Pytest
4. Dokumentacja w Sphinx 7.3.7 