#include "funcoesArvoreB.h"
#include "funcoesFornecidas.h"
#include "funcoesBuscaManipulacao.h"

//////////////////////////////////////////////////////// INICIALIZADORES

CabecalhoArvBin CriarCabecalhoArvBin()
{
    CabecalhoArvBin newCabecalho;

    newCabecalho.status = '0';
    newCabecalho.noRaiz = -1;
    newCabecalho.RRNproxNo = 0;

    for (int i = 0; i < 83; i++)
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

    // for (i = 0; i < tamP; i++)
    //     newNo.P[i] = -2;

    // for (i = 0; i < tamCPR; i++)
    // {
    //     newNo.C[i] = -2;
    //     newNo.PR[i] = -2;
    // }

    newNo.P = calloc(tamP, sizeof(int));
    newNo.C = calloc(tamCPR, 8);
    newNo.PR = calloc(tamCPR, 8);

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
    NoPos result;
    FILE *arq;
    RegDados reg;


    campo = calloc(10, sizeof(char));
    chave = calloc(160, sizeof(char));

    scanf("%s", campo);
    scan_quote_string(chave);
    
    result = BuscarNoArvore(nomeArqArvore, chave);

    if (result.pos == -2 || result.pos == -1)
        return -1;

    arq = fopen(nomeArq, "rb");
    if (arq == NULL)
        return -1;

    fseek(arq, result.no.PR[result.pos], SEEK_SET);
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
NoPos BuscarNoArvore(char *arquivo, char *chave)
{
    CabecalhoArvBin cabecalho = LerCabecalhoArvore(arquivo);
    NoArvBin no;
    NoPos noPos;
    int i, nextRrr;

    noPos.pos = -2;

    if (cabecalho.status == '0')
    {
        printf("Cabecalho corrompido.\n");
        return noPos;
    }

    if (cabecalho.noRaiz == -1)
    {
        printf("Essa arvore esta vazia.\n");
        return noPos;
    }

    nextRrr = cabecalho.noRaiz;

    while (noPos.pos < -1)
    {
        no = LerNoArvore(arquivo, nextRrr);
        for (int i = 0; i < no.nroChavesNo; i++)
        {
            if (no.C[i] == converteNome(chave))
            {
                noPos.no = no;
                noPos.pos = i;
                break;
            }
            else if (no.C[i] > converteNome(chave))
            {
                nextRrr = no.P[i];
                break;
            }
            else if (i == (no.nroChavesNo - 1))
                nextRrr = no.P[i+1];
        }

        if (nextRrr == -1)
            noPos.pos = -1;
    }

    if (noPos.pos == -1)
        printf("Registro inexistente.\n");

    return noPos;
}

CabecalhoArvBin LerCabecalhoArvore(char *arquivo)
{
    CabecalhoArvBin cabecalho = CriarCabecalhoArvBin();
    FILE *arqBin = fopen(arquivo, "rb");

    if (arqBin == NULL)
    {
        printf("Arquivo nulo.\n");
        return cabecalho;
    }

    fread(&cabecalho.status, sizeof(char), 1, arqBin);
    fread(&cabecalho.noRaiz, sizeof(int), 1, arqBin);
    fread(&cabecalho.RRNproxNo, sizeof(int), 1, arqBin);
    fread(cabecalho.lixo, sizeof(char), 83, arqBin);

    fclose(arqBin);
    return cabecalho;
}

NoArvBin LerNoArvore(char *arquivo, int rrn)
{
    NoArvBin no = CriarNo();
    FILE *arqBin = fopen(arquivo, "rb");
    int i;

    if (arqBin == NULL)
        return no;
        
    fseek(arqBin, (rrn * tamNo) + tamCabecalho, SEEK_SET);

    fread(&no.folha, sizeof(char), 1, arqBin);
    fread(&no.nroChavesNo, sizeof(int), 1, arqBin);
    fread(&no.RRNdoNo, sizeof(int), 1, arqBin);

    for (i = 0; i < no.nroChavesNo; i++)
    {
        fread(&no.P[i], sizeof(int), 1, arqBin);
        fread(&no.C[i], sizeof(long), 1, arqBin);
        fread(&no.PR[i], sizeof(long), 1, arqBin);
    }

    fread(&no.P[i], sizeof(int), 1, arqBin);
    fclose(arqBin);
    return no;
}