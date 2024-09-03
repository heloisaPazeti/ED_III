#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcoesFornecidas.h"

int registrarEspecie(char *nomeArq)
{
    FILE *arquivo;
    Especie especie;
    int i, j, num;

    printf("%s", nomeArq);

    arquivo = fopen(nomeArq,"wb");

    scanf("%d", &num);
    getchar();

    for(i=0; i<num; i++)
    {
        scanf("%d", &especie.id);
        getchar();
        fgets(especie.nome, 41, stdin);
        for(j=strlen(especie.nome)-1; j<40;j++)
        {
            especie.nome[j] = '$';
        }
        fgets(especie.nomeCient, 61, stdin);
        for(j=strlen(especie.nomeCient)-1; j<60;j++)
        {
            especie.nomeCient[j] = '$';
        }
        scanf("%d", &especie.populacao);
        getchar();
        fgets(especie.status, 9, stdin);
        for(j=strlen(especie.status)-1; j<8;j++)
        {
            especie.status[j] = '$';
        }
        scanf("%f %f", &especie.locX, &especie.locY);
        getchar();
        scanf("%d", &especie.impacto);
        getchar();

        fprintf(arquivo, "%d", especie.id);
        fprintf(arquivo, "%s", especie.nome);
        fprintf(arquivo, "%s", especie.nomeCient);
        fprintf(arquivo, "%d", especie.populacao);
        fprintf(arquivo, "%s", especie.status);
        fprintf(arquivo, "%.2f%.2f", especie.locX, especie.locY);
        fprintf(arquivo, "%d", especie.impacto);
    }
    fclose(arquivo);

    binarioNaTela(nomeArq);

    return 0;

}
int relatorioEspecies(char *nomeArq);
int buscaEspecie(char *nomeArq);
int registrarInformacao(char *nomeArq);


