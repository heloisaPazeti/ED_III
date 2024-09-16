#include "funcoesBusca.h"
RegDados lerRegistro(FILE *arquivo)
{
    RegDados temp;
    char dado[142], *linha;
    
    fread(&temp.removido, sizeof(int),1,arquivo);
    fread(&temp.encadeamento, sizeof(int),1,arquivo);
    fread(&temp.populacao, sizeof(int),1,arquivo);
    fread(&temp.tamanho, sizeof(int),1,arquivo);
    fread(&temp.unidadeMedida, sizeof(char),1,arquivo);
    fread(&temp.velocidade, sizeof(int), 1,arquivo);
    fread(dado, sizeof(char), 142, arquivo);

    linha = strdup(dado);
    temp.nome = strsep(&linha, "#");
    temp.especie = strsep(&linha, "#");
    temp.habitat = strsep(&linha, "#");
    temp.tipo = strsep(&linha, "#");
    temp.dieta = strsep(&linha, "#");
    temp.alimento = strsep(&linha, "#");

    return temp;
}

int definirTipo(char *nomeCampo)
{
    if(strncmp(nomeCampo, "populacao", 3)==0)
        return 1;

    if(strncmp(nomeCampo, "tamanho", 3)==0)
        return 2;

    if(strncmp(nomeCampo, "unidade", 3)==0)
        return 3;

    if(strncmp(nomeCampo, "velocidade", 3)==0)
        return 4;

    if(strncmp(nomeCampo, "nome", 3)==0)
        return 5;

    if(strncmp(nomeCampo, "especie", 3)==0)
        return 6;

    if(strncmp(nomeCampo, "habitat", 3)==0)
        return 7;

    if(strncmp(nomeCampo, "tipo", 3)==0)
        return 8;
    
    if(strncmp(nomeCampo, "dieta", 3)==0)
        return 9;
    
    if(strncmp(nomeCampo, "alimento", 3)==0)
        return 10;
}

void imprimirRegistro(RegDados registro)
{
    printf("Nome: %s", registro.nome);
    if(registro.especie != NULL)        
        printf("Especie: %s", registro.especie);
    if(registro.tipo != NULL)
        printf("Tipo: %s", registro.tipo);
    if(registro.dieta != NULL)
        printf("Dieta: %s", registro.dieta);
    if(registro.habitat != NULL)
        printf("Lugar que habitava: %s", registro.habitat);
    if(registro.tamanho != 0)
        printf("Tamanho: %f", registro.tamanho);
    if(registro.velocidade != 0 && registro.unidadeMedida != NULL)
        printf("Velocidade: %d %s", registro.velocidade, registro.unidadeMedida);
}