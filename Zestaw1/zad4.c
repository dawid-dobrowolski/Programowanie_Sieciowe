#include <stdbool.h>
#include<stdio.h>

bool printable_buf(const void *buf) 
{
    const char *p = buf;
    while(*p != '\0')
    {
        if(*p < 32 || *p > 126) 
            return false;
        ++p; 
    }   
    return true;
}

int main(int argc, char* argv[])
{
    char chars[10];
    printf("Podaj dowolny ciag znakow :");
    scanf("%s", chars);

    bool flag = printable_buf(chars);
    if(flag == true) 
        printf("Przekazany bufor zawiera wyłacznie znaki ASCII \n");
    else 
        printf("Przekazany bufor zawiera znaki poza ASCII \n");
    return 0;
}