#include <stdio.h>
#include <stdlib.h>

void drukuj(int tablica[], int liczba_elementow) {
    for(int j=0; j < liczba_elementow; j++)
    {
        if(tablica[j] > 10 && (tablica[j] < 100))
        {
            printf("%d \n", tablica[j]);
        }
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