///////////////////////// 2° TRABALHO ESTRUTURA DE DADOS III /////////////////////////
// HELOÍSA PAZETI - 14577991                                                        //
// EDUARDO BRIOSO LUCEIRO - 14607621                                                //
//////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoesArvoreB.h"

int main() 
{
    int n;
    char nomeArq[50];
    char nomeArqArvore[50];

    scanf("%d %s %s", &n, nomeArq, nomeArqArvore);

    switch (n)
    {
        case 7:
            CriarArvore(nomeArq, nomeArqArvore);
            break;

        case 8:
            BuscarRegistroArvore(nomeArq, nomeArqArvore);
            break;

        case 9:
            AdicionarRegistroArvore(nomeArq, nomeArqArvore);
            break;

        default:
            printf("Essa não é uma opcao valida\n");
            break;
    }

    return 0;
}
