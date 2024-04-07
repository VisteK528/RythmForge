# RythmForge audio&signal processing library

## Autorzy

- Piotr Patek
    - e-mail: piotr.patek.stud@pw.edu.pl
    - Indeks 324879
- Damian Baraniak
    - e-mail: damian.baraniak.stud@pw.edu.pl
    - Indeks 324851

## Treść zadania

Celem projektu jest zaimplementowanie prototypu biblioteki do analizy audio z API w Pythonie i niskopoziomowym backendem w lub C++. Docelowo biblioteka ma mieć zbliżoną funkcjonalność do bilbioteki [`librosa`](https://github.com/librosa/librosa).

## Założenia

W ramach projektu stworzona zostanie biblioteka do przetwarzania sygnałów audio. Ze względu na obszerność funkcjonalności oferowanych przez bibliotekę librosa, zaimplementowane zostaną jedynie niektóre algorytmy i funkcje. Cały projekt jest planowany, aby miał strukturę modułową, ułatwaijąc rozbudowę funkcjonalności w przyszłości.

Algorytmy do zaimplementowania:

- Short-time Fourier transform (STFT)
- Inverse short-time Fourier Transform (ISTFT)
- Melspectrogram
- Tempo estimation and beat tracking

#### W tej chwili można wyróżnić moduły:

- Moduł wejścia - wyjścia - ładowanie i zapisywanie plików audio (wspierane formaty - `MP3`, `WAV`, `FLAC`)
- Moduł konwersji formatów - przetwarzanie plików audio zapisanych w różnych formatach, z możliwością konwersji do innych obsługiwanych formatów.
- Moduł podstawowych algorytmów - podstawowym algorytmem wykorzystywanym przy przetwarzaniu sygnałów jest transformata Fouriera, będąca bazą dla pozostałych algortmów. W tej sekcji zaimplementowane zostaną algorytmy
    - STFT - przekształcenie z dziedziny czasu na dziedzinę częstotliwości - wykorzystywana przez **melspectrogram**, dalsza obróbka sygnału w dziedzinie częstotliwości
    - ISTFT - przekształcenie z dziedziny częstotliwości na dziedzinę czasu
- Moduł konwersji jednostek sygnału
    - Zamiana `hz_to_mel`
    - Zamiana `mel_to_hz`
- Moduł ekstrakcji cech
    - Melspectrogram - generowanie danych do spektrogramu w melach
    - Spectrogram - generowanie danych do spektrogramu w hercach
- Moduł API - udostępnianie klas, metod i funkcji zdefiniowanych w C++ do języka Python
    - Założeniem jest wykorzystywanie biblioteki `numpy` po stronie Pythona do reprezentacji wektorów

## Wykorzystywane technologie
- Wersje języków: C++20, python3.10
- Testy: Catch2, pytest
- Zarządzanie zależnościami projektów: conan, pdm
- Budowa projektu: CMake
- Wiązanie Python C++: pybind11
- Generowanie dokumentacji: Sphinx