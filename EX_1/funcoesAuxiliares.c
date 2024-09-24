#include "funcoesAuxiliares.h"
#include <stdlib.h>

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

// Escreve um registro no arquivo de acordo com a struct passada
int EscreverRegistro(FILE *arqBin, RegDados novoRegisto, int quantReg)
{
    long int posicaoAtual;
    int posicaoFinal;
    char delim = '#';
    int tamNome, tamEspecie, tamHabitat, tamTipo, tamDieta, tamAlimento;
    
    if (arqBin == NULL)
    {
        printf("Falha no processamento do arquivo [Arq. CSV == NULL]\n");
        return -1;
    }

    tamNome = tamanhoString(novoRegisto.nome);
    tamEspecie = tamanhoString(novoRegisto.especie);
    tamHabitat = tamanhoString(novoRegisto.habitat);
    tamTipo = tamanhoString(novoRegisto.tipo);
    tamDieta = tamanhoString(novoRegisto.dieta);
    tamAlimento = tamanhoString(novoRegisto.alimento);

    fwrite(&novoRegisto.removido, sizeof(char),1, arqBin);                          // Escreve os dados
    fwrite(&novoRegisto.encadeamento, sizeof(int),1, arqBin);
    fwrite(&novoRegisto.populacao, sizeof(int),1, arqBin);
    fwrite(&novoRegisto.tamanho, sizeof(float),1, arqBin);
    fwrite(&novoRegisto.unidadeMedida, sizeof(char),1, arqBin);
    fwrite(&novoRegisto.velocidade, sizeof(int),1, arqBin);
    fwrite(novoRegisto.nome, sizeof(char), tamNome, arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.especie, sizeof(char), tamEspecie, arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.habitat, sizeof(char), tamHabitat, arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.tipo, sizeof(char), tamTipo, arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.dieta, sizeof(char), tamDieta, arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.alimento, sizeof(char), tamAlimento, arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);

    posicaoAtual = ftell(arqBin);                                                   // ftell -> posição atual
    posicaoFinal = 1600 + (quantReg*160);                                           // Calcula posição no registro

    for(int i = posicaoAtual; i < posicaoFinal; i++)                                // Adiciona lixo
        fwrite("$", sizeof(char), 1, arqBin);
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
RegDados lerDadosDoTeclado()
{
    RegDados registro;
    char *populacao, *tamanho, *velocidade, *medidaVelocidade;

    registro = IniciarRegistroDados();

    populacao = calloc(10, sizeof(char));
    tamanho = calloc(10, sizeof(char));
    velocidade = calloc(10, sizeof(char));
    medidaVelocidade = calloc(10, sizeof(char));

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

    if(strcmp(populacao, "")==0)
        registro.populacao = -1;
    else registro.populacao = atoi(populacao);

    if(strcmp(velocidade, "")==0)
        registro.velocidade = -1;
    else registro.velocidade = atoi(velocidade);

    if(strcmp(medidaVelocidade, "")==0)
        registro.unidadeMedida = '$';
    else registro.unidadeMedida = medidaVelocidade[0];

    if(strcmp(tamanho, "")==0)
        registro.tamanho = -1;
    else registro.tamanho = atof(tamanho);

    registro.removido = '0';

    return registro;
}

void adicionarRegistro(RegDados registro, FILE *arquivo)
{
    fwrite(registro.nome, 1, sizeof(registro.nome), arquivo);
    if(strncmp(registro.especie,"$",1)!=0&& registro.especie[0] != '\0')        
        printf("Especie: %s\n", registro.especie);
    if(strncmp(registro.tipo,"$",1)!=0&& registro.tipo[0] != '\0')
        printf("Tipo: %s\n", registro.tipo);
    if(strncmp(registro.dieta,"$",1)!=0 && registro.dieta[0] != '\0')
        printf("Dieta: %s\n", registro.dieta);
    if(strncmp(registro.habitat,"$",1)!=0 && registro.habitat[0] != '\0')
        printf("Lugar que habitava: %s\n", registro.habitat);
    if(registro.tamanho != -1)
        printf("Tamanho: %.1f m\n", registro.tamanho);
    if(registro.velocidade != -1)
        printf("Velocidade: %d %cm/h\n", registro.velocidade, registro.unidadeMedida);
    printf("\n");
}

void SubstituirRegistro(FILE *arquivo, RegDados registro)
{
    char delim = '#';
    fwrite(&registro.removido, sizeof(char),1, arquivo); 
    fwrite(&registro.encadeamento, sizeof(int),1, arquivo);
    fwrite(&registro.populacao, sizeof(int),1, arquivo);
    fwrite(&registro.tamanho, sizeof(float),1, arquivo);
    fwrite(&registro.unidadeMedida, sizeof(char),1, arquivo);
    fwrite(&registro.velocidade, sizeof(int),1, arquivo);
    fwrite(registro.nome, sizeof(char), strlen(registro.nome), arquivo);
    fwrite(&delim, sizeof(char),1, arquivo);
    fwrite(registro.especie, sizeof(char), strlen(registro.especie), arquivo);
    fwrite(&delim, sizeof(char),1, arquivo);
    fwrite(registro.habitat, sizeof(char), strlen(registro.habitat), arquivo);
    fwrite(&delim, sizeof(char),1, arquivo);
    fwrite(registro.tipo, sizeof(char), strlen(registro.tipo), arquivo);
    fwrite(&delim, sizeof(char),1, arquivo);
    fwrite(registro.dieta, sizeof(char), strlen(registro.dieta), arquivo);
    fwrite(&delim, sizeof(char),1, arquivo);
    fwrite(registro.alimento, sizeof(char), strlen(registro.alimento), arquivo);
    fwrite(&delim, sizeof(char),1, arquivo);
}

