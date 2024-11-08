#ifndef STRUCTS_H
#define STRUCTS_H

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


class Presa
{
    public:
        std::string _nome;

        Presa(std::string nome)
        {
            _nome = nome;
        }
};

class Vertice
{
    private: 
        std::string _nome;
        std::string _especie;
        std::string _habitat; 
        std::string _dieta; 
        std::string _tipo; 
        int _grauEntrada; 
        int _grauSaida;
        int _grau;
        std::set<Presa> _vetorPresa;
    public:
        Vertice(std::string nome, std::string especie, std::string habitat, std::string dieta, std::string tipo)
        {
            _nome = nome;
            _especie = especie;
            _habitat = habitat;
            _dieta = dieta;
            _tipo = tipo;
        }

        void AumentarGrauEntrada()
        {
            ++_grauEntrada;
        }

        void AumentarGrauSaida()
        {
            ++_grauSaida;
        }

        void InserirPresa(std::string nome)
        {
            Presa presa(nome);
            _vetorPresa.insert(presa);
        }

        std::string Nome()
        {
            return _nome;
        }

        void MostrarVertice()
        {
            printf("%d\n", _nome);
            printf("%d\n", _especie);
            printf("%d\n", _habitat);
        }
};


#endif