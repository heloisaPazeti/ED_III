#include "funcoesArvoreB.h"
#include "funcoesFornecidas.h"
#include "funcoesBuscaManipulacao.h"

//////////////////////////////////////////////////////// INICIALIZADORES

CabecalhoArvBin CriarCabecalhoArvBin()
{
    CabecalhoArvBin newCabecalho;

    newCabecalho.status = "0";
    newCabecalho.noRaiz = -1;
    newCabecalho.RRNproxNo = 0;

    for(int i = 0; i < 83; i++)
        newCabecalho.lixo[i] = '$';

    return newCabecalho;
}

NoArvBin CriarNo()
{
    NoArvBin newNo;
    int i;

    newNo.folha = '1';
    newNo.nroChavesNo = 0;
    newNo.RRNdoNo = -1;
    
    for(i = 0; i < tamP; i++)
        newNo.P[i] = -1;

    for(i = 0; i < tamCPR; i++)
    {
        newNo.C[i] = -1;
        newNo.PR[i] = -1;
    }

    return newNo;   
}

///////////////////////////////////////////////////////////////// CRIAR ARVORE (7)

int CriarArvore(char *nomeArq, char *nomeArqArvore)
{

}

///////////////////////////////////////////////////////////////// BUSCAR REGISTRO (8)

int BuscarRegistroArvore(char *nomeArq, char *nomeArqArvore)
{
    char *campo, *chave;
    int index;
    NoArvBin no;
    FILE *arq;
    RegDados reg;

    scanf("%s", campo);
    scan_quote_string(chave);

    no = BuscarNoArvore(nomeArqArvore, chave, &index);

    if(index == -2) return -1;

    fopen(nomeArq, arq);
    if(arq == NULL) return -1;

    fseek(arq, no.PR[index]*tamRegistro, SEEK_SET);
    reg = lerRegistro(arq);
    imprimirRegistro(reg);

    fclose(arq);
    return 0;
}

///////////////////////////////////////////////////////////////// ADICIONAR REGISTRO (9)

int AdicionarRegistroARvore(char *nomeArq, char *nomeArqArvore)
{
    // Lembrar de chamar a função 5
}

///////////////////////////////////////////////////////////////// OUTROS 

// Retorna posicao na arvore na qual está / deveria estar o registro
// Se houver retorna o rrn
// Se não houver retorna -1
NoArvBin BuscarNoArvore(char *arquivo, char *nomeRegistro, int *index)
{
    CabecalhoArvBin cabecalho = LerCabecalhoArvore(arquivo);
    NoArvBin no;
    int nextRrr;

    *index = -2;
    if(cabecalho.status == '0')
    {
        printf("Cabecalho corrompido.\n");
        return no;
    }

    if(cabecalho.noRaiz == -1)
    {
        printf("Essa arvore esta vazia.\n");
        return no;
    }

    nextRrr = cabecalho.noRaiz;

    while(*index < -1)
    {
        no = LerNoArvore(arquivo, nextRrr);
        for(int i = 0; i < tamCPR; i++)
        {
            if(no.C[i] == converteNome(nomeRegistro))
            {
                *index = i;
                break;
            }
            else if(no.C[i] > converteNome(nomeRegistro))
            {
                nextRrr = no.P[i];
                break;
            }
            else if(i == tamCPR-1)
                nextRrr = no.P[i+1];
        }

        if(nextRrr == -1)
            *index = -1;
    }

    if(*index == -1)
        printf("Registro inexistente.\n");

    return no;
}

CabecalhoArvBin LerCabecalhoArvore(char *arquivo)
{
    CabecalhoArvBin cabecalho = CriarCabecalhoArvBin();
    FILE *arqBin = fopen(arquivo, "rb");

    if(arqBin == NULL) return cabecalho;

    fread(&cabecalho.status, sizeof(char), 1, arqBin);
    fread(&cabecalho.noRaiz, sizeof(int), 1, arqBin);
    fread(&cabecalho.RRNproxNo, sizeof(int), 1, arqBin);
    fread(cabecalho.lixo, sizeof(char), 83, arquivo);

    fclose(arqBin);
    return cabecalho;
}

NoArvBin LerNoArvore(char *arquivo, int rrn)
{
    NoArvBin no = CriarNo();
    FILE *arqBin = fopen(arquivo, "rb");

    if(arqBin == NULL) return no;
    fseek(arqBin, rrn*tamNo, SEEK_SET);

    fread(&no.folha, sizeof(char), 1, arqBin);
    fread(&no.nroChavesNo, sizeof(int), 1, arqBin);
    fread(&no.RRNdoNo, sizeof(int), 1, arqBin);

    for(int i = 0; i < tamCPR; i++)
    {
        fread(no.P, sizeof(int), 1, arqBin);
        fread(no.C, sizeof(int), 1, arqBin);
        fread(no.PR, sizeof(int), 1, arqBin);    
    }

    fread(no.P, sizeof(int), 1, arqBin);
}