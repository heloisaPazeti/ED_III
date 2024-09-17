#include "funcoesBusca.h"

int numPagDisco(FILE *arquivo)
{
    int num;
    fseek(arquivo, 13, SEEK_SET);
    fread(&num, sizeof(int),1,arquivo);

    return num;
}

RegDados lerRegistro(FILE *arquivo)
{
    RegDados temp, fim;
    char dado[142], *linha;
    
    if(fread(&temp.removido, sizeof(char),1,arquivo)==0)
    {
        fim.tamanho = -1;
        return fim;
    }    
    fread(&temp.encadeamento, sizeof(int),1,arquivo);
    fread(&temp.populacao, sizeof(int),1,arquivo);
    fread(&temp.tamanho, sizeof(int),1,arquivo);
    fread(&temp.unidadeMedida, sizeof(char),1,arquivo);
    if(fread(&temp.velocidade, sizeof(int), 1,arquivo)==0)
    {
        fim.tamanho=-1;
        return fim;
    }
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
    printf("Nome: %s\n", registro.nome);
    if(strncmp(registro.especie,"$",1)!=0)        
        printf("Especie: %s\n", registro.especie);
    if(strncmp(registro.tipo,"$",1)!=0)
        printf("Tipo: %s\n", registro.tipo);
    if(strncmp(registro.dieta,"$",1)!=0)
        printf("Dieta: %s\n", registro.dieta);
    if(strncmp(registro.habitat,"$",1)!=0)
        printf("Lugar que habitava: %s\n", registro.habitat);
    if(registro.tamanho != 0)
        printf("Tamanho: %.1f m\n", registro.tamanho);
    if(registro.velocidade != 0)
        printf("Velocidade: %d %cm/h\n", registro.velocidade, registro.unidadeMedida);
    printf("\n");
}