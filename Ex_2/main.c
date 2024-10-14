///////////////////////// 2° TRABALHO ESTRUTURA DE DADOS III /////////////////////////
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
    char nomeArq[15];
    char nomeArqArvore[15];

    scanf("%d %s", &n, nomeArq);
    scanf("%d %s", &n, nomeArqArvore);

    switch (n)
    {
    case 1:
        EscreverArquivo(nomeArq);
        break;
    case 2:
        EscreverRegistros(nomeArq);
        break;
    case 3:
        BuscarRegistros(nomeArq);
        break;
    case 4:
        RemoverRegistros(nomeArq);
        break;
    case 5:
        InserirRegistros(nomeArq);
        break;
    case 6:
        Compactador(nomeArq);
        break;

    case 7:
        CriarArvore(nomeArq, nomeArqArvore);
        break;

    case 8:
        BuscarRegistroArvore(nomeArq, nomeArqArvore);
        break;

    case 9:
        AdicionarRegistroARvore(nomeArq, nomeArqArvore);
        break;

    default:
        printf("Essa não é uma opcao valida\n");
        break;
    }

    return 0;
}