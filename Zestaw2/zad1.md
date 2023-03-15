# Włączenie klienta przy pomocy ncat jako pierwszego

Włączenie klienta bez wcześniejszego włączenia serwera, kończy się komunikatem "Connection refused", ponieważ nie ma serwera nasłuchującego na wskazanym porcie z którym mógłby połączyć się klient.

# Co się stanie z drugą stroną komunikacji, jeśli w pewnym momencie wyłączymy tą pierwszą (sprawdzane dla TCP).

Jeśli zabijemy klienta, to tym samym zabijemy serwer.

Jeśli zabijemy serwer, klient dalej działa, w momencie wysłania komunikatu do serwera dostaniemy komunikat "Broken pipe" co zakończy działanie naszego klietna.


