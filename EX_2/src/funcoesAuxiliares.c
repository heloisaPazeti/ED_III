#include "funcoesAuxiliares.h"
#include "structs.h"

//////////////////////////////////////////////////////// FUNCOES DE LEITURA

// Abre o arquivo com o cabecalho e le
CabecalhoArvBin LerCabecalhoArvore(char *arquivo)
{
    CabecalhoArvBin cabecalho = CriarCabecalhoArvBin();                         // Inicializa Arvore
    FILE *arqBin = fopen(arquivo, "rb");                                        // Abre para leitura

    if (ChecarIntegridadeArquivo(arqBin, arquivo) < 0) return cabecalho;        // Checa se esta bem
    
    fseek(arqBin, 0, SEEK_SET);                                                 // Posiciona cursor
    fread(&cabecalho.status, sizeof(char), 1, arqBin);
    fread(&cabecalho.noRaiz, sizeof(int), 1, arqBin);
    fread(&cabecalho.RRNproxNo, sizeof(int), 1, arqBin);
    fread(cabecalho.lixo, sizeof(char), 83, arqBin);

    fclose(arqBin);
    return cabecalho;                                                           // Retorna informacoes
}

// Le um no em um dado rrn
NoArvBin LerNoArvore(char *arquivo, int rrn)
{
    NoArvBin no = CriarNo();                                                    // Inicializa no
    FILE *arqBin = fopen(arquivo, "rb");                                        // Abre para leitura
    int i;

    if (ChecarIntegridadeArquivo(arqBin, arquivo) < 0) return no;
        
    fseek(arqBin, (rrn * tamNo) + tamCabecalhoArvore, SEEK_SET);                // Posiciona cursor

    fread(&no.folha, sizeof(char), 1, arqBin);
    fread(&no.nroChavesNo, sizeof(int), 1, arqBin);
    fread(&no.RRNdoNo, sizeof(int), 1, arqBin);

    for (i = 0; i < tamCPR; i++)                                                // Le todos os Pi, Ci, PRi
    {
        fread(&no.P[i], sizeof(int), 1, arqBin);
        fread(&no.info[i].C, sizeof(long), 1, arqBin);
        fread(&no.info[i].PR, sizeof(long), 1, arqBin);
    }

    fread(&no.P[i], sizeof(int), 1, arqBin);                                    // Le o ponteiro final
    fclose(arqBin);
    return no;                                                                  // Retorna no
}

//////////////////////////////////////////////////////// FUNCOES DE ESCRITA

// Escreve um no no arquivo
int EscreveNo(char *nomeArq, NoArvBin no, int rrn)
{
    FILE *arqArvBin;
    int i;
    int offset = (rrn*tamNo) + tamCabecalhoArvore;                              // Offset passado
    arqArvBin = fopen(nomeArq, "rb+");                                          // Arquivo para leitura e escrita

    if(ChecarIntegridadeArquivo(arqArvBin, nomeArq) == -1) return -1;

    fseek(arqArvBin, offset, SEEK_SET);                                         // Posiciona cursor
    fwrite(&no.folha, sizeof(char), 1, arqArvBin);
    fwrite(&no.nroChavesNo, sizeof(int), 1, arqArvBin);
    fwrite(&no.RRNdoNo, sizeof(int), 1, arqArvBin);

    for (i = 0; i < tamCPR; i++)        
    {
        fwrite(&no.P[i], sizeof(int), 1,  arqArvBin);
        fwrite(&no.info[i].C, sizeof(long), 1,  arqArvBin);
        fwrite(&no.info[i].PR, sizeof(long), 1,  arqArvBin);
    }

    fwrite(&no.P[i], sizeof(int), 1,  arqArvBin);
    fclose(arqArvBin);
    return 0;
}

// Escreve no cabecalho
int AlterarCabecalho(char *nomeArq, char status, int noRaiz, int rrnProxNo)
{
    FILE *arq = fopen(nomeArq, "rb+");
    char c = '$';

    if(ChecarIntegridadeArquivo(arq, nomeArq) == -1) return -1;

    fwrite(&status, sizeof(char), 1, arq);
    fwrite(&noRaiz, sizeof(int), 1, arq);
    fwrite(&rrnProxNo, sizeof(int), 1, arq);

    for(int i = 0; i < 84; i++)
        fwrite(&c, sizeof(char), 1, arq);

    fclose(arq);
    return 0;
}

//////////////////////////////////////////////////////// CHECAGENS

int ChecarCabecalho(CabecalhoArvBin cabecalho)
{
    if (cabecalho.status == '0')
    {
        printf("Cabecalho corrompido.\n");
        return -1;
    }

    return 0;
}

int ChecarArvoreVazia(char *nomeArquivo, int printError)
{
    long int size;
    FILE *arq = fopen(nomeArquivo, "rb");

    if(ChecarIntegridadeArquivo(arq, nomeArquivo) == -1) return -1;

    fseek(arq, 0, SEEK_END);
    size = ftell(arq);
    fclose(arq);

    if(size <= tamCabecalhoArvore || size == 0)
    {
        if(printError == 1) printf("Essa arvore esta vazia.\n");
        return -1;
    }

    return 0;
}

int ChecarIntegridadeArquivo(FILE *arquivo, char *nomeArq)
{
    if(arquivo == NULL)
    {
        printf("Foi impossível de abrir o arquivo: %s\n", nomeArq);
        return -1;
    }

    return 0;
}

//////////////////////////////////////////////////////// ORDENAÇÕES

// Ordena um vetor de informacoes de acordo com a chave
// Ordena um vetor de ponteiros passados por referencia
RegistroInfo* OrdenaInfos(NoArvBin no, int posInsercao, RegistroInfo info, int P, int *pOrdenado)
{
    int i;
    RegistroInfo *infosOrdenadas;    
    infosOrdenadas =  calloc(ordemArvore, sizeof(RegistroInfo));

    for(i = 0; i < ordemArvore; i++)                                // Inicializa como -1
    {
        infosOrdenadas[i].C = -1;
        infosOrdenadas[i].PR = -1;
    }

    pOrdenado[0] = no.P[0];                                         // Seta o primeiro ponteirp

    for(i = 0; i < ordemArvore; i++)                                // Ordena informacoes + ponteiros
    {
        if(i < posInsercao)                                         // Antes da posicao de insercao apenas copia
        {
            infosOrdenadas[i].C = no.info[i].C;
            infosOrdenadas[i].PR = no.info[i].PR;
            pOrdenado[i+1] = no.P[i+1];       
        }
        else if(i == posInsercao)                                   // Na posicao de insercao adiciona novo
        {
            infosOrdenadas[i].C = info.C;
            infosOrdenadas[i].PR = info.PR;
            pOrdenado[i+1] = P;       
        }
        else                                                        // Os seguintes sao deslocados
        {
            infosOrdenadas[i].C = no.info[i-1].C;
            infosOrdenadas[i].PR = no.info[i-1].PR;  
            pOrdenado[i+1] = no.P[i];
        }
    }
    
    return infosOrdenadas;
}

//////////////////////////////////////////////////////// POSIÇÃO

// Encontra a posicao de uma chave num dado no
int EncontraPosicao(NoArvBin no, RegistroInfo info)
{
    for(int i = 0; i < no.nroChavesNo; i++)
        if(no.info[i].C > info.C) return i;
    
    return no.nroChavesNo;
}

