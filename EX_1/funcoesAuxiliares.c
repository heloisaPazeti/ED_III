#include "funcoesAuxiliares.h"
#include <stdlib.h>

// Calcula o tamanho de uma string
int tamanhoString(char *string)
{
    int i=0;
    while(string[i]!='\0')
        i++;

    return i;
}
///////////////////////////////////////////////////////////////// ESCREVER NO ARQUIVO (1)

int EscreverCabecalho(FILE *arqBin, RegCabecalho cabecalho)
{
    fseek(arqBin, 0, SEEK_SET);                 // Para certificar de que está no local certo

    if (arqBin == NULL)
    {
        printf("Falha no processamento do arquivo [Arq. CSV == NULL]\n");
        return -1;
    }

    fwrite(&cabecalho.status, sizeof(char),1, arqBin);                              // Escreve dados
    fwrite(&cabecalho.topo, sizeof(int),1, arqBin);
    fwrite(&cabecalho.proxRRN, sizeof(int),1, arqBin);
    fwrite(&cabecalho.nroRegRem, sizeof(int),1, arqBin);
    fwrite(&cabecalho.nroPagDisco, sizeof(int),1, arqBin);
    fwrite(&cabecalho.qttCompacta, sizeof(int),1, arqBin);

    for(int i = 21; i < 1600; i++)                                                  // Adiciona "lixo"
        fwrite("$", sizeof(char), 1, arqBin);

    return 0;
}



///////////////////////////////////////////////////////////////// PRINTAR REGISTROS (2)

// Le os dados do cabeçalho de um arquivo e retorna uma struct cabeçalho
RegCabecalho LerCabecalho(FILE *arqBin)
{
    RegCabecalho cabecalho;

    if (arqBin == NULL)
    {
        printf("Falha no processamento do arquivo [Arq. CSV == NULL]\n");
        cabecalho.status = '2';
        return cabecalho;
    }

    fseek(arqBin, 0, SEEK_SET);

    fread(&cabecalho.status, sizeof(char),1,arqBin);
    fread(&cabecalho.topo, sizeof(int),1,arqBin);
    fread(&cabecalho.proxRRN, sizeof(int),1,arqBin);
    fread(&cabecalho.nroRegRem, sizeof(int),1,arqBin);
    fread(&cabecalho.nroPagDisco, sizeof(int),1,arqBin);
    fread(&cabecalho.qttCompacta, sizeof(int),1,arqBin);

    fseek(arqBin, 1600, SEEK_SET);

    return cabecalho;
}

///////////////////////////////////////////////////////////////// ADICIONAR REGISTROS (5)

// Lê dados do teclado e atribui os valroes a uma variável de registro
RegDados lerDadosDoTeclado()
{
    RegDados registro;
    char *populacao, *tamanho, *velocidade, *medidaVelocidade;

    registro = IniciarRegistroDados();              // Inicializa um registro

    // Aloca espaço para as variáveis que precisam ser manipuladas 
    populacao = calloc(10, sizeof(char));
    tamanho = calloc(10, sizeof(char));
    velocidade = calloc(10, sizeof(char));
    medidaVelocidade = calloc(10, sizeof(char));


    // Lê todos os campos do teclado
    scan_quote_string(registro.nome);
    scan_quote_string(registro.dieta);
    scan_quote_string(registro.habitat);
    scan_quote_string(populacao);
    scan_quote_string(registro.tipo);
    scan_quote_string(velocidade);
    scan_quote_string(medidaVelocidade);
    scan_quote_string(tamanho);
    scan_quote_string(registro.especie);
    scan_quote_string(registro.alimento); 

    if(strcmp(populacao, "")==0)                    // Caso o campo seja nulo, atualiza o valor da variável para -1        
        registro.populacao = -1;
    else registro.populacao = atoi(populacao);      // Caso contrário, a variável recebe o valor lido

    if(strcmp(velocidade, "")==0)                   // Caso o campo seja nulo, atualiza o valor da variável para -1
        registro.velocidade = -1;
    else registro.velocidade = atoi(velocidade);    // Caso contrário, a variável recebe o valor lido

    if(strcmp(medidaVelocidade, "")==0)            // Caso o campo seja nulo, atualiza o valor da variável para '$' 
        registro.unidadeMedida = '$';
    else registro.unidadeMedida = medidaVelocidade[0];  // Caso contrário, a variável recebe o valor lido

    if(strcmp(tamanho, "")==0)                      // Caso o campo seja nulo, atualiza o valor da variável para -1
        registro.tamanho = -1;
    else registro.tamanho = atof(tamanho);          // Caso contrário, a variável recebe o valor lido

    registro.removido = '0';                        // Certifica que registro.removido = '0'

    return registro;
}



