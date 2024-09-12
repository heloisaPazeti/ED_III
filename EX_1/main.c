#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

int main()
{
    int n;

    scanf("%d", &n);

    switch (n)
    {
    case 1:
        EscreverArquivo();
        break;
    
    default:
    printf("Essa não é uma opcao valida\n");
        break;
    }

    return 0;
}