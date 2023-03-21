#define _POSIX_C_SOURCE 200809L
#define UDP_MAX_PAYLOAD 65535
#define PORT_NUMBER 2020
#define INT_16LIMITER 5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>

bool atoiHandler(int number, char * numberArray, int size)
{
	for(int i = size-1; i >=0; i--)
	{
		int partialNumber = number % 10;
		if((numberArray[i]-'0') != partialNumber)
			return false;
		number /= 10;
	};
	return true;
}

bool isOverflowForInt16Add(int wynik, int liczba)
{
	if(SHRT_MAX-wynik-liczba < 0)
	{
		return true;
	}
	return false;
}

bool isOverflowForInt16Sub(int wynik, int liczba)
{
	if(SHRT_MIN+wynik+liczba > 0)
	{
		return true;
	}
	return false;
}


int main(int argc, char* argv[]) 
{
	int server_desc;
	int rc;
	bool errorFlag = false;
	int cnt;
	int16_t resultNumber = 0;
	int16_t number;

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
		char liczba[INT_16LIMITER+1];
		cnt = recvfrom(server_desc, buf, UDP_MAX_PAYLOAD, 0, (struct sockaddr*) &clnt_addr, &clnt_addr_lenght);
		if(cnt == -1)
		{
			perror("Recvfrom error");
			exit(4);
		}

		// check if datagram isn't empty
		if(cnt == 1 || cnt == 0)
		{
			errorFlag = true;
		}
		else {
				int j = 0;
				int operator = 0;

				for(int i = 0; i < cnt; i++)
		 		{
		 			// check if the expression doesn't contain illegal configurations
		 			if((buf[i] == 32) || ((buf[i] == 43 || buf[i] == 45) && liczba[0] == '\0')) 
		 			{
		 				errorFlag = true;
		 				break;
		 			}
		 			// procedure if end of datagram
		 			else if(buf[i] == 10 || (buf[i] == 13 && buf[i+1] == 10))
		 			{
		 				if(operator == 0)
		 				{
		 					number = atoi(liczba);
		 					if(!atoiHandler(number, liczba, j))
		 					{
		 						errorFlag = true;
		 						break;
		 					}
							resultNumber = number;
		 				}		
		 				else if(operator == 43)
		 				{
		 					number = atoi(liczba);
		 					if(!atoiHandler(number, liczba, j))
		 					{
		 						errorFlag = true;
		 						break;
		 					}
		 					if(isOverflowForInt16Add(resultNumber, number))
		 					{
		 						errorFlag = true;
		 						break;
		 					}
							resultNumber += number;
		 				}	
		 				else if(operator == 45)
		 				{
		 					number = atoi(liczba);
		 					if(!atoiHandler(number, liczba, j))
		 					{
		 						errorFlag = true;
		 						break;
		 					}
		 					if(isOverflowForInt16Sub(resultNumber, number))
		 					{
		 						errorFlag = true;
		 						break;
		 					}
							resultNumber -= number;
		 				}	
		 				break;
		 			}
		 			// if digit
		 			else if(buf[i] >= 48 && buf[i] <= 57)
		 			{
		 				// infinity '0' digit protection
		 				if((buf[i] == 48 && buf[i+1] == 48) && liczba[0] == '\0') continue;
		 				
		 				// catch obvious overflow
		 				if(j == INT_16LIMITER)
		 				{
		 					errorFlag = true;
		 					break;
		 				}
		 				liczba[j] = buf[i];
		 				j++;
		 			}
		 			// if operator
		 			else if(buf[i] == 43 || buf[i] == 45)
		 			{
		 				if(operator == 0)
		 				{
		 					number = atoi(liczba);
		 					if(!atoiHandler(number, liczba, j))
		 					{
		 						errorFlag = true;
		 						break;
		 					}
		 					resultNumber = number;
		 				}
		 				if(operator == 43)
		 				{
		 					number = atoi(liczba);
		 					if(!atoiHandler(number, liczba, j))
		 					{
		 						errorFlag = true;
		 						break;
		 					}
		 					if(isOverflowForInt16Add(resultNumber, number))
		 					{
		 						errorFlag = true;
		 						break;
		 					}
		 					resultNumber += number;
		 				}
		 				if(operator == 45)
		 				{
		 					number = atoi(liczba);
		 					if(!atoiHandler(number, liczba, j))
		 					{
		 						errorFlag = true;
		 						break;
		 					}
		 					if(isOverflowForInt16Sub(resultNumber, number))
		 					{
		 						errorFlag = true;
		 						break;
		 					}
		 					resultNumber -= number;
		 				}
		 				memset(liczba, 0, sizeof(liczba));
		 				j=0;
		 				operator = buf[i];
		 			}
		 			else
		 			{
		 				errorFlag = true;
		 				break;
		 			}
		 		}	
			}
		if(errorFlag == true)
		{
			char errorMessage[] = "ERROR\r\n";
			cnt = sendto(server_desc, errorMessage, sizeof(errorMessage), 0, (struct sockaddr*) &clnt_addr, clnt_addr_lenght);
			if(cnt == -1)
			{
				perror("Sendto error");
				exit(5);
			}
		}
		 else
		 {
		 	int digitNumber = 0;
		 	for(int i = resultNumber; i != 0; i /= 10)
		 	{
		 		digitNumber++;
		 	}
		 	if(resultNumber == 0) digitNumber++;
		 	if(resultNumber < 0 ) digitNumber++;
			char result[digitNumber + 2];
			sprintf(result, "%d\r\n", resultNumber);
			
			cnt = sendto(server_desc, result, sizeof(result), 0, (struct sockaddr*) &clnt_addr, clnt_addr_lenght);
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
		memset(liczba, 0, sizeof(liczba));
		errorFlag = false;
		resultNumber = 0;
	}
	if(close(server_desc) == -1)
	{
		perror("Close error");
		exit(7);
	}
	exit(0);
}