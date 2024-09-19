#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"



int main()
{
    int n;
    char nomeArq[15];

    scanf("%d %s", &n, nomeArq);

    switch (n)
    {
    case 1:
        EscreverArquivo(nomeArq);
        break;
    case 3:
        //BuscarRegistros(nomeArq);
        break;
    
    default:
        printf("Essa não é uma opcao valida\n");
        break;
    }

    return 0;
}