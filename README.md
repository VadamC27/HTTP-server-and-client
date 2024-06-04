# Klient i serwer HTTP napisany z wykorzystaniem połaczenia TCP.

## Temat: 10. Prosty serwer protokołu HTTP zgodny ze specyfikacją RFC 2616 co najmniej w zakresie żądań: GET, HEAD, PUT, DELETE

## Protokół komunikacyjny
Serwer odbiera żądania od klienta poprzez połączenie TCP. Najpierw odczytywane jest żądanie od klienta, a następnie generowana jest odpowiedź, która na końcu jest wysyłana. Serwer obsługuje żądania typu GET, HEAD, PUT, DELETE i obsługuje ścieżki z wieloma znakami "/".
Nadawane spowrotem mogą być odpowiedzi o kodach 200, 201, 400, 403, 404, 405 i 500.  

## Opis implementacji
Kod serwera napisany został w jezyku C dla systemu Linux, natomiast klient w języku C# z pomocą Visual Studio dla systemu Windows.

Serwer składa się z dwóch plików .c, pierwszy server.c zawiera kod odpowiedzialny za nawiązywanie połączeń w utworzonych wątkach, natomiast HTTPprocessing.c zawiera funkcje odpowiedzialne za przetwarzanie żądań i generowanie odpowiedzi.

Główna funkcjonalność klienta znajduje się w pliku Form1.cs gdzie napisane są funkcje odpowiedzialne za wykonanie połaczenia z odpowiednim żądaniem po wciśnięciu odpowiedniego przycisku. Program.cs zawiera punkt wejściowy programu, a plik Form1.Designer.cs zawiera informacje o UI programu, pozostałe pliki są generwoane automatycznie przez środowisko Visual Studio.

## Sposób instalacji i uruchomienia
Aby skompilować i uruchomić serwer należy skorzystać z pliku makefile komendą make i następnie uruchomić plik server. Serwer działa od momentu uruchomienia i nasłuchuje na porcie 8080 tak jak każdy serwer HTTP.

Aby skompilować i uruchomić klienta należy skorzystać z pliku projektu .sln dla programu Visual Studio. W programie należy umieścić rządany adres w górnym pasku, a następnie wybrać metode żądania http, dodatkowo dla metody PUT można wprowadzić w pole po prawej stronie tekst.