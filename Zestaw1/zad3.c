#include<stdlib.h>
#include <stdbool.h>
#include<stdio.h>
#include<string.h>

bool printable_buf(const void *buf, int len) 
{
    printf('%p', buf);
    return true;
}

int main(int argc, char **argv)
{
    char chars[1024];
    printf("Podaj dowolny ciag znakow :");
    fgets(chars, sizeof(chars), stdin);
    printf("Wczytany ciag znakow %s", &chars);

    printable_buf((void *)chars,strlen(chars));
}