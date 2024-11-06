#include "structs.h"

//////////////////////////////////////////////////////// INICIALIZADORES REGISTROS

// Inicializa um cabeçalho com valores padrões ou nulos
RegCabecalho IniciarCabecalho()
{
    RegCabecalho cabecalho;

    cabecalho.status = '0';
    cabecalho.topo = -1;
    cabecalho.proxRRN = 0;
    cabecalho.nroRegRem = 0;
    cabecalho.nroPagDisco = 0;
    cabecalho.qttCompacta = 0;

    return cabecalho;
}

// Inicializa novo registro com valores padrões ou nulos
RegDados IniciarRegistroDados()
{
    RegDados dados;

    dados.removido = '0';
    dados.encadeamento = -1;

    dados.populacao = -1;
    dados.tamanho = -1;
    dados.unidadeMedida = '$';
    dados.velocidade = -1;

    dados.nome = calloc(160, sizeof(char));
    dados.especie = calloc(160, sizeof(char));
    dados.habitat = calloc(160, sizeof(char));
    dados.tipo = calloc(160, sizeof(char));
    dados.dieta = calloc(160, sizeof(char));
    dados.alimento = calloc(160, sizeof(char));

    return dados;
}

//////////////////////////////////////////////////////// INICIALIZADORES ARVORES

// Inicializa um cabecalho da arvore binaria com valores padroes
CabecalhoArvBin CriarCabecalhoArvBin()
{
    CabecalhoArvBin newCabecalho;

    newCabecalho.status = '0';
    newCabecalho.noRaiz = -1;
    newCabecalho.RRNproxNo = 0;

    for (int i = 0; i < 84; i++)
        newCabecalho.lixo[i] = '$';

    return newCabecalho;
}

// Inicializa um no com valores padroes
NoArvBin CriarNo()
{
    NoArvBin newNo;
    int i;

    newNo.folha = '1';
    newNo.nroChavesNo = 0;
    newNo.RRNdoNo = -1;

    newNo.P = calloc(ordemArvore, sizeof(int));
    newNo.info = calloc(tamCPR, sizeof(RegistroInfo));

    for(i = 0; i < tamCPR; i++)
    {
        newNo.P[i] = -1;
        newNo.info[i].C = -1;
        newNo.info[i].PR = -1;
    }

    newNo.P[i] = -1;

    return newNo;
}

NoPos CriarNoPos()
{
    NoPos noPos;
    NoArvBin no = CriarNo();
    NoArvBin noAnt = CriarNo();

    noPos.no = no;
    noPos.noAnt = noAnt;
    noPos.pos = -1;
    noPos.posInsercao = -1;

    return noPos;
}

