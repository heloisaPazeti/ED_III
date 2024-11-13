#include "structs.h"

// ========================================================================
// ====================== FUNCOES DE INICIALIZACAO ========================
// ========================================================================

RegDados InicializarRegistro()
{
    RegDados registro;

    registro.removido = '0';
    registro.encadeamento = -1;
    registro.populacao = 0;
    registro.tamanho = 0;
    registro.unidadeMedida = ' ';
    registro.alimento = (char*)calloc(160, sizeof(char));
    registro.dieta = (char*)calloc(160, sizeof(char));
    registro.especie = (char*)calloc(160, sizeof(char));
    registro.habitat = (char*)calloc(160, sizeof(char));
    registro.nome = (char*)calloc(160, sizeof(char));
    registro.tipo = (char*)calloc(160, sizeof(char));

    return registro;
}