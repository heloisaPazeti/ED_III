#include "funcoesAuxiliares.h"
#include "structs.h"

//////////////////////////////////////////////////////// FUNCOES DE LEITURA

CabecalhoArvBin LerCabecalhoArvore(char *arquivo)
{
    CabecalhoArvBin cabecalho = CriarCabecalhoArvBin(); // Inicializa Arvore
    FILE *arqBin = fopen(arquivo, "rb");

    if (ChecarIntegridadeArquivo(arqBin, arquivo) < 0) return cabecalho;
    
    fseek(arqBin, 0, SEEK_SET);
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

    for (i = 0; i < tamCPR; i++)        // Le todos os Pi, Ci, PRi
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
    int i;
    int offset = (rrn*tamNo) + tamCabecalhoArvore;
    FILE *arqArvBin;
    arqArvBin = fopen(nomeArq, "rb+");
    if(ChecarIntegridadeArquivo(arqArvBin, nomeArq) == -1) return -1;

    fseek(arqArvBin, offset, SEEK_SET);
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

NoArvBin AlterarNo(NoArvBin no, char folha, int nroChavesNo, int rrnNo)
{
    no.folha = folha;
    no.nroChavesNo = nroChavesNo;
    no.RRNdoNo = rrnNo;

    return no;
}

//////////////////////////////////////////////////////// ORDENAÇÕES

RegistroInfo* OrdenaInfos(NoArvBin no, int posInsercao, RegistroInfo info, int P, int *pOrdenado)
{
    int i;
    RegistroInfo *infosOrdenadas;    
    infosOrdenadas =  calloc(ordemArvore, sizeof(RegistroInfo));

    for(i = 0; i < ordemArvore; i++)
    {
        infosOrdenadas[i].C = -1;
        infosOrdenadas[i].PR = -1;
    }

    pOrdenado[0] = no.P[0];

    for(i = 0; i < ordemArvore; i++)
    {
        if(i < posInsercao)
        {
            infosOrdenadas[i].C = no.info[i].C;
            infosOrdenadas[i].PR = no.info[i].PR;
            pOrdenado[i+1] = no.P[i+1];       
        }
        else if(i == posInsercao)
        {
            infosOrdenadas[i].C = info.C;
            infosOrdenadas[i].PR = info.PR;
            pOrdenado[i+1] = P;       
        }
        else
        {
            infosOrdenadas[i].C = no.info[i-1].C;
            infosOrdenadas[i].PR = no.info[i-1].PR;  
            pOrdenado[i+1] = no.P[i];
        }
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

void LiberaNo(NoArvBin no)
{
    free(no.info);
    free(no.P);
}

