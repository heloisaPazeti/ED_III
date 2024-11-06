#include <iostream>
#include <stdio.h>
#include "funcoes.h"

int main()
{
    int n;
    char nomeArq[50];
    scanf("%d %s", &n, nomeArq);

    switch(n)
    {
        case 10:
            CriarGrafo(nomeArq);
            break;
        // case 11:
        // case 12:
        // case 13:
        // case 14:
    }
}