#include "funcoesInsereArvore.h"

//////////////////////////////////////////////////////// FUNCOES DE INSERCAO

// Insere o primeiro no em uma arvore vazia
void InserirArvoreVazia(char *nomeArqArvore, RegistroInfo info)
{
    CabecalhoArvBin cabecalho;
    NoArvBin no;
    int resultado;

    no = CriarNo();                                 // Inicia um no
    no.nroChavesNo = 1;                             // Adiciona uma chave
    no.info[0].C = info.C;                          // Adiciona a info
    no.info[0].PR = info.PR;                        // Adiciona o offset
    no.RRNdoNo = 0;                                 // Coloca rrn do no

    AlterarCabecalho(nomeArqArvore, '1', 0, 1);     // Salva no cabecalho
    if(EscreveNo(nomeArqArvore, no, 0) == -1)       // Escreve no arquivo
        printf("ERRO: INSERIR VAZIA -> ESCREVE NO\n");

}

// Insere em um que tem espaco ainda
int InserirNoSemOverflow(char *nomeArqArvore, NoArvBin no, int P, int posInsercao, RegistroInfo info)
{
    for(int i = no.nroChavesNo; i > posInsercao; i--)                   // Ordena as informacoes e ponteiros que foram deslocados
    {
        no.info[i] = no.info[i-1];
        no.P[i+1] = no.P[i];
    }

    no.info[posInsercao] = info;                                        // Adiciona a nova informacao
    no.P[posInsercao + 1] = P;                                          // Adiciona o novo ponteiro
    no.nroChavesNo++;                                                   // Aumenta o numero de nos

    if(EscreveNo(nomeArqArvore, no, no.RRNdoNo) == -1)                  // Escreve o no
        printf("ERRO: INSERIR SEM OVERFLOW -> ESCREVE NO\n");

    return 0;
}

// Caso nao tenha espaco no no para inserir
int InserirNoComOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info, int noDireita) {

    CabecalhoArvBin cabecalho = CriarCabecalhoArvBin(); 
    NoArvBin noEsquerdo = CriarNo();                                       // Cria-se o no que ficara a esquerda
    NoArvBin noDireito = CriarNo();                                        // Cria-se o no que ficara a direita
    RegistroInfo infoPromovida;                                            // Informacao do no original que sera promovida

    RegistroInfo infosOrdenadas[5];
    int pOrdenados[6];
    int i;

    cabecalho = LerCabecalhoArvore(nomeArqArvore);
    pOrdenados[0] = resultado.no.P[0];                                      // O primeiro ponteiro nunca eh alterado
    
    for(i = 0; i < resultado.posInsercao; i++)                              // Copia-se as infos anteriores a insercao
    {
        infosOrdenadas[i] = resultado.no.info[i];
        pOrdenados[i+1] = resultado.no.P[i+1];
    }

    infosOrdenadas[resultado.posInsercao] = info;                           // Adiciona-se a nova informacao
    pOrdenados[resultado.posInsercao+1] = noDireita;                        // Adiciona-se a referencia ao novo no

    for(i = 4; i > resultado.posInsercao; i--){                             // Adiciona-se as informacoes deslocadas
        infosOrdenadas[i] = resultado.no.info[i-1];
        pOrdenados[i+1] = resultado.no.P[i];
    }


    ///////////////////////////////////////// ADICIONANDO NOS A ESQUERDA E DIREITA
    // Passamos agora com as infos ordenadas metade para o no esquerdo
    // Metade para o direto
    // Assim como os ponteiros

    noEsquerdo.info[0] = infosOrdenadas[0];
    noEsquerdo.info[1] = infosOrdenadas[1];
    noEsquerdo.P[0] = pOrdenados[0];
    noEsquerdo.P[1] = pOrdenados[1];
    noEsquerdo.P[2] = pOrdenados[2];
    noEsquerdo.nroChavesNo = 2;
    noEsquerdo.folha = resultado.no.folha;
    noEsquerdo.RRNdoNo = resultado.no.RRNdoNo;

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


    ///////////////////////////////////////// PROMOCAO

    infoPromovida = infosOrdenadas[2];

    if(resultado.noAnt.RRNdoNo == -1)                                               // Caso o overflow seja na raiz
    {
        NoArvBin noRaiz = CriarNo();                                                // Cria-se uma nova raiz

        noRaiz.RRNdoNo = cabecalho.RRNproxNo++;                                     // Adicionamos seu rrn
        noRaiz.folha = '0';                                                         // Nunca sera uma folha
        noRaiz.nroChavesNo = 1;                                                     // Adicionamos uma chave
        noRaiz.info[0] = infoPromovida;                                             // Colocamos a informacao promovida
        noRaiz.P[0] = noEsquerdo.RRNdoNo;                                           // Menor a esquerda
        noRaiz.P[1] = noDireito.RRNdoNo;                                            // Maior a direita
        EscreveNo(nomeArqArvore, noRaiz, noRaiz.RRNdoNo);                           // Escrevemos o no
        AlterarCabecalho(nomeArqArvore, '1', noRaiz.RRNdoNo, cabecalho.RRNproxNo);  // Alteramos o cabecalho
        return 0;
    }
    else if(resultado.noAnt.nroChavesNo < tamCPR)                                   // No anterior sem overflow
    {
        int posicao = EncontraPosicao(resultado.noAnt, infoPromovida);              // Encontra posicao, insere sem overflow
        InserirNoSemOverflow(nomeArqArvore, resultado.noAnt, noDireito.RRNdoNo, posicao, infoPromovida);
        AlterarCabecalho(nomeArqArvore, '1', cabecalho.noRaiz, cabecalho.RRNproxNo);
    }
    else                                                                            // No anterior com overflow
    {
        resultado = BuscarNoArvore(nomeArqArvore, resultado.noAnt.info[0].C);       // Refaz a busca para receber no anterior do anterior
        resultado.posInsercao = EncontraPosicao(resultado.no, infoPromovida);       // Reencontra a posicao de insercao
        resultado.pos = -1;                                                         // Nao foi encontrado, refaz-se a recursao
        AlterarCabecalho(nomeArqArvore, '1', cabecalho.noRaiz, cabecalho.RRNproxNo);
        InserirNoComOverflow(nomeArqArvore, resultado, infoPromovida, noDireito.RRNdoNo);
    }

    return 0;
}