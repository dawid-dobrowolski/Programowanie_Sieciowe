#include <stdio.h>

int main(int argc, char **argv) 
{

    if(argc == 0) 
    {
        perror("Wrong number of arguments");
        exit(1);
    }
    int array_size = 50;
    int liczby[array_size];
    return 0;
}