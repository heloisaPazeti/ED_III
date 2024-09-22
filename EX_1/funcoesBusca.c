#include "funcoesBusca.h"

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

// Compara o nome do campo aos possíveis critérios de busca e retorna um inteiro
// Busca facilitar as demais funções
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

// Recebe um registro e mostra seus campos não vazios
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

// Elimina registros, mantendo apenas o sinalizador de remoção e um campo indicando o próximo campo vazio
void eliminarRegistro(FILE* arquivo, int encadeamento)
{
    char lixo = '$';
    int i;
    char removido = '1';
 

    fseek(arquivo, -160, SEEK_CUR);
    fwrite(&removido, 1, 1, arquivo);
    fwrite(&encadeamento, 4, 1, arquivo);
    for(i=0; i<155; i++)
    {
        fwrite(&lixo, 1, 1, arquivo);
    }
    
    
}