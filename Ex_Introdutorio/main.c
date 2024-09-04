#include <stdlib.h>
#include <stdio.h>
#include "funcoes.h"

int main()
{
    int n;
    char arquivo[15];

    scanf("%d ", &n);
    scanf("%s", arquivo);

    switch (n)
    {
        case 1:
            registrarEspecie(arquivo);
            break;

        case 2:
            relatorioEspecies(arquivo);
            break;

        case 3:
            buscarEspecie(arquivo);
            break;
    
        case 4:
            registrarInformacao(arquivo);
            break;

        default:
            printf(">> Comando não encontrado...\n");
            break;
    }

    return 0;
}
