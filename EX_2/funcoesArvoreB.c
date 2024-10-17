#include "funcoesArvoreB.h"
#include "funcoesFornecidas.h"
#include "funcoesAuxiliares.h"

//////////////////////////////////////////////////////// INICIALIZADORES



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

