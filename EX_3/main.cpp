///////////////////////// 3° TRABALHO ESTRUTURA DE DADOS III /////////////////////////
// HELOÍSA PAZETI - 14577991                                                        //
// EDUARDO BRIOSO LUCEIRO - 14607621                                                //
//////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <stdio.h>
#include "funcoes.h"

int main()
{
    int n;
    std::string nomeArq;
    
    std::cin >> n >> nomeArq;

    switch(n)
    {
        case 10:
            std::cout << "aaaaaaaaaa" << std::endl;
            CriarGrafo(nomeArq);
            break;

        default:
            std::cout << "Opcao invalida!" << std::endl;
    }

    return 0;
}