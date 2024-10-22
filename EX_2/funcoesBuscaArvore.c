#include "funcoesBuscaArvore.h"

//////////////////////////////////////////////////////// FUNCOES DE BUSCA

// Retorna uma struct com no + pos na arvore na qual está / deveria estar o registro
// Se houver retorna o no + rrn
// Se não houver retorna na posicao -1
// Retorna -2 na posicao se der algum erro
NoPos BuscarNoArvore(char *arquivo, long int chave)
{
    CabecalhoArvBin cabecalho = LerCabecalhoArvore(arquivo);
    NoArvBin no = CriarNo();
    NoPos noPos;
    int i, nextRrr, encontrou;

    noPos.posInsercao = -2;
    noPos.pos = -2;

    if (ChecarArvoreVazia(arquivo, 0) < 0) return noPos;

    nextRrr = cabecalho.noRaiz;                     // Pega o rrn da raiz

    while (noPos.pos == -2)                         // Enquanto não encontrar o fim ou a chave
    {
        noPos.noAnt = no;
        no = LerNoArvore(arquivo, nextRrr);         // Le o no
        noPos.no = no;
        
        if(no.nroChavesNo <= 0)
        {
            noPos.pos = 0;
            noPos.posInsercao = 0;
            return noPos;
        }

        for (i = 0; i < no.nroChavesNo; i++)    // Checa todas as chaves no no
        {
            if (no.info[i].C == chave)     // Se encontrou
            {
                //noPos.no = no;                     // Seleciona o no
                noPos.pos = i;                      // Seleciona a pos
                break;
            }
            else if (no.info[i].C > chave) // Se a chave atual for maior que a chave procurada
            {
                nextRrr = no.P[i];                  // "Desce" para o no Pi
                noPos.posInsercao = i;
                break;
            }
            else if (i == (no.nroChavesNo - 1))     // Se chegar ao final da arvore
            {
                nextRrr = no.P[i+1];                // "Desce" para o no Pi+1
                noPos.posInsercao = i+1;
                break;
            }
        }

        if (nextRrr == -1)                          // Caso nao tenha encontrado
        {
            noPos.pos = -1;
            noPos.posInsercao = i;
        }
    }

    /*
    printf(">>>>>>>>> CHAVE PROCURADA: %ld || QUANTIDADE DE NÓS: %d\n\n", chave, noPos.no.nroChavesNo);

    for(int i = 0; i < noPos.no.nroChavesNo; i++)
        printf("CHAVE NO NÓ[%d]: %ld || ", i, noPos.no.info[i].C);

    printf("\n\n");

    for(i = 0; i < noPos.noAnt.nroChavesNo; i++)
        printf("CHAVE ANTERIOR[%d]: %ld || ", i, noPos.noAnt.info[i].C);

    printf("\n\n");
    */

    return noPos;
}