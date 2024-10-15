///////////////////////// 1° TRABALHO ESTRUTURA DE DADOS III /////////////////////////
// HELOÍSA PAZETI - 14577991                                                        //
// EDUARDO BRIOSO LUCEIRO - 14607621                                                //
//////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcoesArvoreB.h"


int main()
{
    int n;
    char arq[15], arqArvore[15];

    scanf("%d %s %s", &n, arq, arqArvore);

    switch (n)
    {
    case 1:
        EscreverArquivo(arq);
        break;
    case 2:
        EscreverRegistros(arq);
        break;
    case 3:
        BuscarRegistros(arq);
        break;
    case 4:
        RemoverRegistros(arq);
        break;
    case 5:
        InserirRegistros(arq);
        break;
    case 6:
        Compactador(arq);
        break;

    case 7:
        CriarArvore(arq, arqArvore);
        break;

    case 8:
        CriarArvore(arq, arqArvore);
        break;

    case 9:
        CriarArvore(arq, arqArvore);
        break;

    default:
        printf("Essa não é uma opcao valida\n");
        break;
    }

    return 0;
}
