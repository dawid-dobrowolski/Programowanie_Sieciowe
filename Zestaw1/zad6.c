#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define RIGHTS 0644
#define BUF_SIZE 3

int main(int argc, char* argv[]) 
{
	if(argc != 3) 
	{
		perror("Wrong arguments number");
		exit(1);
	}

	int file_rd,file_wd;
	
	if((file_rd = open(argv[1],O_RDONLY)) == -1) 
	{
		perror("Error open file");
		exit(1);
	}
	
	if((file_wd = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, RIGHTS)) == -1)
	{
		perror("Error open file");
		exit(1);
	}

	ssize_t read_bytes;
	char buf[BUF_SIZE];

	while(1) 
	{
		read_bytes = read(file_rd, buf, BUF_SIZE); 
		if(read_bytes == -1) {
			perror("Error reading data from file");
			exit(3);
		}
		if(read_bytes == 0) break;	
		
		if(write(file_wd, buf, read_bytes) == -1) 
		{
			perror("Error writing data to file");
			exit(2);
		}
	}

	if(close(file_rd) == -1) 
	{
		perror("Error closing file decriptor");
		exit(4);
	}
	
	if(close(file_wd) == -1) 
	{
		perror("Error closing file decriptor");
		exit(4);
	}
	
	exit(0);
}