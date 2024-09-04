#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcoesFornecidas.h"

#define tamanhoRegistro 131

//////////////////////////////////////////// OPÇÕES DE REGISTROS

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

    fclose(arquivo);
    return 0;
}

int procurarRegistro(char *nomeArq)
{
    int rrn, byteOffset;
    FILE *arquivo;
    Especie especie;

    scanf("%d", &rrn);
    byteOffset = rrn * tamanhoRegistro;

    arquivo = fopen(nomeArq, "rb");
    if(arquivo == NULL)
    {
        printf(">> Arquivo nao encontrado...\n");
        return -1;
    }

    int result = fseek(arquivo, byteOffset, SEEK_SET);

    if(result != 0)
    {
        printf(">> Erro ao posicionar byte offset\n");
        return -1;
    }

    if(fread(&especie.id, sizeof(int),1,arquivo) == 0)
    {
        printf("Espécie não encontrada\n");
        return -1;
    }
    fread(especie.nome, sizeof(char), 41, arquivo);
    fread(especie.nomeCient, sizeof(char), 61, arquivo);
    fread(&especie.populacao, sizeof(int), 1, arquivo);
    fread(especie.status, sizeof(char), 9, arquivo);
    fread(&especie.locX, sizeof(float), 1, arquivo);
    fread(&especie.locY, sizeof(float), 1, arquivo);
    if(fread(&especie.impacto, sizeof(int),1,arquivo) == 0)
        return -1;                                                  //Hã?
            

    mostrarRelatorio(especie);
    fclose(arquivo);
    return 0;
    
}

int alterarRegistro(char *nomeArq)
{
    int id, quantCampo;
    int result, i, k = 0;
    FILE *arquivo;
    Especie especie;

    scanf("%d", &id);
    
    arquivo = fopen(nomeArq, "rb+");
    if(arquivo == NULL)
    {
        printf(">> Arquivo nao encontrado...\n");
        return -1;
    }

    while(1)
    {
        k++;
        if(fread(&especie.id, sizeof(int),1,arquivo) == 0)
        {
            printf(">> Espécie não encontrada...\n");
            break;
        }

        if(especie.id != id)
            fseek(arquivo, tamanhoRegistro-4, SEEK_CUR);
        else
        {
            scanf("%d", &quantCampo);
            getchar();

            fread(especie.nome, sizeof(char), 41, arquivo);
            fread(especie.nomeCient, sizeof(char), 61, arquivo);
            fread(&especie.populacao, sizeof(int), 1, arquivo);
            fread(especie.status, sizeof(char), 9, arquivo);
            fread(&especie.locX, sizeof(float), 1, arquivo);
            fread(&especie.locY, sizeof(float), 1, arquivo);
            fread(&especie.impacto, sizeof(int),1,arquivo);
                                               
            for(i = 0; i < quantCampo; i++)
            {
                char campo[15];

                fflush(stdin);
                fgets(campo, 15, stdin);

                if(strncmp(campo, "POPULATION", 10) == 0)
                {
                    int populacao;
                    scanf("%d", &populacao);
                    getchar();

                    if(especie.populacao != 0)
                        result = 1;
                    else
                    {
                        fseek(arquivo, -25, SEEK_CUR);
                        fwrite(&populacao, sizeof(int),1,arquivo);
                    }
                }
                else if(strncmp(campo, "STATUS", 6) == 0)
                {
                    char status[9];
                    for(int h=0; h<9;h++)
                        status[h] = '$';

                    readline(status);

                    if(strcmp(especie.status, "NULO") != 0)
                        result = 1;
                    else
                    {
                        fseek(arquivo, -21, SEEK_CUR);
                        fwrite(status, sizeof(char),9,arquivo);
                    }
                }
                else if(strncmp(campo, "HUMAN IMPACT", 12) == 0)
                {
                     int impact;
                    scanf("%d", &impact);
                    getchar();

                    if(especie.impacto != 0)
                        result = 1;
                    else
                    {   
                        fseek(arquivo, -4, SEEK_CUR);
                        fwrite(&impact, sizeof(int),1,arquivo);
                    }
                }
                else
                {
                    printf(">> Esse campo não pode ser alterado ou não existe!!\n");
                    return -1;
                }

                if(result == 1)
                    printf("Informação já inserida no arquivo\n");

                fseek(arquivo, k*tamanhoRegistro, SEEK_SET);
            }
            
            break;
        }
    }

    fclose(arquivo);
    binarioNaTela(nomeArq);
    return 0;

}


//////////////////////////////////////////// FUNÇÕES AUXILIARES

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
