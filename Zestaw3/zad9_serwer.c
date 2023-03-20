#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char* argv[]) 
{
	int server_desc;
	int rc;
	ssize_t cnt;

	if(argc != 2) 
	{
		perror("Wrong argument number");
		exit(1);
	}

	server_desc = socket(AF_INET, SOCK_DGRAM, 0);
	if(server_desc == -1)
	{
		perror("Socket error");
		exit(2);
	}

	int port_number = atoi(argv[1]);
	if(port_number == 0) 
	{
		perror("Atoi error");
		exit(3);
	}

	struct sockaddr_in addr =
	{
		.sin_family = AF_INET,
		.sin_addr = { .s_addr = htonl(INADDR_ANY)},
		.sin_port = htons(port_number)
	};

	rc = bind(server_desc, (struct sockaddr*) &addr, sizeof(addr));
	if(rc == -1)
	{
		perror("Bind error");
		exit(4);
	}

	char message[] = "Hello, world!\r\n";
	bool keep_on_handling_clients = true;
	while(keep_on_handling_clients)
	{
		unsigned char buf[15];
		struct sockaddr_in clnt_addr;
		socklen_t clnt_addr_len = sizeof(clnt_addr);

		cnt = recvfrom(server_desc, buf, sizeof(buf), 0, (struct sockaddr*) &clnt_addr, &clnt_addr_len);
		if(cnt == -1)
		{
			perror("Recvfrom error");
			exit(5);
		}
		
		cnt = sendto(server_desc, message, sizeof(message), 0, (struct sockaddr*) &clnt_addr, clnt_addr_len);
		if(cnt == -1)
		{
			perror("Sendto error");
			exit(6);
		}
		if(cnt != sizeof(message))
		{
			perror("Sendto error");
			exit(7);
		}
	}
	if(close(server_desc) == -1)
	{
		perror("Close error");
		exit(7);
	}
	exit(0);
}