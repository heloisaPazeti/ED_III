#ifndef STRUCTS_H
#define STRUCTS_H

#include <string>
#include <iostream>
#include <set>
#include <list>
#include <algorithm>
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
// ====================== FUNCOES DE INICIALIZACAO ========================
// ========================================================================

RegDados InicializarRegistro();

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

        std::string Nome() const { return _nome; }
        int Populacao() { return _populacaoPredador; }

    friend bool operator<(const Presa p1, const Presa p2) {return p1._nome < p2._nome;}


    friend std::ostream& operator<<(std::ostream &out, const std::set<Presa> &vetorPresas) 
    {
        std::set<Presa>::iterator itPresas;

        if(vetorPresas.empty())
            out << "VAZIA";
        else
        {
            for(itPresas = vetorPresas.begin(); itPresas!=vetorPresas.end(); itPresas++)
                out << "-> " << (*itPresas).Nome() << "\n";

        }

        return out;
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
        std::set<Presa>::iterator _it;
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

        Vertice(std::string nome) { _nome = nome; }

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

        std::string Nome() const { return _nome; }
        
        void InserirPresa(std::string nome, int populacao)
        {
            std::pair<std::set<Presa>::iterator, bool> par;
            Presa presa(nome, populacao);
            par = _vetorPresa.insert(presa);
            _it = par.first;
        }


        void MostrarVertice()
        {
            std::set<Presa>::iterator it;
            for(it = _vetorPresa.begin(); it!=_vetorPresa.end(); it++)
            {
                Presa ans = *it;
                if(ans.Populacao()!=-1)
                    std::cout <<  _nome << " " << _especie << " " << _habitat << " " << _dieta << " " << _tipo << " " << _grauEntrada 
                    << " " << _grauSaida << " " << _grau << " " << ans.Nome() << " " << ans.Populacao() <<  std::endl;
            }
            
        }

        std::string BuscarPresa(std::string nomePresa)
        {
            Presa presa(nomePresa, 0);
            std::set<Presa>::iterator it = _vetorPresa.find(presa);
            if(it != _vetorPresa.end())
                return _nome;
            else 
                return "-1";
        }

        std::set<Presa> Adjacencias() { return _vetorPresa; }

        friend bool operator==(const Vertice v1, const Vertice v2) {return v1._nome == v2._nome;}
        friend bool operator<(const Vertice v1, const Vertice v2) {return v1._nome < v2._nome;}

        friend std::ostream& operator<<(std::ostream &out, const std::set<Vertice> &vetorVertices) 
        {
            if(vetorVertices.empty())
                out << "vazia\n";
            else
            {
                for(Vertice v : vetorVertices)
                    out << "-> " << v.Nome() << "\n";
            }

            return out;
        }

        friend std::ostream& operator<<(std::ostream &out, const std::list<Vertice> &vetorVertices) 
        {
            //std::list<Vertice>::iterator itVertice;
            for(Vertice v : vetorVertices)
                out << "-> " << v.Nome() << "\n";

            return out;
        }
};


#endif