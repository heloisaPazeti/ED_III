#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcoesFornecidas.h"
#include "funcoesAuxiliares.h"
//////////////////////////////////////////// FUNÇÕES AUXILIARES

// Abre arquivo.
FILE* abrirArquivo(char *nomeArq, char *mode)
{
    FILE *arquivo = fopen(nomeArq, mode);

    if(arquivo == NULL)
        printf(">> ARQUIVO <%s> NÃO ENCONTRADO.\n", nomeArq);

    return arquivo;
}

// Cria a espécie e formata os campos a serem preenchidos
Especie criarEspecie(void)
{
    Especie especie;
    int j;

    for(j=0; j<41;j++)                                          // Preenche os campos da string com o caracter de espaço vazio
    {
        especie.nome[j] = '$';
    }
    for(j=0; j<61;j++)                                          // Preenche os campos da string com o caracter de espaço vazio
    {
        especie.nomeCient[j] = '$';
    }
    for(j=0; j<9;j++)                                           // Preenche os campos da string com o caracter de espaço vazio
    {
        especie.status[j] = '$';
    }
    scanf("%d", &especie.id);                                   // Obtém os dados da espécie
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

// Recebe uma espécie lida de um arquivo binário e mostra ao usuário suas propriedades formatadas
void mostrarRelatorio(Especie especie)
{
        printf("ID: %d\n", especie.id);
        printf("Nome: %s\n", especie.nome);
        printf("Nome Científico: %s\n", especie.nomeCient);
        if(especie.populacao == 0)                              // Caso a população seja 0, imprime 'NULO'
        {
            printf("População: NULO\n");
        }
        else
        {
            printf("População: %d\n", especie.populacao);
        }
        printf("Status: %s\n", especie.status);
        printf("Localização: (%.2f, %.2f)\n", especie.locX, especie.locY);
        if(especie.impacto == 0)                                // Caso o impacto seja 0, imprime 'NULO'
        {
            printf("Impacto Humano: NULO\n");
        }
        else
        {
            printf("Impacto Humano: %d\n", especie.impacto);
        }
        printf("\n");
}

// Recupera o registro de alguma especie.
// Supõe-se que o id já terá sido verificado e o registro existe.
// Serão lidos os demais campos.
int montarEspecie(Especie *especie, FILE *arquivo)
{
    fread(especie -> nome, sizeof(char), 41, arquivo);
    fread(especie -> nomeCient, sizeof(char), 61, arquivo);
    fread(&especie -> populacao, sizeof(int), 1, arquivo);
    fread(especie -> status, sizeof(char), 9, arquivo);
    fread(&especie -> locX, sizeof(float), 1, arquivo);
    fread(&especie -> locY, sizeof(float), 1, arquivo);
    fread(&especie -> impacto, sizeof(int),1,arquivo);
}
