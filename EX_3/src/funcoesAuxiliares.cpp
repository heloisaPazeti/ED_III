#include "structs.h"
#include <string.h>

// ========================================================================
// ========================= FUNCOES DE LEITURA ===========================
// ========================================================================

RegDados LerRegistro(FILE *arquivo)
{
    RegDados temp;
    char dado[142], *linha;
    
    temp = InicializarRegistro(temp);
    
    if(fread(&temp.removido, sizeof(char),1,arquivo)==0)    // Caso a leitura falhe, o campo de remoção recebe um valor logicamente inválido
    {
        temp.removido = '2';
        return temp;
    }    
    if(temp.removido == '1')                                // Caso o registro tenha sido removido, não completa a leitura e retorna 
    {
        fread(&temp.encadeamento, sizeof(int),1,arquivo);
        return temp;
    }
    if(temp.removido != '1')                                // Caso o campo não tenha sido removido, a leitura dos campos é finalizada
    {
        fread(&temp.encadeamento, sizeof(int),1,arquivo);
        fread(&temp.populacao, sizeof(int),1,arquivo);
        fread(&temp.tamanho, sizeof(float),1,arquivo);
        fread(&temp.unidadeMedida, sizeof(char),1,arquivo);
        fread(&temp.velocidade, sizeof(int), 1,arquivo);
        if(fread(dado, sizeof(char), 142, arquivo)==0)      // Caso a leitura falhe, o campo de remoção recebe um valor logicamente inválido
        {
            temp.removido = '2';
            return temp;
        }

        linha = strdup(dado);                               // A string de dados (armazena os campos de tamanho variável) é duplicada
        temp.nome = strsep(&linha, "#");                    // Separação dos dados de acordo com o separador '#'
        temp.especie = strsep(&linha, "#");
        temp.habitat = strsep(&linha, "#");
        temp.tipo = strsep(&linha, "#");
        temp.dieta = strsep(&linha, "#");
        temp.alimento = strsep(&linha, "#");

        return temp;
    }
}

RegCabecalho LerCabecalho(FILE *arquivo) 
{
    RegCabecalho cabecalho;

    if (arquivo == NULL)
    {
        printf("Falha no processamento do arquivo [Arq. CSV == NULL]\n");
        cabecalho.status = '2';
        return cabecalho;
    }

    fseek(arquivo, 0, SEEK_SET);

    fread(&cabecalho.status, sizeof(char),1,arquivo);
    fread(&cabecalho.topo, sizeof(int),1,arquivo);
    fread(&cabecalho.proxRRN, sizeof(int),1,arquivo);
    fread(&cabecalho.nroRegRem, sizeof(int),1,arquivo);
    fread(&cabecalho.nroPagDisco, sizeof(int),1,arquivo);
    fread(&cabecalho.qttCompacta, sizeof(int),1,arquivo);

    fseek(arquivo, 1600, SEEK_SET);

    return cabecalho;
}

RegDados InicializarRegistro(RegDados registro)
{
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