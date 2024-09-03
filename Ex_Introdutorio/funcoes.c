#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcoesFornecidas.h"

int registrarEspecie(char *nomeArq)
{
    FILE *arquivo;
    Especie especie;
    int i, num;

    printf("%s", nomeArq);

    arquivo = fopen(nomeArq,"wb");

    scanf("%d", &num);
    getchar();

    for(i=0; i<num; i++)
    {
        especie = criarEspecie();

        fwrite(&especie.id, sizeof(int),1,arquivo);
        fwrite(especie.nome, sizeof(char),41,arquivo);
        fwrite(especie.nomeCient, sizeof(char),61,arquivo);
        fwrite(&especie.populacao, sizeof(int),1,arquivo);
        fwrite(especie.status, sizeof(char),9,arquivo);
        fwrite(&especie.locX, sizeof(float),1,arquivo);
        fwrite(&especie.locY, sizeof(float),1,arquivo);
        fwrite(&especie.impacto, sizeof(int),1,arquivo);
    }
    fclose(arquivo);

    binarioNaTela(nomeArq);

    return 0;

}
int relatorioEspecies(char *nomeArq)
{
    FILE *arquivo;
    Especie especie;
    char c;

    arquivo = fopen(nomeArq, "rb");

    while(1)
    {
        if(fread(&especie.id, sizeof(int),1,arquivo) == 0)
            break;
        fread(especie.nome, sizeof(char), 41, arquivo);
        fread(especie.nomeCient, sizeof(char), 61, arquivo);
        fread(&especie.populacao, sizeof(int), 1, arquivo);
        fread(especie.status, sizeof(char), 9, arquivo);
        fread(&especie.locX, sizeof(float), 1, arquivo);
        fread(&especie.locY, sizeof(float), 1, arquivo);
        if(fread(&especie.impacto, sizeof(int),1,arquivo) == 0)
            break;

        mostrarRelatorio(especie);

    }
    return 0;
}
int buscarEspecie(char *nomeArq);
int registrarInformacao(char *nomeArq);

Especie criarEspecie(void)
{
    Especie especie;
    int j;

    for(j=0; j<41;j++)
    {
        especie.nome[j] = '$';
    }
    for(j=0; j<61;j++)
    {
        especie.nomeCient[j] = '$';
    }
    for(j=0; j<9;j++)
    {
        especie.status[j] = '$';
    }
    scanf("%d", &especie.id);
    getchar();
    readline(especie.nome);
    readline(especie.nomeCient);
    scanf("%d", &especie.populacao);
    getchar();
    readline(especie.status);
    scanf("%f %f", &especie.locX, &especie.locY);
    getchar();
    scanf("%d", &especie.impacto);
    getchar();

    return especie;
}

void mostrarRelatorio(Especie especie)
{
    printf("ID: %d\n", especie.id);
        printf("Nome: %s\n", especie.nome);
        printf("Nome Científico: %s\n", especie.nomeCient);
        if(especie.populacao == 0)
        {
            printf("População: NULO\n");
        }
        else
        {
            printf("População: %d\n", especie.populacao);
        }
        printf("Status: %s\n", especie.status);
        printf("Localização: (%.2f, %.2f)\n", especie.locX, especie.locY);
        if(especie.impacto == 0)
        {
            printf("Impacto Humano: NULO\n");
        }
        else
        {
            printf("Impacto Humano: %d\n", especie.impacto);
        }
        printf("\n");
}
