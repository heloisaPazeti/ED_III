#include "funcoesAuxiliares.h"
#include "structs.h"

//////////////////////////////////////////////////////// FUNCOES DE LEITURA

CabecalhoArvBin LerCabecalhoArvore(char *arquivo)
{
    CabecalhoArvBin cabecalho = CriarCabecalhoArvBin(); // Inicializa Arvore
    FILE *arqBin = fopen(arquivo, "rb");

    if (ChecarIntegridadeArquivo(arqBin, arquivo) < 0) return cabecalho;
    
    fread(&cabecalho.status, sizeof(char), 1, arqBin);
    fread(&cabecalho.noRaiz, sizeof(int), 1, arqBin);
    fread(&cabecalho.RRNproxNo, sizeof(int), 1, arqBin);
    fread(cabecalho.lixo, sizeof(char), 83, arqBin);

    fclose(arqBin);
    return cabecalho;
}

NoArvBin LerNoArvore(char *arquivo, int rrn)
{
    NoArvBin no = CriarNo();                    // Inicializa no
    FILE *arqBin = fopen(arquivo, "rb");
    int i;

    if (ChecarIntegridadeArquivo(arqBin, arquivo) < 0) return no;
        
    fseek(arqBin, (rrn * tamNo) + tamCabecalhoArvore, SEEK_SET);

    fread(&no.folha, sizeof(char), 1, arqBin);
    fread(&no.nroChavesNo, sizeof(int), 1, arqBin);
    fread(&no.RRNdoNo, sizeof(int), 1, arqBin);

    for (i = 0; i < no.nroChavesNo; i++)        // Le todos os Pi, Ci, PRi
    {
        fread(&no.P[i], sizeof(int), 1, arqBin);
        fread(&no.info[i].C, sizeof(long), 1, arqBin);
        fread(&no.info[i].PR, sizeof(long), 1, arqBin);
    }

    fread(&no.P[i], sizeof(int), 1, arqBin);    // Le o ponteiro final
    fclose(arqBin);
    return no;
}

//////////////////////////////////////////////////////// FUNCOES DE ESCRITA

int EscreveNo(char *nomeArq, NoArvBin no, int rrn)
{
    int i, offset = rrn*tamNo + tamCabecalhoArvore;
    FILE *arqArvBin = fopen(nomeArq, "wb+");
    if(ChecarIntegridadeArquivo(arqArvBin, nomeArq) == -1) return -1;

    fseek(arqArvBin, offset, SEEK_SET);
    fwrite(&no.folha, sizeof(char), 1, arqArvBin);
    fwrite(&no.nroChavesNo, sizeof(int), 1, arqArvBin);
    fwrite(&no.RRNdoNo, sizeof(int), 1, arqArvBin);

    for (i = 0; i < no.nroChavesNo; i++)        
    {
        fwrite(&no.P[i], sizeof(int), 1,  arqArvBin);
        fwrite(&no.info[i].C, sizeof(long), 1,  arqArvBin);
        fwrite(&no.info[i].PR, sizeof(long), 1,  arqArvBin);
    }

    fwrite(&no.P[i], sizeof(int), 1,  arqArvBin);
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


//////////////////////////////////////////////////////// ALTERAÇÕES

int AlterarCabecalho(char *nomeArq, char status, int noRaiz, int rrnProxNo)
{
    FILE *arq = fopen(nomeArq, "wb+");

    if(ChecarIntegridadeArquivo(arq, nomeArq) == -1) return -1;

    fwrite(&status, sizeof(char), 1, arq);
    fwrite(&noRaiz, sizeof(int), 1, arq);
    fwrite(&rrnProxNo, sizeof(int), 1, arq);
    fclose(arq);
    return 0;
}

NoArvBin AlterarNo(NoArvBin no, char folha, int nroChavesNo, int rrnNo)
{
    no.folha = folha;
    no.nroChavesNo = nroChavesNo;
    no.RRNdoNo = rrnNo;

    return no;
}

//////////////////////////////////////////////////////// ORDENAÇÕES

NoArvBin OrdenaNo(NoArvBin noOriginal, int posInsercao, RegistroInfo info)
{
    NoArvBin noFinal = noOriginal;

    noFinal.info = OrdenaInfos(noOriginal.nroChavesNo, noOriginal, posInsercao, info);
    return noFinal;
}

RegistroInfo* OrdenaInfos(int size, NoArvBin no, int posInsercao, RegistroInfo info)
{
    RegistroInfo *infosOrdenadas;
    
    infosOrdenadas =  calloc(size, sizeof(RegistroInfo));
    infosOrdenadas = no.info;

    infosOrdenadas[posInsercao].C = info.C;
    infosOrdenadas[posInsercao].PR = info.PR;

    for(int i = posInsercao+1; i < size; i++)
    {
        infosOrdenadas[i].C = no.info[i-1].C;
        infosOrdenadas[i].PR = no.info[i-1].PR;
    }

    return infosOrdenadas;
}

//////////////////////////////////////////////////////// POSIÇÃO

int EncontraPosicao(NoArvBin no, RegistroInfo info)
{
    for(int i = 0; i < no.nroChavesNo; i++)
        if(no.info[i].C > info.C) return i;
    
    return no.nroChavesNo;
}

