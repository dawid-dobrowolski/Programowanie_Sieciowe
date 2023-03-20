#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

bool printable_buf(const char *buf, int len)
{
    const char *p = buf;
    for(int i = 0; i < len; i++)
    {
        if(*p == 9 || *p == 10 || *p == 13)
        {
            ++p;
            continue;
        }

        if(*p < 32 || *p > 126)
            return false;
        ++p;
    }
    return true;
}

int main(int argc, char* argv[])
{
    int client_desc;
    unsigned short port_number;
    int rc;
    ssize_t read_desc;

    if(argc != 3) 
    {
        perror("Wrong arguments number");
        exit(1);
    }

    client_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(client_desc == -1)
    {
        perror("Socket error");
        exit(2);
    }

    port_number = atoi(argv[2]);
    if(port_number == 0) 
    {
        perror("Atoi error");
        exit(3);
    }

    struct sockaddr_in addr =
    {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = inet_addr(argv[1]) },
        .sin_port = htons(port_number)
    };

    rc = connect(client_desc, (struct sockaddr *) &addr, sizeof(addr));
    if(rc == -1)
    {
        perror("Connect error");
        exit(4);
    }
    
    char buf[15];

    read_desc = read(client_desc, buf, sizeof(buf));
    if(read_desc == -1) 
    {
        perror("Read error");
        exit(5);
    }

    if(!printable_buf(buf, 15))
    {
        perror("Receive messages include unprintable characters");
        exit(6);
    }
    printf("%s \n", buf);

    if(close(client_desc) == -1)
    {
        perror("Unable to close socket descriptor");
        exit(8);
    }
    exit(0);
}