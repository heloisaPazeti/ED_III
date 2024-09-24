#include "structs.h"

//////////////////////////////////////////////////////// INICIALIZAÇÕES

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
