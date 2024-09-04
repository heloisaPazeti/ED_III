#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"
#include "funcoesFornecidas.h"

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

// Encontra um determinado registrado dado um rrn especifico
// É encontrado utilizando o calculo byte offset = rrn * tamanho do registro
// Com o byte offset é possível mover o ponteiro do leitor do arquivo para o registro desejado
int buscarEspecie(char *nomeArq)
{
    int rrn, byteOffset;
    FILE *arquivo;
    Especie especie;

    scanf("%d", &rrn);
    byteOffset = rrn * tamanhoRegistro;

    arquivo = abrirArquivo(nomeArq, "rb");                  // Abre o arquivo - 'rb' read-only binario
    if(arquivo == NULL) {return -1;}                        // Se for nulo sai da função

    int result = fseek(arquivo, byteOffset, SEEK_SET);      // Move o ponteiro leitor do arquivo até o ponto desejado a partir do inicio

    if(result != 0)                                         // Checa se encontrou esse ponto na memória
    {
        printf(">> Erro ao posicionar byte offset\n");
        return -1;
    }
    
    if(fread(&especie.id, sizeof(int),1,arquivo) == 0)      // Checa se o ID existe
    {
        printf("Espécie não encontrada\n");
        return -1;
    }

    montarEspecie(&especie, arquivo);                       // Recupera a especie nesse registro
    mostrarRelatorio(especie);                              // Escreve as informações do registro
    fclose(arquivo);
    return 0;
    
}

// A partir de um ID passado encontra o registro (caso exista).
// Ao encontrar o registro recolhe quais campos devem ser alterados (caso existam ou possam ser alterados).
// Ao entrar no campo checa se já existe ou não alguma informação nesse campo (caso sim nada é alterado).
// Altera os dados e imprime o binario.
int registrarInformacao(char *nomeArq)
{
    int id, quantCampo;
    int result, i, k = 0;
    FILE *arquivo;
    Especie especie;

    scanf("%d", &id);

    arquivo = abrirArquivo(nomeArq, "rb+");                         // Abre arquivo - 'rb+' read+write em binário
    if(arquivo == NULL) {return -1;}

    while(1)
    {
        k++;                                                        // Quantos arquivos foi avançando
        if(fread(&especie.id, sizeof(int),1,arquivo) == 0)          // Chegou ao final e não encontrou o id
        {
            printf("Espécie não encontrada\n");
            break;
        }

        if(especie.id != id)                                        // Se esse não for o id procurado
            fseek(arquivo, tamanhoRegistro-idSize, SEEK_CUR);       // Avança para o próximo registro (tamanho-idSize porque já foi lido o id)
        else
        {                                                           // Quando encontrar o Id
            scanf("%d", &quantCampo);                               // Quantos campos irá alterar
            getchar();  
            montarEspecie(&especie, arquivo);                       // Recupera registro desse id

            for(i = 0; i < quantCampo; i++)                         // Enquanto tem que alterar campos
            {
                char campo[15];

                fflush(stdin);
                fgets(campo, 15, stdin);                            // Recebe campo que deve ser alterado

                if(strncmp(campo, "POPULATION", 10) == 0)           // Caso seja o campo População
                {
                    int populacao;
                    scanf("%d", &populacao);                        // Nova quantidade de população
                    getchar();

                    if(especie.populacao != 0)                      // Verifica se o campo está "vazio"
                        result = 1;                                 // Não altera o campo, depois irá printar aviso
                    else    
                    {
                        fseek(arquivo, -25, SEEK_CUR);              // Move ponteiro do final do registro para o campo de população
                        fwrite(&populacao, sizeof(int), 1, arquivo);// Altera dados
                    }
                }
                else if(strncmp(campo, "STATUS", 6) == 0)           // Caso seja Status
                {
                    char status[9];
                    for(int h=0; h<9;h++)                           // Deixa tudo com $
                        status[h] = '$';

                    readline(status);                               // Pega o novo status

                    if(strcmp(especie.status, "NULO") != 0)         // Verifica se o campo está "vazio" ou não
                        result = 1;
                    else
                    {
                        fseek(arquivo, -21, SEEK_CUR);              // Move o ponteiro do final do registro para o campo status
                        fwrite(status, sizeof(char), 9, arquivo);   // Altera o campo status
                    }
                }
                else if(strncmp(campo, "HUMAN IMPACT", 12) == 0)    // Caso seja o Impacto Humano
                {
                    int impact;
                    scanf("%d", &impact);
                    getchar();

                    if(especie.impacto != 0)                        // Verifica se o campo está "vazio"
                        result = 1;
                    else
                    {   
                        fseek(arquivo, -4, SEEK_CUR);               // Move ponteiro do final do registro para o campo impacto
                        fwrite(&impact, sizeof(int), 1, arquivo);   // Altera o campo do impacto
                    }
                }
                else
                {
                    printf(">> O campo <%s", campo ,"> não pode ser alterado ou não existe.\n");   // Caso o campo não exista ou não possa ser alterado (por ser um campo não nulo)
                    return -1;
                }

                if(result == 1)
                    printf("Informação já inserida no arquivo\n");  // Caso algum campo não estivesse vazio

                fseek(arquivo, k*tamanhoRegistro, SEEK_SET);        // Move o ponteiro para o final do registro (para os demais cálculos funcionarem)
            }
            
            break;
        }
    }

    fclose(arquivo);
    binarioNaTela(nomeArq);
    return 0;

}

//////////////////////////////////////////// FUNÇÕES AUXILIARES

// Abre arquivo.
FILE* abrirArquivo(char *nomeArq, char *mode)
{
    FILE *arquivo = fopen(nomeArq, mode);

    if(arquivo == NULL)
        printf(">> ARQUIVO <%s", nomeArq, "> NÃO ENCONTRADO.\n");

    return arquivo;
}

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