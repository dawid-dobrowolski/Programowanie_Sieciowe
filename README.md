# Programowanie_Sieciowe
```
https://users.uj.edu.pl/~palacz/edu/202223-PS/index.html
```

# Wykład 1 

## Protokół komunikacji 

Protkół to zbiór zasad postępowania ( kto, kiedy, co wysyła ). Jest to dokument w którym szczegółowo jest opisane co i jak jest wysyłane, jaki jest format tych informacji, jak interpretować sekwencje bajtów, które wędrują od procesu do procesu.

Najpopularniejszez protokoły transportowe to UDP i TCP, moduły je obsługujące oraz protokoły znajdujące się "pod nimi" są częścią jądra systemu operacyjnego, a korzystnaie z ich usług jest możliwe za pośrednictwem tzw. gniazdek sieciowych.

Gniazdka sieciowe przypominają tradycyjne deskryptory plików, tak samo jak w przypadku operacji wejścia/wyjścia w systemach UNIX-owych. Tu i ty mamy do czynienia z przsyłaniem ciągiem bajtów.

## TCP a UDP 

### UDP
Jest to protokół datagramowy. Proces komunikacji przypomina ludzi wysyłających do siebie listy tradycyjne. Każdy list jest transmitowany jako niezależna całość. Nie ma gwarancji, że każdy list dojdzie do adresata oraz że listy wysłane w pewnej kolejności, dojdą do adresata w takiej samej kolejności.

W przypadki UDP mamy jakieś niezależne paczki danych, wysyłamy takie paczki i dojdą one lub nie, nie ma na to 100 % gwarancji. 

Do przesyłania długich dokumentów UDP nie za bardzo się nadaje, gdyż datagramy, te porcje danych są stosunkowe małe.

### TCP
Jest to protokół strumieniowy. Mamy taki dwukierunkowy strumień danych, jest on ciągły, nie ma możliwości żeby komunikaty się zgubiły, bądź zmieniły swoją kolejność.

## Deskryptory

Przy operacjach wejścia/wyjścia w przypadku poprawnego otwarcia pliku, jądro systemu zwraca dla nas deskryptor będący nieujemną liczbą naturalną. Poprzez ten deskryptor na pliku można wykonywać inne operacje wejścia/wyjścia.

W przypadku niepoprawnego otwarcia pliku, system zazwyczaj zwraca deskryptor z wartością -1, zawsze należy obsłużyć przypadki w których otwarcie pliku mogło pójść źle.

W momencie skończenia korzystania z plików, należy pamiętać o zamknięciu deskryptorów, pamiętać aby je zwolinić.

Deskryptory nie odnoszą się jedynie do plików, może odnosić się również do urządzenia USB, portu szeregowego czy potoku, według zasady, że w systemach UNIX-owych "wszystko jest plikiem". Tak samo sytuacja ma się w przypadku gniazdek sieciowych.


## Gniazdka API

Gniazdko tworzymy przy pomocy funkcji socket() z argumentami określającymi typ gniazdka (TCP czy UDP) i z którego protokołu sieciowego chcemy korzystać (IPv4 czy IPv6).
Następnie chcąc połączyć gniazdko z jakimś innym procesem czekającym na przychodzące połączenia, korzystamy z funkcji connect(), bierze ona deskrytpor gniazdka, późniejsze wskazują na adres partnera z którym chcemy się połączyć.

Gniazdko, które jest serwerem używa funkcji bind() oraz listen() aby nasłuchiwać. Potem wywołujemy funkcje accept(), która czeka na przychodzące połączenie.
Później do przesyłu bajtów korzystamy z znanym nam funkcji write() oraz read().

Funkcje read() oraz write() ma swoje rozszerzone implementacje:
* recv() i send() 
* recvfrom() i sendto() dla UDP.

### Gniazdka API Dziedziny

Gniazdka należy do pewnej dziedziny komunikacyjnej np. IPv4 i potrafi obsługiwać połączenia w obrębie tej dziedziny. Proces może mieć otwartych kilka gniazdek należących do różnych dziedzin.

Oznaczenia dziedzin w POSIX:
*AF_INET - IPv4
*AF_INET6 - IPv6
*AF_UNIX - gniazdko lokalne widziane jako pseudopliki

AF - address family

### Gniazdka Api - typy gniazdek

POSIX definuje następujące typy gniazdek:
* SOCK_STREAM - niezawodny strumień bajtów (np. TCP)
* SOCK_SEQPACKET - niezawodne datagramy (datagram nie może zginąć, datagramy nie mogą w innej kolejności zostać dostarczone)
* SOCK_DGRAM - zawodne datagramy (np. UDP)
* SOCK_RAW - bezpośredni dostęp do niższych wartsw (tylko z prawami roota, jeśli chcemy wiedzieć co jest przesyłane przez karte sieciową )

Klasyfikacja typów gniazdek jest na ich cechy, czego wynikiem są powyższe klasy dla protokołów transportowych.

Powyższe 4 klasy nie wyczerpują całości, w niektórych systemach operacyjnych są dodatkowe typy gniazdek:
* SOCK_RDM
* SOCK_CONN_DGRAM

## Gniazdka Api - funkcje
### Funkcja socket
Przy pomocy tej funkcji tworzymy nowe gniazdko. Funkcja ta przyjmuje następujące argumenty:
* typ domeny 
* typ gniazdka
* typ protokołu (domyślnie dajemy 0 - jądro wybiera wtedy domyślny protokół) 
Wartość różną od zera trzeba podać tylko wtedy, gdy w danej rodzinie są dwa protokoły danego typu i chcemy użyć tego drugiego.

### Adresy rodziny INET

Każda rodzina ma swoją specyfikacje adresów. Adres składa się z dwóch części, części wskazującej komputer, drugą nie może być proces bo proces może mieć otwarte wiele gniazdek. Druga część musi wskazywać na jakieś jedno gniazdko, w przypadku protokołów internetowych jest to numer portu. Numery portów to liczby naturalne, 16-bitowe. Adresy komputerów to są też liczby naturalne tylko że 32-bitowe. 

Adress gniazdka w C implementujemy jako strukturę. Pierwsze pole zawiera identyfikatora rodzin do której ten adres należy. Ma to sens ze względu że inne funkcje np. connect() jako argument bierze wskaźnik na tą strukturę. Funkcja connect() musi być uniwersalna, więc musi działac z różnymi strukturami rodzin adresowych.
sa_family_t sin_family = INET.
Później numer portu( w sieciowej kolejności bajtów - wynika z zaszłości historycznych, wiele procesorów dawniej, dwie strategie wysyłanie bajtów - najmłodzszy bajt najpierw lub najstarszy bajt najpierw). Sieciowa kolejność bajtów ma standard: starszym bajtem najpierw - żeby nie robić konwesji.
Adres komputera jest zawarty w strukturę, może zawierać dodatkowe pola ale nas nie obchodzi.

### Funkcja connect
Służy do nawiązania połączenia z zdalnym partnerem. Ma ona następujace parametry:
* deskrypotor gniazdka
* wskaźnik na początek wypełnionej stryktury adresowej ( do typu bazowego sockaddr) 
* rozmiar ile bajtów ta struktura liczy ( jądro dzięki temu dokonuje dodatkowych sprawdzeń )

Wywoływana przez klienta

--
Nie wszystkie protokoły sprawdzają czy ktoś po drugiej stronie jest. Tak jest przy TCP ale nie UDP. connect w przpadku UDP zawsze zwróci nam zero, on tylko zapamięta adres na przyszłość.


# Wykład 2
## Funkcje serwera
### Funkcja socket
Opisana wcześniej, służy do tworzenia gniazdka.

### Funkcja bind
Serwer musi ustalić jakiś numer portu, na którym klienci mogą go wywołać. Numer portu nie może być losowo wybrany, musi być znany klientom.

Argumenty funkcji bind:
* deskryptor gniazdka
* strukturę adresową zawierającą adres który chcemy przypisac jako nasz lokalny

Ustalamy port nie adres IP bo on może się zmieniać, może być uruchomiany na wielu komputerach. Drugi powód to taki, że komputer prawie zawsze ma więcej niż jeden taki adres.
Każdy interfejs sieciowy jest związany z pewnym adresem IP, ethernet inny, wifi inne itd, możemy mieć też więcej niż jedną kartę sieciową.
127.0.0.1 - lookback, adres lokalny, wirtualny interfejs sieciowy.

Z wyżej wymienionych powodów w polu na inicjalizacje adresu IP, ustawiamy INADDR_ANY, jądro systemu będzie traktowało to jako "jestem gotów w to miejsce dopasować w to miejsce dowolny adres IP związany z lokalnym komputerem".

Porty 0-1023 są zarezerwowane dla procesów z prawami administratora !!

### Funkcja listen
Przygotowujemy gniazdko do odbierania przychodzących połączeń i te połączenia odbierać. Oznacza gniazdko jako nasłuchujące i ustala długość kolejki połączeń oczekujących. 
Prośba połączenia od klienta trafi do tej kolejki właśnie i czeka na accept()

### Funkcja accept
Funkcja accept() patrzy do kolejki, jeśli jest ktoś w kolejce to bierzemy pierwszego oczekującego, tworzymy gniazdko, które reprezentuje to połączenie z klientem !!!!!!
Po powrocie z accept() mamy dwa gniazdka sieciowe:
* oryginalne do nasłuchiwania
* do rozmawiania z konkretnym klientem

Można uzyskać adres klienta, który się z nami połączył, chyba że jako drugi argument damy NULL.
Co się dzieje jeśli kolejka połączeń jest pusta ?? Zawiesi program serwera, do momentu aż klient się pojawi, jak się pojawi to nawet bez kolejki natychmiast zostanie stworzonę gniazdko sieciowe.

### funkcja shutdown
Funkcja ta to uogólnione close(), gniazdko reprezentuje dwukierunkowe połączenie, to można zamknąc je tylko w jednym kierunku, drugi pozostawiając otwarty, close() natomiast zamyka tą komunikacje w dwóch kierunkach.

* SHUT_RD
* SHUT_WR
* SHUT_RDWR

Korzystanie z SHUT_WR można w ten sposób zasygnalizować koniec wysyłanych przez nas dancyh a potem jeszcze odebrać finalną odpoweidź od procesu na drugim końcu gniazdka.

## Gniazdka bezpołączeniowe

Gniazdka UDP mogą wysyłać i odbierać datagramy w tzw. trybie bezpołączeniowym, w którym gniazdko nie ma ystalonego adrresu zdalnego. Każdy transmitowany datgram może mieć inny adres zdalny. 
Mamy tylko jedno gniazdko na nim nasłuchuje i z niego odbiera datagramy i z niego wysyła datagramy. Mamy tu funkcje sendto() i recvfrom().
W tym wypadku mając jedynie jedno gniazdko sieciowe, recvfrom i sendto zawierają adres na który wysyłamy i z którego odbieramy dany datagram.

## Nieblokujące deskryptory
Funkcja accept() jest funkcją blokującą, podobnie jest z odczytem read(), recvfrom() one się zawieszą jeśli nie było czekających danych. Domyślnie ten okres czekania jest nieskończony, można temu zaradzić.

### Przestawienie deskryptora w tryb nieblokujący
Wyciąga się flagi z deskryptora przez fcntl, pobieramy aktualnie flagi, dodajemy flagę O_NONBLOCK i ustawiamy flagi na nowo, od teraz jak coś się zawiesi choćby na chwilę to dostaniemy -1. Możemy wtedy zasypiać na 0.5 sek i próbować na nowo, nie jeset to optymalne.

Drugi pomysł to ustawienie timeoutu na gniazdkach, jest to możliwe tylko na gniazdkach sieciowych. Korzystamy tu z fubkcji setsockopt().

