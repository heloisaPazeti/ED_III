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
    size_t tam;

    arquivo = fopen("saida.txt","w");

    scanf("%d", &num);
    for(i=0; i<num; i++)
    {
        scanf("%d", &especie.id);
        printf("%d", especie.id);
        fprintf(arquivo, "%d", especie.id);
        fgets(especie.nome, sizeof(especie.nome), stdin);
        printf("%s", especie.nome);
        fprintf(arquivo, "%s", especie.nome);
        fgets(especie.nomeCient, sizeof(especie.nomeCient), stdin);
        printf("%s", especie.nomeCient);
        fprintf(arquivo, "%s", especie.nomeCient);
        scanf("%d", &especie.populacao);
        printf("%d", especie.populacao);
        fprintf(arquivo, "%d", especie.populacao);
        fgets(especie.status, sizeof(especie.status), stdin);
        printf("%s", especie.status);
        fprintf(arquivo, "%s", especie.status);
        scanf("%f", &especie.locX);
        printf("%f", especie.locX);
        fprintf(arquivo, "%f", especie.locX);
        scanf("%f", &especie.locY);
        printf("%f", especie.locY);
        fprintf(arquivo, "%f", especie.locY);
        scanf("%d", &especie.impacto); 
        printf("%d", especie.impacto);
        fprintf(arquivo, "%d", especie.impacto);       
    }
    fclose(arquivo);
    
    
    
    return 0;

}
int relatorioEspecies(char *nomeArq);
int buscaEspecie(char *nomeArq);
int registrarInformacao(char *nomeArq);


