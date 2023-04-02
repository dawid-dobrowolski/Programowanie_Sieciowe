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

# Wykład 3
## Model ISO/OSI
Model ISO/OSI jest modelem koncepcyjnym, określa w jaki sposób należy dzielić skomplikowane sieci telekomunikacyjny, aby można było zapanować nad złożonością. Model ISO/OSI proponuje podział na siedem warstw.Każda z nich kontaktuje się tylko z wartstwami sąsiednimi.

Tzw. Model internetowy ma tylko cztery warstwy.

## Warstwy modelu ISO/OSI
Od najniższej do nawyższej
1.Fizyczna
Zagadnienia związane w jaki sposób urządzenia sieciowe są połączone, jakie medium transmisyjne są używane skrętka/światłowód. Określa się tu również w jaki sposób modulowane są te sygnały, żeby można wyrazić 0 i 1.

2.Łącza danych

W jaki sposób komunikują się ze sobą komputery połączone wspólnym medium transmisyjnym( połączone jakimś kablem lub łącze radiowe pozwala przesyłanie danych z jednego na drugi ze względu na niewielką odległość pomiędzy nimi.
Jeśli są to komputery na różnych kontynentach to warstwa 2 nie ma tu nic do czynienia.

Jeśli mamy jakieś medium do którego wiele urządzeń ma dostęp(np. fale radiowe) to musimy mieć sposób adresacji tych urządzeń. Nadawca nadaje infomracje trzeba wskazać kto ma to odebrać. Adres warstwy drugiej/adres łącza danych/ adres MAC.

Czyli definiujemy adres MAC.

Musimy zdefiniować również format ramki sieciowej.Dzisiejsze sieci są sieciami pakietowymi, wymieniamy pewne skończone porcje danych, nie są to datagramy bo jest to warstwa 4. Można na to mówić jako paczka/ ramka danych. W sieciach lokalnych Ethernet/Wifi przesyłane są ramki danych, musimy zdefiniować jak taka ramka wygląda. Jej nagłówek(adres odbiorcy, nadwacy, suma kontrolna), jeśli medium transmisyjne jest zawodne to pewnie też mechanizm odsyłania potwierdzeń, jeśli w rozsądnym czasie potwierdzenie nie wróci, to nadawca dokonuje retransmisji, tak działają bezprzewodowe sieci WiFi, w przypadku Ethernet nie ma takiego mechanizmu, ponieważ gubienie ramek jest bardzo rzadkie i nie ma to sensu.

Ethernet nie daje gwarancji, że ramka nie zginie, odpowiedzialność za retransmisje jest spychana do warstw wyższych.

3.Sieciowa

Mamy już jakieś sieci lokalne, które chcemy połączyć w większe sieci miejskie/globalne. Tutaj wchodzi  wartswa trzecia, warstwa sieciowa. Chodzi tutaj o sieć rozległą, np. Internet. W tej warstwie działa protokół IP - Internet Protocol, zajmuje się on tym jak znaleźć ścieżkę, połączenie która pozwoli dostarczyć pakiet IP z komputera w Polsce do komputera na innym kontynencie. Pojawiają się adresy unikalne w skali globalnej, pokazują się narzędzie tzw. routery, których zadaniem jest przekazyuwanie pakietów od nadawcy poprzez kolejne routery aż do docelowego komputera(one muszą wiedzieć jakie ścieżki wybierać).

4.Transportowa

Tu są protokoły transportowe, do których dają nam dostęp gniazdka unixowe UDP/TCP.Tu zajmujemy się przekazywaniem danych nie od komputera do komputera lecz od działającego procesu do działającego procesu. Są różne rodzaje protkołów strumieniowe/datagramowe, dokonujące retransimsje i nie itd.
Nie ma czegoś takiego jak niezawodny protokół transportowy. 
O niezawodnym protkole mówi, że nam zgłosi błąd jak nie będzie w stanie nam zapewnić żeby te dane prszyszły.

5.Sesji

Chodzi o to żeby z komputera z pracy i z domu można kontynuować tą samą pracę korzystające z połączenia z jakimś serwerem np. bazodanowym.

6.Prezentacji

Straciło to w dzisiejszych czasach na znaczeniu, wszystkie systemu posiadają kodowanie UTF-8, dane są przekazywane porcjami bajtów itd. dużo ustandaryzowania.
Wszędzie jest ASCII albo Unicode z UTF-8, nie zawsze tak było.

7.Aplikacji

Pojęcia specyficzne dla konkretnego zastosowania. Jeśli chcę pobierać dane z serwera to muszę zdefiniować jak będą wyglądały nazwy tych plików, jak poprosić o takie pliki itd.Pojęcia specyficzne na poziomie przesyłania plików.
Z drugiej strony jak byśmy chcieli przesyłać listy elektroniczne to trzeba wyspecyfikowac jak takie listy się adresuje, odbiera, czy są potwierdzenia itd.

Człowiek-użytkownik jest w warstwie 8.

Rzeczywiste standardy:
Warstwa 1 i 2 - Ethernet
Warstwa 3 - IP
Warstwa 4 i 5 - TCP I UDP
Warstwa 7 - Poczta, WWW, SSH

W rzeczywistości warstwy sąsiednie są łączone w jedną np. 1 i 2, wartstwa zazwyczaj jest samodzielna, warstwa 4 czasami ma trochę z warstwy 5. Warstwa 6 jest w zaniku, warstwa 7 to wiele protokółów z których korzystamy HTTP, SSH itd.

## Adresowanie w poszczególnych warstwach
Ethernet - 48-bitowe(6 bajtów), nadawany przez producentów sprzętu, zapewniając ich globalną unikalność.

Wi-Fi - wspólna przestrzeń adresowa z Ethernet. Zaprojektowano tak, żeby mógł współistnieć z Ethernet.

IPv4(warstwa trzecia) - 32 bitowe(4 bajty) przydzielane przez operatorów sieci szkieletowych(providerów internetu), przydzielona pula 
pomiędzy kontynenty,kraje, miasta i tak w dół, aż do sieci loklanej.

IPv6 - 128-bitowe(16 bajtów) jak wyżej przy IPv4.

TCP - 16 bitowe numery portów, przydzielone przez IANA - Internet Assigned Numbers Authority. Istnieją "dobrze znane numery portów" np.
80 -HTTP
22 -SSH

UDP - podobnie z odrębną przestrzeń numerów.
Można mieć dwa procesy na tym samym porcie pod warunkiem, że jeden korzysta z TCP drugi z UDP. Inaczej ten port traktują.

Warstwa 7 to dużo adresów ale między innymi poczta czy URL

## Walidacja danych wejściowych
Przy pisaniu aplikacji sieciowych obowiązuje zasada ograniczonego zaufania.Odczytywanie z gniazdek ciągi bajtów nie zawsze będą zgodne ze specyfikacją protokołu.

## Automaty
Do walidacji zapytań można użyć automatu.Deterministyczny automat skończony ( albo "skończenie stanowy") definiuje się w oparciu o:
- zbiór symboli z których skadają się ciągi dostarczane na wejście automatu
- zbiór stanów, w których automat może się znajdować( jeden z nich wyróżniony jako początkowy)
- funkcje przejścia, określającą przejścia między stanami pod wpływem przetwarzanych symboli.Bierze aktualny stan, symbol z wejścia i stan do którego przejdzie.

Automat maszynka która połyka następne symbole, jak zjada symbole to wykonuje pewne czynności i tak aż skończą się symbole. Mamy odpowiedź 0 1 był poprawny nie był poprawny.

## Automat walidujący ciąg liczb

Podstawowym typem automatu są automaty akceptujące, mający wyróżniony zbiór stanów akceptujących. Jeśli po zakończeniu przetwarzania ciągu wejściowego automat jest w jednym z tych stanów, to ciąg był poprawny.

Można zaprojektować jakieś 4 stany
I - stan początkowy
C - ostatnio przetworzony symbol to liczba
S - ostatnio przetworzony symbol to spacja
ERR - poprzednio przetworzony symbol nie układają się w poprawny ciąg

Jedynym stanem akceptującym jest C.

## Automaty z akcjami
Chodzi o to, żeby nie robić osobno walidacji a później jakieś akcje bo dwa razy robimy to samo często. Nie zawsze jest to możliwe, ale jeśli jest to możliwe to powinniśmy odrazu robić walidacje z akcjami, czyli korzystamy automat z akcjami, czyli nie tylko zmienia się stan automatu ale także wykonujemy dodatkowe czynności.

## Automaty i przetwarzanie strumieni danych
Podejście to wygodne jest to o protokoł korzystających z transportu strumieniowego, nie mających ograniczenia na długość przesyłanych komunikatów (np. sumator TCP).
Wczytywanie i przetwarzanie musi być robione porcjami o rozsądnym rozmiarze. Przetwarza to co się mu udało zapisać w buforze, wczytuje kolejną porcje danych i przetwarza kolejną porcje danych.
