#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define RIGHTS 0644
#define BUF_SIZE 1024

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
	
	if((file_wd = open(argv[2], O_WRONLY | O_EXCL, O_CREAT, RIGHTS)) == -1)
	{
		perror("Error open file");
		exit(1);
	}

	int read_bytes;
	char buf[BUF_SIZE];

	while(1) 
	{
		if((read_bytes = read(file_rd, buf, BUF_SIZE) > 0)) 
		{
			if(write(file_wd, buf, BUF_SIZE) == -1) 
			{
				perror("Error writing data to file");
				exit(2);
			}
		}

		if(read_bytes == 0) break;
	}

	if(close(file_rd) == -1) 
	{
		perror("Error closing file decriptor");
		exit(3);
	}
	
	if(close(file_wd) == -1) 
	{
		perror("Error closing file decriptor");
		exit(3);
	}
	
	exit(0);
}