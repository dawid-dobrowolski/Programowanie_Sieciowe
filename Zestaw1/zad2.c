#include <stdio.h>
#include <stdlib.h>

void drukuj(int *tablica, int liczba_elementow) {
    int * p = tablica;

    while( p < tablica + liczba_elementow)
    {
        if(*p > 10 && (*p > 100))
        {
            printf("%d", *p);
        }
        ++p;
    } 
}

int main(int argc, char **argv) 
{
    int array_size = 5;
    int liczby[array_size];
    
    for(int i = 0; i < array_size; i++) 
    {
        int liczba;
        printf("Liczba nr.%d :", i+1);
        scanf("%d", &liczba);

        if(liczba == 0) 
        {
            exit(1);
        }
        liczby[i] = liczba;
    }

    drukuj(liczby, array_size);
    
    return 0;
}