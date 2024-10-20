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
    NoArvBin no;

    for(int i = 0; i < resultado.no.nroChavesNo; i++)
        printf("CHAVES: %ld\n", resultado.no.info[i].C);

    no = OrdenaNo(resultado.no, resultado.posInsercao, info);
    
    for(int i = 0; i < no.nroChavesNo; i++)
        printf("CHAVE: %ld\n", no.info[i].C);

    //if(EscreveNo(nomeArqArvore, no, no.RRNdoNo) == -1) return -1;

    return 0;
}

int InserirNoComOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info)
{
    RegistroInfo *infosOrdenadas;
    NoArvBin noEsquerdo = CriarNo();
    NoArvBin noDireito = CriarNo();
    int posicaoPromovida = 2;                   // (m/2 + 0.5)-1 => Como ordem é impar soma-se 0.5 e retira-se 1 pela contagem começar no zero. 

    infosOrdenadas = OrdenaInfos(ordemArvore, resultado.no, resultado.posInsercao, info);

    for(int i = 0; i < ordemArvore; i++)
    {
        if(i < posicaoPromovida)
        {
            noEsquerdo.info[i] = infosOrdenadas[i];
            noEsquerdo.P[i] = resultado.no.P[i];
        }
        else if(i > posicaoPromovida)
        {
            noDireito.info[i-posicaoPromovida-1] = infosOrdenadas[i];
            noDireito.P[i-posicaoPromovida-1] = resultado.no.P[i];
        }
    }

    noEsquerdo = AlterarNo(noEsquerdo, resultado.no.folha, posicaoPromovida, resultado.no.RRNdoNo);
    noDireito = AlterarNo(noDireito, resultado.no.folha, posicaoPromovida, LerCabecalhoArvore(nomeArqArvore).RRNproxNo);

    if(resultado.noAnt.RRNdoNo <= -1)                           // Overflow na Raiz
    {
        NoArvBin novaRaiz = CriarNo();

        novaRaiz = AlterarNo(novaRaiz, 0, 1, LerCabecalhoArvore(nomeArqArvore).RRNproxNo);
        novaRaiz.P[0] = noEsquerdo.RRNdoNo;
        novaRaiz.P[1] = noDireito.RRNdoNo;
        AlterarCabecalho(nomeArqArvore, 1, novaRaiz.RRNdoNo, novaRaiz.RRNdoNo++);

        resultado.noAnt = novaRaiz;
    }
    else
    {
        int novaPosicao = EncontraPosicao(resultado.noAnt, resultado.no.info[posicaoPromovida]);
        resultado.noAnt.nroChavesNo++;

        for(int i = novaPosicao+2; i < resultado.noAnt.nroChavesNo; i++)
            resultado.noAnt.P[i] = resultado.noAnt.P[i-1]; 

        //resultado.noAnt.P[novaPosicao] = noEsquerdo.RRNdoNo;
        resultado.noAnt.P[novaPosicao++] = noDireito.RRNdoNo;
    }
    
    PromoveChave(nomeArqArvore, resultado.no.info[posicaoPromovida], resultado.noAnt);


    EscreveNo(nomeArqArvore, noEsquerdo, noEsquerdo.RRNdoNo);
    EscreveNo(nomeArqArvore, noDireito, noDireito.RRNdoNo);

}

int PromoveChave(char *nomeArqArvore, RegistroInfo promovido, NoArvBin noPromocao)
{
    int posInsercao = EncontraPosicao(noPromocao, promovido);
    NoPos resultado = BuscarNoArvore(nomeArqArvore, noPromocao.info[0].C);
    resultado.pos= -1;
    resultado.posInsercao = posInsercao;
    if(noPromocao.nroChavesNo >= tamCPR)                                    // Ocorre overflow na promocao
    {
        if(resultado.noAnt.RRNdoNo <= -1)
        {
            printf("Anterior inexistente.\n");
            return -1;
        }

        InserirNoComOverflow(nomeArqArvore, resultado, promovido);
    }
    else                                                                    // Não ocorre overflow
    {
        InserirNoSemOverflow(nomeArqArvore, resultado, promovido);
        //NoArvBin noOrdenado = OrdenaNo(noPromocao, posInsercao, promovido);
        //if (EscreveNo(nomeArqArvore, noOrdenado, noOrdenado.RRNdoNo) == -1) return -1;
    }

    return 0;
}
