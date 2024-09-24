#include "funcoesBuscaManipulacao.h"

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

// Substitui um registro apagado por um lido anteriormente
void substituirRegistro(FILE *arquivo, RegDados registro)
{
    char delim = '#';
    fwrite(&registro.removido, sizeof(char),1, arquivo); 
    fwrite(&registro.encadeamento, sizeof(int),1, arquivo);
    fwrite(&registro.populacao, sizeof(int),1, arquivo);
    fwrite(&registro.tamanho, sizeof(float),1, arquivo);
    fwrite(&registro.unidadeMedida, sizeof(char),1, arquivo);
    fwrite(&registro.velocidade, sizeof(int),1, arquivo);
    fwrite(registro.nome, sizeof(char), strlen(registro.nome), arquivo);
    fwrite(&delim, sizeof(char),1, arquivo);
    fwrite(registro.especie, sizeof(char), strlen(registro.especie), arquivo);
    fwrite(&delim, sizeof(char),1, arquivo);
    fwrite(registro.habitat, sizeof(char), strlen(registro.habitat), arquivo);
    fwrite(&delim, sizeof(char),1, arquivo);
    fwrite(registro.tipo, sizeof(char), strlen(registro.tipo), arquivo);
    fwrite(&delim, sizeof(char),1, arquivo);
    fwrite(registro.dieta, sizeof(char), strlen(registro.dieta), arquivo);
    fwrite(&delim, sizeof(char),1, arquivo);
    fwrite(registro.alimento, sizeof(char), strlen(registro.alimento), arquivo);
    fwrite(&delim, sizeof(char),1, arquivo);
}

// Escreve um registro no arquivo de acordo com a struct passada
int escreverRegistro(FILE *arqBin, RegDados novoRegisto, int quantReg)
{
    long int posicaoAtual;
    int posicaoFinal;
    char delim = '#';
    int tamNome, tamEspecie, tamHabitat, tamTipo, tamDieta, tamAlimento;
    
    if (arqBin == NULL)
    {
        printf("Falha no processamento do arquivo [Arq. CSV == NULL]\n");
        return -1;
    }

    tamNome = tamanhoString(novoRegisto.nome);
    tamEspecie = tamanhoString(novoRegisto.especie);
    tamHabitat = tamanhoString(novoRegisto.habitat);
    tamTipo = tamanhoString(novoRegisto.tipo);
    tamDieta = tamanhoString(novoRegisto.dieta);
    tamAlimento = tamanhoString(novoRegisto.alimento);

    fwrite(&novoRegisto.removido, sizeof(char),1, arqBin);                          // Escreve os dados
    fwrite(&novoRegisto.encadeamento, sizeof(int),1, arqBin);
    fwrite(&novoRegisto.populacao, sizeof(int),1, arqBin);
    fwrite(&novoRegisto.tamanho, sizeof(float),1, arqBin);
    fwrite(&novoRegisto.unidadeMedida, sizeof(char),1, arqBin);
    fwrite(&novoRegisto.velocidade, sizeof(int),1, arqBin);
    fwrite(novoRegisto.nome, sizeof(char), tamNome, arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.especie, sizeof(char), tamEspecie, arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.habitat, sizeof(char), tamHabitat, arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.tipo, sizeof(char), tamTipo, arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.dieta, sizeof(char), tamDieta, arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);
    fwrite(novoRegisto.alimento, sizeof(char), tamAlimento, arqBin);
    fwrite(&delim, sizeof(char),1, arqBin);

    posicaoAtual = ftell(arqBin);                                                   // ftell -> posição atual
    posicaoFinal = 1600 + (quantReg*160);                                           // Calcula posição no registro

    for(int i = posicaoAtual; i < posicaoFinal; i++)                                // Adiciona lixo
        fwrite("$", sizeof(char), 1, arqBin);
}