///////////////////////// 2° TRABALHO ESTRUTURA DE DADOS III /////////////////////////
// HELOÍSA PAZETI - 14577991                                                        //
// EDUARDO BRIOSO LUCEIRO - 14607621                                                //
//////////////////////////////////////////////////////////////////////////////////////

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

        default:
            std::cout << "Opcao invalida!\n" << std::endl;
    }
}