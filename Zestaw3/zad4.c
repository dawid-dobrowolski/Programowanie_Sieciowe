#define _POSIX_C_SOURCE 200809L
#define UDP_MAX_PAYLOAD 65507 //theoretical limit is 65535 but, due to IPv4 protocol is 65507
#define PORT_NUMBER 2020
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>

int main(int argc, char* argv[]) 
{
	int server_desc;
	int rc;
	bool errorFlag = false;
	int cnt;
	int resultNumber = 0;

	server_desc = socket(AF_INET, SOCK_DGRAM, 0);
	if(server_desc == -1) 
	{
		perror("Socker error");
		exit(2);
	}

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT_NUMBER);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	rc = bind(server_desc, (struct sockaddr*) &addr, sizeof(addr));
	if(rc == -1)
	{
		perror("Bind erorr");
		exit(3);
	}
	while(true)
	{

		struct sockaddr_in clnt_addr;
		socklen_t clnt_addr_lenght = sizeof(clnt_addr);
		char buf[UDP_MAX_PAYLOAD];
		char liczba[UDP_MAX_PAYLOAD];

		cnt = recvfrom(server_desc, buf, UDP_MAX_PAYLOAD, 0, (struct sockaddr*) &clnt_addr, &clnt_addr_lenght);
		if(cnt == -1)
		{
			perror("Recvfrom error");
			exit(4);
		}
		// wyrazenie nie moze zawierac spacji - ascii 32
		// moze zawierac jedynie liczby 0-9 + operatory dodawania i odejmowania
		// czyli dozwolone znaki ascii to 43,45,48-57 oraz znaki spec. 10,13
		// nie moze byc podwojnego operatora lub operator bez poprzedzajacej liczby 
		// nie moze byc liczb calkowitych np -20 + 60
		// wyrazenie puste ma byc bledem
		// sam operator bledem
		// przepelnienie bledem

		// Sprawdzam czy datagram nie jest pusty, jeśli tak to wysyłam kod błędu
		if(cnt != 1 && cnt != 0)
		{
			resultNumber = 0;
			int j = 0;
			int operator = 0;
			memset(liczba, 0, sizeof(liczba));
			errorFlag = false;
			for(int i = 0; i < cnt; i++)
		 	{
		 		//fprintf(stderr, "Jaki wynik narazie: %d \n", resultNumber);
		 		if((buf[i] == 32) || ((buf[i] == 43 || buf[i] == 45) && liczba[0] == '\0')) 
		 		{
		 			errorFlag = true;
		 			break;
		 		}
		 		if(buf[i] == 10 || (buf[i] == 13 && buf[i+1] == 10))
		 		{
		 			if(operator == 0)
		 			{
		 				resultNumber = atoi(liczba);
		 				fprintf(stderr, "Koniec ! Wynik : %d\n", resultNumber);
		 				break;
		 			}
		 			if(operator == 43)
		 			{
		 				resultNumber += atoi(liczba);
		 				fprintf(stderr, "Koniec ! Wynik : %d\n", resultNumber);
		 				break;
		 			}
		 			if(operator == 45)
		 			{
		 				resultNumber -= atoi(liczba);
		 				fprintf(stderr, "Koniec ! Wynik : %d\n", resultNumber);
		 				break;
		 			}
		 		}
		 		if(buf[i] >= 48 && buf[i] <= 57)
		 		{
		 			//fprintf(stderr, "Liczba ! : %d\n", buf[i]);
		 			liczba[j] = buf[i];
		 			//fprintf(stderr, "Co w tablicy liczb ? : %s \n", liczba);
		 			j++;
		 		}
		 		if(buf[i] == 43)
		 		{
		 			fprintf(stderr, "Dodawanie ! : %s \n", liczba);
		 			if(operator == 0)
		 			{
		 				resultNumber = atoi(liczba);
		 			}
		 			if(operator == 43)
		 			{
		 				resultNumber += atoi(liczba);
		 			}
		 			if(operator == 45)
		 			{
		 				resultNumber -= atoi(liczba);
		 			}
		 			memset(liczba, 0, sizeof(liczba));
		 			j=0;
		 			operator = 43;
		 		}
		 		if(buf[i] == 45)
		 		{	
		 			//fprintf(stderr, "Odejmowanie ! : %s \n", liczba);
		 			if(operator == 0)
		 			{
		 				resultNumber = atoi(liczba);
		 			}
		 			if(operator == 43)
		 			{
		 				resultNumber += atoi(liczba);
		 			}
		 			if(operator == 45)
		 			{
		 				resultNumber -= atoi(liczba);
		 			}
		 			operator = 45;
		 			memset(liczba, 0, sizeof(liczba));
		 			j=0;
		 		}
		 		else {
		 			errorFlag = true;
		 			break;
		 		}
		 	}
		}
		else
		{
			char errorMessage[] = "ERROR\n";
			cnt = sendto(server_desc, errorMessage, sizeof(errorMessage), 0, (struct sockaddr*) &clnt_addr, clnt_addr_lenght);
			if(cnt == -1)
			{
				perror("Sendto error");
				exit(5);
			}
		}
		if(errorFlag == true)
		{
			char errorMessage[] = "ERROR\n";
			cnt = sendto(server_desc, errorMessage, sizeof(errorMessage), 0, (struct sockaddr*) &clnt_addr, clnt_addr_lenght);
			if(cnt == -1)
			{
				perror("Sendto error");
				exit(5);
			}
		}
		 else
		 {
			char result[UDP_MAX_PAYLOAD];
			sprintf(result, "%d",resultNumber);
			cnt = sendto(server_desc, (void *)result, sizeof(result), 0, (struct sockaddr*) &clnt_addr, clnt_addr_lenght);
			if(cnt == -1)
			{
				perror("Sendto error");
				exit(5);
			}
			if(cnt != sizeof(result))
			{
				perror("Sendto error");
				exit(6);
			}
		} 
	}
	if(close(server_desc) == -1)
	{
		perror("Close error");
		exit(7);
	}
	exit(0);
}