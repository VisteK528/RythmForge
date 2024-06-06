# RythmForge audio&signal processing library
## Autorzy

- Piotr Patek
    - e-mail: piotr.patek.stud@pw.edu.pl
    - Indeks 324879
- Damian Baraniak
    - e-mail: damian.baraniak.stud@pw.edu.pl
    - Indeks 324851


# Rythm Forge - dokumentacja końcowa

## Ogólne informacje

Celem projektu było zaimplementowanie biblioteki o funkcjonalności podobnej do bilbioteki [librosa]([https://librosa.org/](https://librosa.org/)), wykorzystując język niskopoziomowy C++ do backendu.

Przetestowano na platformach:

- `Ubuntu 22.04 LTS`
- `macOS Sonoma 14.3`
- `Windows`

Wykorzystane wersje języków:

- Python: `3.10.x`
- C++ `20`

## Architektura

![Struktura repozytorium](docs\images\ProjectDiagram.png)

Struktura repozytorium

Cały backend, kod źródłowy, wykonujący najcięższe obliczenia, napisany w C++, znajduje się w folderze `src`. Wewnątrz występuje podstawowe rozróżnienie między plikami nagłówkowymi w folderze `include` oraz źródłowymi w `src`. Całość aplikacji podzielono na dwie grupy, odpowiedzialny za ładowanie plików `.wav` oraz wykonujący na danych obliczenia.

Kod napisany w Pythonie znajduje się w folderze `rythm_forge`, występuje podobny podział na podmoduły, takie jak wcześniej wspomniane i dodatkowe `exceptions` i `display` odpowiedzialne odpowiednio za specjalnie przygotowane wyjątki i wizualizujący dane.

W celu połączenia przygotowanego niskopoziomowego backendu z Pythonem stworzono folder `/src/module`, wewnątrz którego stworzono pliki korzystające z pakietu `pybind11`. Wewnątrz folderu znajdują sie funckje służące w dużej mierze za nakładki na goły C++, często konwertują dane z Pythona na przystępne dla C++, wywołują liczące funkcje, by na koniec wynik z powrotem dostosować dla Pythona.  

## Instalacja pakietu pythonowego

W katalogu projektu z plikiem `setup.py` wykonujemy następującą komendę:

```bash
pip3 install .
```

> Jeżeli chcemy zainstalować pakiet w środowisku wirtualnym, należy najpierw aktywować środowisko. Dla środowiska wirtualnego utworzonego z wykorzystaniem anaconda należy najpierw stworzyć środowisko komendą conda create env -n <nazwa>, a następnie aktywować je conda activate <nazwa>
> 

## Dokumentacja biblioteki

[Dokumentacja]([https://vistek528.github.io/RythmForge/](https://vistek528.github.io/RythmForge/)) biblioteki zawierająca opisy dostępnych funkcji i przykłady wykorzystania.

## Uruchamianie testów

Aby uruchomić testy jednostkowe funkcji w Python, z poziomu całego projektu wykonać komendę

```jsx
python3 -m pytest tests/
```

## Narzędzia wykorzystane w projekcie

W trakcie pracy nad biblioteką, w celu zachowania jakości i czytelności kodu wykorzystano różne narzędzia jak lintery `ruff` dla Pythona i `cppcheck` dla C++. Znajdowały się lokalnie na maszynach autorów, ale i także w zautomatyzowanym pipeline CI.

Dodatkowo wykorzystywano formatery `black` dla języka Python i `clang-format` dla języka C++.

Na potrzeby projektu stworzono zautomatyzowane środowisko integracyjne na wykorzystując usługę Github Actions. Proces jest podzielony na kolejne etapy i pomyślne jedynia zakończenie pozwala przejść do kolejnego. Po umieszczeniu zmian w zdalnym repozytorium następuje  lintowanie. Następnie rozpoczyna się budowanie pakietu zarówno dla systemu `Ubuntu` jak i systemu `Windows` . Do kompilacji wykorzystywane są dwa kompilatory `g++` dla systemu `Ubuntu` i `MSVC` dla systemu `Windows`. Po etapie kompilacji uruchamiane są testy jednostkowe w języku C++. Na koniec jest budowany pakiet Pyhon i uruchamiane testy są jednostkowe w Pythonie.

## Co udało się zaimplementować?

Na wstępie wypada zaznaczyć, że zagadnienie przetwarzania sygnałów jest bardzo obszerną dziedziną, która również była zupełnie obca przed rozpoczęciem projektu. Przed rozpoczęciem pisania kodu, trzeba było spędzić mnóstwo czasu, aby zrozumieć zagadnienie, podstawy matematyczne i algorytmy. Dodatkowo, pobieżna znajomość tematu utrudniała proces projektowania, testowania i debugowania.

Ostatecznie z listy wstępnych założeń zrealizowano:

- Short Time Fourier Transform
- Inverse Short Time Fourier Transform
- Melspectrogram
- Mel filter bank
- Ładowanie plików w formacie `.wav`
- Fast Fourier Transform (w wersji radix-2 Cooley-Tukey)
- Inverse Fast Fourier Transform
- Kilka funkcji pomocniczych

Nie udało się zrealizować:

- Ładowania plików `.flac` i `.mp3`
- Konwersji pomiędzy typami `.flac` , `.mp3` i `.wav`
- Tempo estimation
- Beat tracking

Napisanie dekodera dla konkretnego formatu audio jest samo w sobie wyzwaniem, nie uwzględniając innych algorytmów zaimplementowanych w bibliotece RythmForge. Autorom biblioteki udało się dodać ładowanie danych z plików w rozszerzeniu `.wav`  aby następnie przechowywać je PCM (Pulse Code Modification), niemniej jednak zrozumienie wszystkich nagłówków (które nie muszą być stałe lub nie muszą wcale występować) zajęło istotną ilość czasu. Całkowicie samodzielne zaimplementowanie dekodowania pilków w innych formatach zajęłoby ok. 1-2 tyg. intensywnej pracy na pojedyńczy format w przypadku zaangażowania jednej osoby. Z racji braku wsparcia dla innych formatów niż `.wav` nie zaimplementowano także modułu konwersji.

Aby estymować pozycję beatów, kluczowe jest najpierw oszacowanie tempa ścieżki dźwiękowej, jednym z problemów jest fakt, że jest to problem niezwykle nie precyzyjny i każdy człowiek może inaczej rozpoznawać tempo utworu, co więcej człowiek w zależności od warunków ten sam utwór może oceniać inaczej. W trakcie przygotowywania merytorycznego zapoznano się z dużą liczbą artykułów naukowych i popularno naukowych, próbowano inspirować się implementacjami innych, między innymi librosa. Nigdy nie udało się jakichkolwiek sensownych rezultatów. 

## Zmiany w stosunku do pierwotnych założeń

Kolejną zmianą w stosunku do początkowych założeń było wykorzystanie biblioteki `multiarray` wchodzącej w skład bibliotek `Boost`  zamiast kontera w postaci wektora wektorów. Zdecydowano się na ten krok, ponieważ w przypadku przechowywania próbek audio dla różnych kanalów czy macierzy STFT zawsze mamy do czynienia ze stałą liczbą elementów w każdej kolumnie macierzy (jest ona prostokątna), ta sama zasada stosuje się w przypadku tensora rzędu 3, który jest zwracany przez STFT - (kanały, częstotliwości, ramki).

Niestety okazało się to błędem, dokumentacja jest uboga i nieprzystępna. Nie znaleziono też dodatkowej lektury czy przykładów. Biblioteka wydaje się zaniedbana i nierozwijana (ostatnia modyfikacja na repozytorium 3 lata temu). Nie zawiera więc ona często elementarnych operatorów takich jak metoda `.at` znana z `std::vector` . Przez to aby dostać się do elementu macierzy, gdy jest ona przechowywana we wskaźniku musimy ciągle używać operatora dereferencji, aby dopiero użyć operatora nawiasów kwadratowych. 

Wykorzystanie algorytmów z biblioteki standardowej też jest ograniczone, ponieważ macierze z biblioteki `boost::multi_array` oczekują, że zapisany zostanie do nich obiekt typu `subarray` , którego autorom biblioteki mimo licznych prób nigdy nie udało się stworzyć. 

Lepszym rozwiązaniem z perspektywy doświadczeń byłoby porzucenie niepraktycznego rozwiązania. Można było przygotować własne kontenery o zbliżonym działaniu, ale z metodami i możliwościami, które były przydatne przy projekcie. Innym rozwiązaniem mogło być wykorzystanie zwykłego `std::vector` , ale z wykorzystaniem `std::mdspan` , który to dołączył do biblioteki standardowej w `C++23` . 

Zostano przy nieoptymalnym rozwiązaniu, w momencie zrozumienia problemu i znalezienia potencjalnych rozwiązań, powstało kilka funckji wykorzystujących `boost::multiarray` i refaktoryzacja całego projektu mogła być ryzykowna i czasochłonna.

## Co możnaby zrobić inaczej

Pierwszą rzeczą jaka nasuwa się na myśl w kwesti zmiany implementacji jest sprawa przejścia z przetwarzania jednowątkowego na przetwarzanie wielowątkowe w przypadku algorytmów `STFT` oraz `ISTFT` . Takie rozwiązanie mogłoby znacząco przyspieszyć wykonywanie się tych funkcji.

## Podsumowanie projektu

Projekt okazał się wymagający, czasowo i intelektualnie. Mierzyliśmy się regularnie różnymi wyzwaniami. Wykorzystaliśmuy nowe dla nas narzędzia. Mimo wszystko dołożyliśmy wszelkich starań, aby zrealizować jak najwięcej planowanych funkcji. 

W trakcie programowania regularnie dążyliśmy do wykorzystywania nowoczesnych ułatwień oferowanych C++20, takich jak koncepty, szablony, algorytmy biblioteki standardowej. Udało nam się skutecznie zintegorać cały pipeline CI oraz faktycznie przygotować działający i przystępny pakiet języka Python.