#include "funcoes.h"
#include "structs.h"
#include "funcoesAuxiliares.h"

int CriarGrafo(char* nomeArq)
{
    FILE *arquivo;
    RegDados dado;

    arquivo = fopen(nomeArq, "rb");

    while(dado.removido != '2')
    {
        dado = lerRegistro(arquivo);
    }
    return 0;
}