#include "funcoes.h"

int EscreverArquivo()
{
    char arqCSV[15];
    char arqBin[15];
    RegCabecalho newCabecalho = IniciarCabecalho();
    RegDados newDado = IniciarRegistroDados();

    scanf("%s", arqCSV);
    scanf("%s", arqBin);

    while(newDado.população != -1)
    {
        newDado = LerCSV(arqCSV);
        //Escrever no arquivo
    }
   
}