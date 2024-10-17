#include "funcoesAuxiliares.h"
#include "structs.h"

//////////////////////////////////////////////////////// FUNCOES DE LEITURA

CabecalhoArvBin LerCabecalhoArvore(char *arquivo)
{
    CabecalhoArvBin cabecalho = CriarCabecalhoArvBin();
    FILE *arqBin = fopen(arquivo, "rb");

    if (arqBin == NULL)
    {
        printf("Arquivo nulo.\n");
        return cabecalho;
    }

    fread(&cabecalho.status, sizeof(char), 1, arqBin);
    fread(&cabecalho.noRaiz, sizeof(int), 1, arqBin);
    fread(&cabecalho.RRNproxNo, sizeof(int), 1, arqBin);
    fread(cabecalho.lixo, sizeof(char), 83, arqBin);

    fclose(arqBin);
    return cabecalho;
}

NoArvBin LerNoArvore(char *arquivo, int rrn)
{
    NoArvBin no = CriarNo();
    FILE *arqBin = fopen(arquivo, "rb");
    int i;

    if (arqBin == NULL)
        return no;
        
    fseek(arqBin, (rrn * tamNo) + tamCabecalho, SEEK_SET);

    fread(&no.folha, sizeof(char), 1, arqBin);
    fread(&no.nroChavesNo, sizeof(int), 1, arqBin);
    fread(&no.RRNdoNo, sizeof(int), 1, arqBin);

    for (i = 0; i < no.nroChavesNo; i++)
    {
        fread(&no.P[i], sizeof(int), 1, arqBin);
        fread(&no.C[i], sizeof(long), 1, arqBin);
        fread(&no.PR[i], sizeof(long), 1, arqBin);
    }

    fread(&no.P[i], sizeof(int), 1, arqBin);
    fclose(arqBin);
    return no;
}

//////////////////////////////////////////////////////// FUNCOES DE BUSCA

// Retorna posicao na arvore na qual está / deveria estar o registro
// Se houver retorna o rrn
// Se não houver retorna -1
NoPos BuscarNoArvore(char *arquivo, char *chave)
{
    CabecalhoArvBin cabecalho = LerCabecalhoArvore(arquivo);
    NoArvBin no;
    NoPos noPos;
    int i, nextRrr;

    noPos.pos = -2;

    if (cabecalho.status == '0')
    {
        printf("Cabecalho corrompido.\n");
        return noPos;
    }

    if (cabecalho.noRaiz == -1)
    {
        printf("Essa arvore esta vazia.\n");
        return noPos;
    }

    nextRrr = cabecalho.noRaiz;

    while (noPos.pos < -1)
    {
        no = LerNoArvore(arquivo, nextRrr);
        for (int i = 0; i < no.nroChavesNo; i++)
        {
            if (no.C[i] == converteNome(chave))
            {
                noPos.no = no;
                noPos.pos = i;
                break;
            }
            else if (no.C[i] > converteNome(chave))
            {
                nextRrr = no.P[i];
                break;
            }
            else if (i == (no.nroChavesNo - 1))
                nextRrr = no.P[i+1];
        }

        if (nextRrr == -1)
            noPos.pos = -1;
    }

    if (noPos.pos == -1)
        printf("Registro inexistente.\n");

    return noPos;
}

//////////////////////////////////////////////////////// FUNCOES TRABALHO 1

// Lê um registro e retorna
RegDados lerRegistro(FILE *arquivo)
{
    RegDados temp, fim;
    char dado[142], *linha;
    
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

void imprimirRegistro(RegDados registro)
{
    printf("Nome: %s\n", registro.nome);
    if(strncmp(registro.especie,"$",1)!=0&& registro.especie[0] != '\0')        
        printf("Especie: %s\n", registro.especie);
    if(strncmp(registro.tipo,"$",1)!=0&& registro.tipo[0] != '\0')
        printf("Tipo: %s\n", registro.tipo);
    if(strncmp(registro.dieta,"$",1)!=0 && registro.dieta[0] != '\0')
        printf("Dieta: %s\n", registro.dieta);
    if(strncmp(registro.habitat,"$",1)!=0 && registro.habitat[0] != '\0')
        printf("Lugar que habitava: %s\n", registro.habitat);
    if(registro.tamanho != -1)
        printf("Tamanho: %.1f m\n", registro.tamanho);
    if(registro.velocidade != -1)
        printf("Velocidade: %d %cm/h\n", registro.velocidade, registro.unidadeMedida);
    printf("\n");
}