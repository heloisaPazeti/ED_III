#include <string>
#include <iostream>
#include <set>
#include <stdio.h>
#include <stdlib.h>

// ========================================================================
// ========================= STRUCTS DOS REGISTROS ========================
// ========================================================================

typedef struct RegDados
{
    char removido;
    int encadeamento;
    int populacao;
    float tamanho;
    char unidadeMedida;
    int velocidade;

    char *nome;
    char *especie;
    char *habitat;
    char *tipo;
    char *dieta;
    char *alimento;

} RegDados;

typedef struct _cabecalho
{
    char status;
    int topo;
    int proxRRN;
    int nroRegRem;
    int nroPagDisco;
    int qttCompacta;

} RegCabecalho;

// ========================================================================
// =========================== CLASSES DO GRAFO ===========================
// ========================================================================

class Vertice
{
    private: 
        std::string nome;
        std::string especie;
        std::string habitat; 
        std::string dieta; 
        std::string tipo; 
        int grauEntrada; 
        int grauSaida;
        int grau;
        std::set<Presa> vetorPresa;
};

class Presa
{
    public:
        std::string nome;
        int populacao;
};