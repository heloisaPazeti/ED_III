#include "funcoesInsereArvore.h"

//////////////////////////////////////////////////////// FUNCOES DE INSERCAO

int InserirArvoreVazia(char *nomeArqArvore, char *chave, int pr)
{
    CabecalhoArvBin cabecalho;
    NoArvBin no;
    int resultado;

    no = CriarNo();
    no.nroChavesNo = 1;
    no.info[0].C = converteNome(chave);
    no.info[0].PR = pr;
    no.RRNdoNo = 0;

    if (EscreveNo(nomeArqArvore, no, no.RRNdoNo) == -1) return -1;

    return 0;
}

int InserirNoSemOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info)
{
    NoArvBin no = CriarNo();
    //resultado.no.nroChavesNo++;

    printf("NO SIZE: %d || INFO TO ADD: %ld\n", resultado.no.nroChavesNo, info.C);

    no = OrdenaNo(resultado.no, resultado.posInsercao, info);

    printf("[INSERCAO SEM OVERFLOW] >>>> NÓ ORGANIZADO >>>>>");
    for(int i = 0; i < tamCPR; i++)
        printf("C[%d]: %ld || ", i, no.info[i].C);

    printf("\n\n");

    
    if(EscreveNo(nomeArqArvore, no, no.RRNdoNo) == -1) return -1;
    return 0;
}

int InserirNoComOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info)
{
    CabecalhoArvBin cabecalho = LerCabecalhoArvore(nomeArqArvore);
    RegistroInfo *infosOrdenadas;
    NoArvBin noEsquerdo = CriarNo();
    NoArvBin noDireito = CriarNo();
    int j = 0, posicaoPromovida = 2;                   // (m/2 + 0.5)-1 => Como ordem é impar soma-se 0.5 e retira-se 1 pela contagem começar no zero. 
    int novoProxRRN = cabecalho.RRNproxNo;

    infosOrdenadas = OrdenaInfos(ordemArvore, resultado.no, resultado.posInsercao, info);
    
    /*
    */
    printf("[NOVO INSERE COM OVERFLOW] >>>> NÓ INICAL >>>> ");
    for(int i = 0; i < tamCPR; i++)
        printf("C[%d]: %ld || ", i, resultado.no.info[i].C);
    printf("\n");

    printf("INFO ADICIONADA >>>>>> %ld\n", info.C);


    printf("INFOS ORDENADAS >>>> ");
    for(int i = 0; i < ordemArvore; i++)
        printf("C[%d]: %ld || ", i, infosOrdenadas[i].C);
    printf("\n\n");

    for(int i = 0; i < ordemArvore; i++)
    {
        if(i < posicaoPromovida)
        {
            noEsquerdo.info[i] = infosOrdenadas[i];
            noEsquerdo.P[i] = resultado.no.P[i];
        }
        else if(i > posicaoPromovida)
        {
            noDireito.info[j] = infosOrdenadas[i];
            noDireito.P[j] = resultado.no.P[i];
            j++;
        }
    }

    noEsquerdo = AlterarNo(noEsquerdo, resultado.no.folha, posicaoPromovida, resultado.no.RRNdoNo);
    noDireito = AlterarNo(noDireito, resultado.no.folha, posicaoPromovida, novoProxRRN);
    novoProxRRN++;

    int novaPosicao = EncontraPosicao(resultado.noAnt, resultado.no.info[posicaoPromovida]);
    if(resultado.noAnt.RRNdoNo <= -1)                           // Overflow na Raiz
    {
        NoArvBin novaRaiz = CriarNo();

        novaRaiz = AlterarNo(novaRaiz, 0, 1, cabecalho.RRNproxNo);
        novaRaiz.P[0] = noEsquerdo.RRNdoNo;
        novaRaiz.P[1] = noDireito.RRNdoNo;

        cabecalho.noRaiz = novaRaiz.RRNdoNo;
        novoProxRRN = ++novaRaiz.RRNdoNo;

        novaPosicao = 0;
        resultado.noAnt = novaRaiz;
    }
    else
    {
        NoArvBin noTemp = CriarNo();
        int i;
        resultado.noAnt.nroChavesNo++;

        for(i = 0; i < ordemArvore; i++)
            noTemp.P[i] = resultado.noAnt.P[i];
        
        for(i = novaPosicao+2; i < ordemArvore; i++)
            resultado.noAnt.P[i] = noTemp.P[i-1]; 
        
        resultado.noAnt.P[novaPosicao + 1] = noDireito.RRNdoNo;
        /*
        for(i = 0; i < ordemArvore; i++)
            printf("EM NO TEMP P[%d] >> RRN: %d|| ", i, noTemp.P[i]);
        printf("\n\n");


        for(i = 0; i < ordemArvore; i++)
            printf("EM P[%d] >> RRN: %d|| ", i, resultado.noAnt.P[i]);
        printf("\n\n");
        */

       /////////////////
       //FREE(NOTEMP);|| => Botar aqui por favor
       /////////////////

    }
    
    AlterarCabecalho(nomeArqArvore, '1', cabecalho.noRaiz, novoProxRRN);
    PromoveChave(nomeArqArvore, novaPosicao, infosOrdenadas[posicaoPromovida], resultado.noAnt);

    EscreveNo(nomeArqArvore, noEsquerdo, noEsquerdo.RRNdoNo);
    EscreveNo(nomeArqArvore, noDireito, noDireito.RRNdoNo);
}

int PromoveChave(char *nomeArqArvore, int posInsercao, RegistroInfo promovido, NoArvBin noPromocao)
{
    //int posInsercao = EncontraPosicao(noPromocao, promovido);
    NoPos resultado = BuscarNoArvore(nomeArqArvore, noPromocao.info[0].C);
    resultado.no = noPromocao;
    resultado.pos= -1;
    resultado.posInsercao = posInsercao;
    //resultado.no.nroChavesNo++;

    if(noPromocao.nroChavesNo > tamCPR)                                    // Ocorre overflow na promocao
    {
        if(resultado.noAnt.RRNdoNo <= -1)
        {
            printf("Anterior inexistente.\n");
            return -1;
        }

        InserirNoComOverflow(nomeArqArvore, resultado, promovido);
    }
    else                                                                    // Não ocorre overflow
        InserirNoSemOverflow(nomeArqArvore, resultado, promovido);

    return 0;
}
