#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
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
	ssize_t cnt;

	if(argc != 3)
	{
		perror("Wrong argument number");
		exit(1);
	}

	client_desc = socket(AF_INET, SOCK_DGRAM, 0); 
	if(client_desc == -1)
	{
		perror("Socket error");
		exit(2);
	}

	int port_number = atoi(argv[2]);
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


	cnt = sendto(client_desc, "", 0, 0, (struct sockaddr*) &addr, sizeof(addr));
	if(cnt == -1)
	{
		perror("Sendto error");
		exit(4);
	}

	char buf2[15];
	socklen_t size = sizeof(addr);
	cnt = recvfrom(client_desc, buf2, sizeof(buf2), 0,  (struct sockaddr*) &addr, &size);
	if(cnt == -1)
	{
		perror("Recivefrom error");
		exit(5);
	}

	if(!printable_buf(buf2, 15))
    {
    	sleep(1);
        perror("Receive messages include unprintable characters");
        exit(6);
    }
    printf("%s \n", buf2);

    if(close(client_desc) == -1)
    {
        perror("Unable to close socket descriptor");
        exit(8);
    }
    exit(0);
}