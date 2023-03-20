#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

// Napisz prosty serwer zwracający wizytówkę.
// Powinien tworzyć gniazdko TCP nasłuchujące na porcie o numerze podanym jako argv[1] (użyj socket, bind i listen),
// następnie w pętli czekać na przychodzące połączenia (accept),
// wysyłać ciąg bajtów Hello, world!\r\n jako swoją wizytówkę,
// zamykać odebrane połączenie i wracać na początek pętli.
// Pętla ma działać w nieskończoność, aby przerwać działanie programu trzeba będzie użyć Ctrl-C.


int main(int argc, char* argv[])
{

    int lst_sock;   // gniazdko nasłuchujące
    int clnt_sock;  // gniazdko połączone z bieżącym klientem
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // wyniki zwracane przez read() i write() są tego typu
    int port_nr;
    
    if(argc != 2) {
        perror("Wrong arguments number");
        exit(1);
    }
    port_nr = atoi(argv[1]);
    
    if(port_nr == 0) {
        perror("Error converting port number");
        exit(2);
    }

    lst_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (lst_sock == -1) {
        perror("socket");
        exit(3);
    }
     
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
        .sin_port = htons(port_nr) // w sieciowej kolejnosci bitow
    };

    rc = bind(lst_sock, (struct sockaddr *) & addr, sizeof(addr));
    if (rc == -1) {
        perror("bind");
        exit(4);
    }

    rc = listen(lst_sock, 10);
    if (rc == -1) {
        perror("listen");
        exit(5);
    }

    char message[] = "Hello, world!\r\n";
    bool keep_on_handling_clients = true;
    while (keep_on_handling_clients) {

        clnt_sock = accept(lst_sock, NULL, NULL);
        if (clnt_sock == -1) {
            perror("accept");
            exit(6);
        }

        cnt = write(clnt_sock, message, sizeof(message));
        if (cnt == -1) {
            perror("write");
            exit(7);
        }

        rc = close(clnt_sock);
        if (rc == -1) {
            perror("close");
            exit(8);
        }
    }

    rc = close(lst_sock);
    if (rc == -1) {
        perror("close");
        exit(9);
    }

    exit(0);
}