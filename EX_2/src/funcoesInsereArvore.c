#include "funcoesInsereArvore.h"

//////////////////////////////////////////////////////// FUNCOES DE INSERCAO

void InserirArvoreVazia(char *nomeArqArvore, RegistroInfo info)
{
    CabecalhoArvBin cabecalho;
    NoArvBin no;
    int resultado;

    no = CriarNo();
    no.nroChavesNo = 1;
    no.info[0].C = info.C;
    no.info[0].PR = info.PR;
    no.RRNdoNo = 0;

    AlterarCabecalho(nomeArqArvore, '1', 0, 1);
    EscreveNo(nomeArqArvore, no, 0);
}

void InserirNoSemOverflow(char *nomeArqArvore, NoArvBin no, int P, int posInsercao, RegistroInfo info)
{
    //NoArvBin no = CriarNo();
    //no.nroChavesNo++;

    //int aux[6];
    //no.info = OrdenaInfos(no, posInsercao, info, P, aux);

    for(int i = no.nroChavesNo; i > posInsercao; i--){
        no.info[i] = no.info[i-1];
        no.P[i+1] = no.P[i];
    }

    no.info[posInsercao] = info;
    no.P[posInsercao + 1] = P;
    no.nroChavesNo++;

    if(EscreveNo(nomeArqArvore, no, no.RRNdoNo) == -1)
        printf("ERRO: INSERIR SEM OVERFLOW -> ESCREVE NO\n");
}

void InserirNoComOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info, int noDireita) {

    CabecalhoArvBin cabecalho = LerCabecalhoArvore(nomeArqArvore); 
    NoArvBin noEsquerdo = CriarNo();
    NoArvBin noDireito = CriarNo();
    RegistroInfo infoPromovida;

    RegistroInfo infosOrdenadas[5];
    int pOrdenados[6];
    int i;

    pOrdenados[0] = resultado.no.P[0];
    
    for(i = 0; i < resultado.posInsercao; i++)
    {
        infosOrdenadas[i] = resultado.no.info[i];
        pOrdenados[i+1] = resultado.no.P[i+1];
    }

    infosOrdenadas[resultado.posInsercao] = info;
    pOrdenados[resultado.posInsercao+1] = noDireita;

    for(i = 4; i > resultado.posInsercao; i--){
        infosOrdenadas[i] = resultado.no.info[i-1];
        pOrdenados[i+1] = resultado.no.P[i];
    }

    // int pOrdenado[6] = {-1,-1,-1,-1,-1,-1};
    // for(int i=0; i<5; i++)
    //     pOrdenado[i] = resultado.no.P[i];
    // RegistroInfo *infosOrdenadas = OrdenaInfos(resultado.no, resultado.posInsercao, info, noDireita, pOrdenado);



    ///////////////////////////////////////// ADICIONANDO NOS A ESQUERDA E DIREITA

    noEsquerdo.info[0] = infosOrdenadas[0];
    noEsquerdo.info[1] = infosOrdenadas[1];
    noEsquerdo.P[0] = pOrdenados[0];
    noEsquerdo.P[1] = pOrdenados[1];
    noEsquerdo.P[2] = pOrdenados[2];
    noEsquerdo.nroChavesNo = 2;
    noEsquerdo.folha = resultado.no.folha;

    noDireito.info[0] = infosOrdenadas[3];
    noDireito.info[1] = infosOrdenadas[4];
    noDireito.P[0] = pOrdenados[3];
    noDireito.P[1] = pOrdenados[4];
    noDireito.P[2] = pOrdenados[5];
    noDireito.nroChavesNo = 2;
    noDireito.folha = resultado.no.folha;
    noDireito.RRNdoNo = cabecalho.RRNproxNo++;

    EscreveNo(nomeArqArvore, noEsquerdo, noEsquerdo.RRNdoNo);
    EscreveNo(nomeArqArvore, noDireito, noDireito.RRNdoNo);

    // PARTE DOS P

    ///////////////////////////////////////// PROMOCAO

    infoPromovida = infosOrdenadas[2];

    if(resultado.noAnt.RRNdoNo == -1)       // SEM NO ANTERIOR
    {
        NoArvBin noRaiz = CriarNo();

        noRaiz.RRNdoNo = cabecalho.RRNproxNo++;
        noRaiz.folha = '0';
        noRaiz.info[0] = infoPromovida;
        noRaiz.P[0] = noEsquerdo.RRNdoNo;
        noRaiz.P[1] = noDireito.RRNdoNo;
    }
    else if(resultado.noAnt.nroChavesNo < tamCPR)
    {
        int posicao = EncontraPosicao(resultado.noAnt, infoPromovida);
        InserirNoSemOverflow(nomeArqArvore, resultado.noAnt, noDireito.RRNdoNo, posicao, infoPromovida);
    }
    else 
    {
        resultado = BuscarNoArvore(nomeArqArvore, resultado.noAnt.info[0].C);
        resultado.posInsercao = EncontraPosicao(resultado.no, infoPromovida);
        resultado.pos = -1;
        InserirNoComOverflow(nomeArqArvore, resultado, infoPromovida, noDireito.RRNdoNo);
    }
}