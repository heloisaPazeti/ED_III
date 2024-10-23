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

    if(AlterarCabecalho(nomeArqArvore, '1', no.RRNdoNo, no.RRNdoNo+1) == -1) return -1;
    if (EscreveNo(nomeArqArvore, no, no.RRNdoNo) == -1) return -1;

    return 0;
}

NoPos InserirNoSemOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info)
{
    NoArvBin no = CriarNo();
    //resultado.no.nroChavesNo++;

    no = OrdenaNo(resultado.no, resultado.posInsercao, info);

    if(EscreveNo(nomeArqArvore, no, no.RRNdoNo) == -1)
        printf("ERRO: INSERIR SEM OVERFLOW -> ESCREVE NO\n");

    resultado.no = no;

    //LiberaNo(no);
    return resultado;
}

int InserirNoComOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info)
{
    printf("=========================================== NOVO INSERIR COM OVERFLOW ===========================================\n");
    /*
    printf(">>>>>>>>>>>>>> PARA INSERIR: %ld\n", info.C);
    printf(">>>>>>>>>>>>>> ONDE INSERIR -> RRN: %d ", resultado.no.RRNdoNo);
    for(int y = 0; y < tamCPR; y++)
        printf("C[%d]: %ld -> ", y, resultado.no.info[y].C);

    printf("\n\n");
    */

    while(resultado.no.nroChavesNo > tamCPR)
    {
        CabecalhoArvBin cabecalho = LerCabecalhoArvore(nomeArqArvore);
        RegistroInfo *infosOrdenadas;
        NoArvBin noEsquerdo = CriarNo();
        NoArvBin noDireito = CriarNo();
        int j = 0, posicaoPromovida = 2;                   // (m/2 + 0.5)-1 => Como ordem é impar soma-se 0.5 e retira-se 1 pela contagem começar no zero. 
        int novoProxRRN = cabecalho.RRNproxNo;
        //NoPos resultado = BuscarNoArvore(nomeArqArvore, noPromocao.info[0].C);

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
                noDireito.info[j] = infosOrdenadas[i];
                noDireito.P[j] = resultado.no.P[i];
                j++;
            }
        }

        noEsquerdo = AlterarNo(noEsquerdo, resultado.no.folha, posicaoPromovida, resultado.no.RRNdoNo);
        noDireito = AlterarNo(noDireito, resultado.no.folha, posicaoPromovida, novoProxRRN);
        novoProxRRN++;
        int novaPosicao = EncontraPosicao(resultado.noAnt, resultado.no.info[posicaoPromovida]);

        /*
        for(int y = 0; y < 10; y++)
            printf("RRN[%d]: %d -> ", y, resultado.listaRRN[y]);

        printf("\nRRN DO NO: %d -> ", resultado.no.RRNdoNo);
        for(int y = 0; y < tamCPR; y++)
            printf("C[%d]: %ld -> ", y, resultado.no.info[y].C);

        printf("\n\n");
        */

        printf(">>>>>>>>>>>>>> NO ANTERIOR ANTES DA INSERCAO -> RRN: %d ", resultado.noAnt.RRNdoNo);
        for(int y = 0; y < ordemArvore; y++)
            printf("C[%d]: %ld -> ", y, resultado.noAnt.info[y].C);

        printf("\n\n"); 

        if(resultado.no.RRNdoNo == resultado.listaRRN[0])
        {
            NoArvBin novaRaiz = CriarNo();

            novaRaiz = AlterarNo(novaRaiz, '0', 1, novoProxRRN);
            novaRaiz.P[0] = noEsquerdo.RRNdoNo;
            novaRaiz.P[1] = noDireito.RRNdoNo;
            novaRaiz.info[0].C = -2;

            cabecalho.noRaiz = novaRaiz.RRNdoNo;
            novoProxRRN++;

            novaPosicao = 0;
            printf("INSERE OVERFLOW -> NOVA RAIZ: %d\n", novaRaiz.RRNdoNo);
            resultado.noAnt = novaRaiz;
        }
        else
        {
            NoArvBin noTemp = CriarNo();
            int i;
            //resultado.noAnt.nroChavesNo++;
            //printf("RRN DO NO ANTERIOR: %d || NRO CHAVES: %d\n", resultado.noAnt.RRNdoNo, resultado.noAnt.nroChavesNo);

            for(i = 0; i < ordemArvore; i++)
                noTemp.P[i] = resultado.noAnt.P[i];
            
            for(i = novaPosicao+2; i < ordemArvore; i++)
                resultado.noAnt.P[i] = noTemp.P[i-1]; 
            
            resultado.noAnt.P[novaPosicao + 1] = noDireito.RRNdoNo;
        }
        
        //printf("INSERE OVERFLOW -> CABECALHO: %d\n", cabecalho.noRaiz);
        AlterarCabecalho(nomeArqArvore, '1', cabecalho.noRaiz, novoProxRRN);
        resultado = PromoveChave(nomeArqArvore, novaPosicao, infosOrdenadas[posicaoPromovida], resultado.noAnt);

        /*
        */
        printf(">>>>>>>>>>>>>> CHAVE PARA PROMOVER: %ld\n", infosOrdenadas[posicaoPromovida].C);
        printf(">>>>>>>>>>>>>> NO APOS INSERCAO -> RRN: %d ", resultado.no.RRNdoNo);
        for(int y = 0; y < ordemArvore; y++)
            printf("C[%d]: %ld -> P[%d]: %d || ", y, resultado.no.info[y].C, y, resultado.no.P[y]);

        printf("\n");

        printf("\n  >>>>>>>>>>>>>> NO ESQUERDA -> RRN: %d ", noEsquerdo.RRNdoNo);
        for(int y = 0; y < ordemArvore; y++)
            printf("C[%d]: %ld -> P[%d]: %d || ", y, noEsquerdo.info[y].C, y, noEsquerdo.P[y]);

        printf("\n  >>>>>>>>>>>>>> NO DIREITA -> RRN: %d ", noDireito.RRNdoNo);
        for(int y = 0; y < ordemArvore; y++)
            printf("C[%d]: %ld -> P[%d]: %d || ", y, noDireito.info[y].C, y, noDireito.P[y]);

        printf("\n\n");

        EscreveNo(nomeArqArvore, noEsquerdo, noEsquerdo.RRNdoNo);
        EscreveNo(nomeArqArvore, noDireito, noDireito.RRNdoNo);


        free(noEsquerdo.P);
        free(noEsquerdo.info);
        free(noDireito.P);
        free(noDireito.info);
    }
    /*
    CabecalhoArvBin cabecalho = LerCabecalhoArvore(nomeArqArvore);
    RegistroInfo *infosOrdenadas;
    NoArvBin noEsquerdo = CriarNo();
    NoArvBin noDireito = CriarNo();
    int j = 0, posicaoPromovida = 2;                   // (m/2 + 0.5)-1 => Como ordem é impar soma-se 0.5 e retira-se 1 pela contagem começar no zero. 
    int novoProxRRN = cabecalho.RRNproxNo;

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
            noDireito.info[j] = infosOrdenadas[i];
            noDireito.P[j] = resultado.no.P[i];
            j++;
        }
    }

    noEsquerdo = AlterarNo(noEsquerdo, resultado.no.folha, posicaoPromovida, resultado.no.RRNdoNo);
    noDireito = AlterarNo(noDireito, resultado.no.folha, posicaoPromovida, novoProxRRN);
    novoProxRRN++;

    int novaPosicao = EncontraPosicao(resultado.noAnt, resultado.no.info[posicaoPromovida]);
    //printf("|||||||||||||||||||||||||||||||| TAM LISTA: %d\n", resultado.tamListaRRN);
    //if(resultado.no.RRNdoNo == resultado.listaRRN[0])

    for(int y = 0; y < 10; y++)
        printf("RRN[%d]: %d -> ", resultado.listaRRN[y]);

    printf("\n");

    //if(tamListaRRN <= 1)                           // Overflow na Raiz
    if(resultado.no.RRNdoNo == resultado.listaRRN[0])
    {
        NoArvBin novaRaiz = CriarNo();

        novaRaiz = AlterarNo(novaRaiz, 0, 1, novoProxRRN);
        novaRaiz.P[0] = noEsquerdo.RRNdoNo;
        novaRaiz.P[1] = noDireito.RRNdoNo;
        novaRaiz.info[0].C = -2;

        cabecalho.noRaiz = novaRaiz.RRNdoNo;
        novoProxRRN = novaRaiz.RRNdoNo+1;

        novaPosicao = 0;
        printf("INSERE OVERFLOW -> NOVA RAIZ: %d\n", novaRaiz.RRNdoNo);
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
    }
    
    //printf("INSERE OVERFLOW -> CABECALHO: %d\n", cabecalho.noRaiz);
    AlterarCabecalho(nomeArqArvore, '1', cabecalho.noRaiz, novoProxRRN);
    PromoveChave(nomeArqArvore, novaPosicao, infosOrdenadas[posicaoPromovida], resultado.noAnt);

    EscreveNo(nomeArqArvore, noEsquerdo, noEsquerdo.RRNdoNo);
    EscreveNo(nomeArqArvore, noDireito, noDireito.RRNdoNo);

    free(infosOrdenadas);
    */
    return 0;
}

// NoPromocao -> onde insere
NoPos PromoveChave(char *nomeArqArvore, int posInsercao, RegistroInfo promovido, NoArvBin noPromocao)
{
    //int posInsercao = EncontraPosicao(noPromocao, promovido);
    NoPos resultado = BuscarNoArvore(nomeArqArvore, noPromocao.info[0].C);
    resultado.no = noPromocao;
    resultado.pos= -1;
    resultado.posInsercao = posInsercao;
    
    resultado.no.nroChavesNo++;

    if(noPromocao.info[0].C == -2)              // CASO SEJA NOVA RAIZ
    {
        noPromocao.info[0].C = promovido.C;
        noPromocao.info[0].PR = promovido.PR;
        
        if(EscreveNo(nomeArqArvore, noPromocao, noPromocao.RRNdoNo) == -1) 
            printf("ERRO: ESCREVE NO -> PROMOVE CHAVE\n");

        /*
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> NOVA RAIZ => RRN: %d\n", noPromocao.RRNdoNo);
        for(int h = 0; h < tamCPR; h++)
            printf("C[%d]: %ld -> P[%d]: %d || ", h, noPromocao.info[h].C, h, noPromocao.P[h]);
        
        printf("P[%d]: %d\n\n", ordemArvore, noPromocao.P[ordemArvore-1]);

        */
        return resultado;
    }

    if(resultado.no.nroChavesNo <= tamCPR)
    {
        resultado = InserirNoSemOverflow(nomeArqArvore, resultado, promovido);

    }

     
    return resultado;

    //printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>> NRO CHAVES NO PROMOCAO: %d\n", noPromocao.nroChavesNo);
    /*
    for(int h = 0; h < ordemArvore; h++)
        printf("C[%d]: %ld|| ", h, noPromocao.info[h].C);
    printf("\n\n");

    if(noPromocao.nroChavesNo == tamCPR)                                    // Ocorre overflow na promocao
    {
        //sprintf("TAM ORIGINAL: %d -> TAM - 1: %d\n", tamListaRRN, tamListaRRN-1);


        //if(tamListaRRN > 0)
            //tamListaRRN -= 1;

        //return -1;
        //tamListaRRN=tamListaRRN-1;
        //printf("=============================================== OVERFLOW DE OVERFLOW ===============================================\n");
        //InserirNoComOverflow(nomeArqArvore, resultado, promovido);
    }
    else                                                                    // Não ocorre overflow
        InserirNoSemOverflow(nomeArqArvore, resultado, promovido);

    */

}
