#include "funcoesAuxiliares.h"

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

    if (arqBin == NULL)
    {
        printf("Falha no processamento do arquivo [Arq. CSV == NULL]\n");
        return -1;
    }

    fwrite(&novoRegisto.removido, sizeof(char),1, arqBin);                          // Escreve os dados
    fwrite(&novoRegisto.encadeamento, sizeof(int),1, arqBin);
    fwrite(&novoRegisto.populacao, sizeof(int),1, arqBin);
    fwrite(&novoRegisto.tamanho, sizeof(float),1, arqBin);
    fwrite(&novoRegisto.unidadeMedida, sizeof(char),1, arqBin);
    fwrite(&novoRegisto.velocidade, sizeof(int),1, arqBin);
    fwrite(novoRegisto.nome, sizeof(char), strlen(novoRegisto.nome), arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.especie, sizeof(char), strlen(novoRegisto.especie), arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.habitat, sizeof(char), strlen(novoRegisto.habitat), arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.tipo, sizeof(char), strlen(novoRegisto.tipo), arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.dieta, sizeof(char), strlen(novoRegisto.dieta), arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.alimento, sizeof(char), strlen(novoRegisto.alimento), arqBin);
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
    char populacao[4], tamanho[4], velocidade[4];

    scan_quote_string(registro.nome);
    scan_quote_string(registro.dieta);
    scan_quote_string(registro.habitat);
    scanf("%d", &registro.populacao);
    // scanf("%s", populacao);
    // if(strcmp(populacao, "NULO")!=0)
    // {
    //     registro.populacao = atoi(populacao);
    // }
    // else registro.populacao = 0;
    scan_quote_string(registro.tipo);
    scanf("%d", &registro.velocidade);
    // scanf("%s", velocidade);
    // if(strcmp(velocidade, "NULO")!=0)
    // {
    //     registro.velocidade = atoi(velocidade);
    // }
    // else registro.velocidade = 0;
    scanf("%s", &registro.unidadeMedida);
    scanf("%f", &registro.tamanho);
    // scanf("%s", tamanho);
    // if(strcmp(tamanho, "NULO")!=0)
    // {
    //     registro.populacao = atof(tamanho);
    // }
    // else registro.tamanho = 0;
    scan_quote_string(registro.especie);
    scan_quote_string(registro.alimento);
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

