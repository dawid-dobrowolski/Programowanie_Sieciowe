# Sprawdź co się teraz będzie działo, gdy jeden z działających netcatów zabijesz przez Ctrl-C. Co się zmieniło w porównaniu do eksperymentów z TCP? I czy treść wyświetlanych komunikatów o błędach jest taka sama?

Zabicie procesu klienta, nie zamyka natychmiastowo serwera tak jak to miało miejsce w przypadku TCP, w dalszym ciągu można z strony serwera wysyłać komunikaty, jednakże do nikogo one nie trafiają ze względu, że klient już nie działa.

Co ciekawe po ponownym podłączeniu klienta, wiadomości które wysyła serwer, nie są już odbierane przez klienta, do momentu w którym nie wyśle on komunikatu. W momencie kiedy klient wyślę ponownie wiadomość do serwera wiadomości od niego są odbierane. --> Jest to przypadek ogólny. Dopiero w momencie gdy klient wyślę wiadomość do serwera, wszystkie wysłane wiadomości od serwera są odebrane przez klienta, później komunikacja wygląda normalnie i wiadomości są dostarczane natychmiastowo.

W przypadku wyłączenia serwera, klient dalej chodzi, w momencie chęci wysłania komunikatu do serwera, dostajemy komunikat "Connection refused" i klient zakończy swoją pracę.
