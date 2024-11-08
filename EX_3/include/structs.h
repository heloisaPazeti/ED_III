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
        int _populacaoPredador;

        Presa(std::string nome, int populacao)
        {
            _nome = nome;
            _populacaoPredador = populacao;
        }

    friend bool operator<(const Presa p1, const Presa p2) {return p1._nome < p2._nome;}
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
        Vertice(char* nome, char* especie, char* habitat, char* dieta, char* tipo)
        {
            _nome = nome;
            _especie = especie;
            _habitat = habitat;
            _dieta = dieta;
            _tipo = tipo;
            _grauEntrada = 0;
            _grauSaida = 1;
            _grau = 1;
        }

        void AumentarGrauEntrada()
        {
            ++_grauEntrada;
            ++_grau;
        }

        void AumentarGrauSaida()
        {
            ++_grauSaida;
            ++_grau;
        }

        void InserirPresa(std::string nome, int populacao)
        {
            Presa presa(nome, populacao);
            _vetorPresa.insert(presa);
        }

        std::string Nome()
        {
            return _nome;
        }

        void MostrarVertice()
        {
            std::cout <<  _nome << std::endl;
            std::cout <<  _especie << std::endl;
            std::cout <<  _habitat << std::endl;
            std::cout <<  _dieta << std::endl;
        }

        friend bool operator<(const Vertice v1, const Vertice v2) {return v1._nome < v2._nome;}
};


#endif