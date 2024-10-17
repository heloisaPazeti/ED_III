#include "funcoesAuxiliares.h"
#include "structs.h"

//////////////////////////////////////////////////////// FUNCOES DE LEITURA

CabecalhoArvBin LerCabecalhoArvore(char *arquivo)
{
    CabecalhoArvBin cabecalho = CriarCabecalhoArvBin(); // Inicializa Arvore
    FILE *arqBin = fopen(arquivo, "rb");

    if (ChecarIntegridadeArquivo(arqBin, arquivo) < 0) return cabecalho;
    
    fread(&cabecalho.status, sizeof(char), 1, arqBin);
    fread(&cabecalho.noRaiz, sizeof(int), 1, arqBin);
    fread(&cabecalho.RRNproxNo, sizeof(int), 1, arqBin);
    fread(cabecalho.lixo, sizeof(char), 83, arqBin);

    fclose(arqBin);
    return cabecalho;
}

// Le o nó no rrn especifico
NoArvBin LerNoArvore(char *arquivo, int rrn)
{
    NoArvBin no = CriarNo();                    // Inicializa no
    FILE *arqBin = fopen(arquivo, "rb");
    int i;

    if (ChecarIntegridadeArquivo(arqBin, arquivo) < 0) return no;
        
    fseek(arqBin, (rrn * tamNo) + tamCabecalho, SEEK_SET);

    fread(&no.folha, sizeof(char), 1, arqBin);
    fread(&no.nroChavesNo, sizeof(int), 1, arqBin);
    fread(&no.RRNdoNo, sizeof(int), 1, arqBin);

    for (i = 0; i < no.nroChavesNo; i++)        // Le todos os Pi, Ci, PRi
    {
        fread(&no.P[i], sizeof(int), 1, arqBin);
        fread(&no.C[i], sizeof(long), 1, arqBin);
        fread(&no.PR[i], sizeof(long), 1, arqBin);
    }

    fread(&no.P[i], sizeof(int), 1, arqBin);    // Le o ponteiro final
    fclose(arqBin);
    return no;
}

//////////////////////////////////////////////////////// FUNCOES DE BUSCA

// Retorna uma struct com no + pos na arvore na qual está / deveria estar o registro
// Se houver retorna o no + rrn
// Se não houver retorna na posicao -1
// Retorna -2 na posicao se der algum erro
NoPos BuscarNoArvore(char *arquivo, char *chave)
{
    CabecalhoArvBin cabecalho = LerCabecalhoArvore(arquivo);
    NoArvBin no;
    NoPos noPos;
    int i, nextRrr;

    noPos.pos = -2;

    if ((ChecarCabecalho(cabecalho) < 0) || (ChecarArvoreVazia(cabecalho) < 0)) return noPos;

    nextRrr = cabecalho.noRaiz;                     // Pega o rrn da raiz
    while (noPos.pos < -1)                          // Enquanto não encontrar o fim ou a chave
    {
        no = LerNoArvore(arquivo, nextRrr);         // Le o no
        for (int i = 0; i < no.nroChavesNo; i++)    // Checa todas as chaves no no
        {
            if (no.C[i] == converteNome(chave))     // Se encontrou
            {
                noPos.no = no;                      // Seleciona o no
                noPos.pos = i;                      // Seleciona a pos
                break;
            }
            else if (no.C[i] > converteNome(chave)) // Se a chave atual for maior que a chave procurada
            {
                nextRrr = no.P[i];                  // "Desce" para o no Pi
                break;
            }
            else if (i == (no.nroChavesNo - 1))     // Se chegar ao final da arvore
                nextRrr = no.P[i+1];                // "Desce" para o no Pi+1
        }

        if (nextRrr == -1)                          // Caso nao tenha encontrado
            noPos.pos = -1;
    }

    if (noPos.pos == -1)                            // Registro não existe
        printf("Registro inexistente.\n");

    return noPos;
}

//////////////////////////////////////////////////////// CHECAGENS

int ChecarCabecalho(CabecalhoArvBin cabecalho)
{
    if (cabecalho.status == '0')
    {
        printf("Cabecalho corrompido.\n");
        return -1;
    }

    return 0;
}

int ChecarArvoreVazia(CabecalhoArvBin cabecalho)
{
    if (cabecalho.noRaiz == -1)
    {
        printf("Essa arvore esta vazia.\n");
        return -1;
    }

    return 0;
}

int ChecarIntegridadeArquivo(FILE *arquivo, char *nomeArq)
{
    if(arquivo == NULL)
    {
        printf("Foi impossível de abrir o arquivo: %s\n", nomeArq);
        return -1;
    }

    return 0;
}

//////////////////////////////////////////////////////// FUNCOES TRABALHO 1

// Lê um registro e retorna
RegDados lerRegistro(FILE *arqBin, char *arquivo)
{
    RegDados temp, fim;
    char dado[142], *linha;

    if(ChecarIntegridadeArquivo(arqBin, arquivo) < 0) return temp;

    if(fread(&temp.removido, sizeof(char),1,arqBin)==0)    // Caso a leitura falhe, o campo de remoção recebe um valor logicamente inválido
    {
        temp.removido = '2';
        return temp;
    }    
    if(temp.removido == '1')                                // Caso o registro tenha sido removido, não completa a leitura e retorna 
    {
        fread(&temp.encadeamento, sizeof(int),1,arqBin);
        return temp;
    }
    if(temp.removido != '1')                                // Caso o campo não tenha sido removido, a leitura dos campos é finalizada
    {
        fread(&temp.encadeamento, sizeof(int),1,arqBin);
        fread(&temp.populacao, sizeof(int),1,arqBin);
        fread(&temp.tamanho, sizeof(float),1,arqBin);
        fread(&temp.unidadeMedida, sizeof(char),1,arqBin);
        fread(&temp.velocidade, sizeof(int), 1,arqBin);
        if(fread(dado, sizeof(char), 142, arqBin)==0)      // Caso a leitura falhe, o campo de remoção recebe um valor logicamente inválido
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