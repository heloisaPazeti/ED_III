#include <stdlib.h>
#include <stdio.h>
#include "structs.h"

//////////////////////////////////////////////////////// LEITURA

RegDados lerRegistro(FILE *arqBin, char *arquivo)
{
    RegDados temp;
    char dado[142], *linha;

    // linha = calloc(142, sizeof(char));

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
    if(temp.removido =='0')                                // Caso o campo não tenha sido removido, a leitura dos campos é finalizada
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

RegDados lerTeclado()
{
    RegDados registro;
    char *populacao, *tamanho, *velocidade, *medidaVelocidade;

    registro = IniciarRegistroDados();              // Inicializa um registro

    // Aloca espaço para as variáveis que precisam ser manipuladas 
    populacao = calloc(10, sizeof(char));
    tamanho = calloc(10, sizeof(char));
    velocidade = calloc(10, sizeof(char));
    medidaVelocidade = calloc(10, sizeof(char));


    // Lê todos os campos do teclado
    scan_quote_string(registro.nome);
    scan_quote_string(registro.dieta);
    scan_quote_string(registro.habitat);
    scan_quote_string(populacao);
    scan_quote_string(registro.tipo);
    scan_quote_string(velocidade);
    scan_quote_string(medidaVelocidade);
    scan_quote_string(tamanho);
    scan_quote_string(registro.especie);
    scan_quote_string(registro.alimento); 

    if(strcmp(populacao, "")==0)                    // Caso o campo seja nulo, atualiza o valor da variável para -1        
        registro.populacao = -1;
    else registro.populacao = atoi(populacao);      // Caso contrário, a variável recebe o valor lido

    if(strcmp(velocidade, "")==0)                   // Caso o campo seja nulo, atualiza o valor da variável para -1
        registro.velocidade = -1;
    else registro.velocidade = atoi(velocidade);    // Caso contrário, a variável recebe o valor lido

    if(strcmp(medidaVelocidade, "")==0)            // Caso o campo seja nulo, atualiza o valor da variável para '$' 
        registro.unidadeMedida = '$';
    else registro.unidadeMedida = medidaVelocidade[0];  // Caso contrário, a variável recebe o valor lido

    if(strcmp(tamanho, "")==0)                      // Caso o campo seja nulo, atualiza o valor da variável para -1
        registro.tamanho = -1;
    else registro.tamanho = atof(tamanho);          // Caso contrário, a variável recebe o valor lido

    registro.removido = '0';                        // Certifica que registro.removido = '0'

    return registro;
}

RegCabecalho LerCabecalho(FILE *arqBin)
{
    RegCabecalho cabecalho;

    if (arqBin == NULL)
    {
        printf("Falha no processamento do arquivo [Arq. CSV == NULL]\n");
        cabecalho.status = '2';
        return cabecalho;
    }

    fseek(arqBin, 0, SEEK_SET);

    fread(&cabecalho.status, sizeof(char),1,arqBin);
    fread(&cabecalho.topo, sizeof(int),1,arqBin);
    fread(&cabecalho.proxRRN, sizeof(int),1,arqBin);
    fread(&cabecalho.nroRegRem, sizeof(int),1,arqBin);
    fread(&cabecalho.nroPagDisco, sizeof(int),1,arqBin);
    fread(&cabecalho.qttCompacta, sizeof(int),1,arqBin);

    fseek(arqBin, 1600, SEEK_SET);

    return cabecalho;
}

//////////////////////////////////////////////////////// ESCRITA

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

int EscreverCabecalho(FILE *arqBin, RegCabecalho cabecalho)
{
    fseek(arqBin, 0, SEEK_SET);                 // Para certificar de que está no local certo

    if (arqBin == NULL)
    {
        printf("Falha no processamento do arquivo [Arq. CSV == NULL]\n");
        return -1;
    }

    fwrite(&cabecalho.status, sizeof(char),1, arqBin);                              // Escreve dados
    fwrite(&cabecalho.topo, sizeof(int),1, arqBin);
    fwrite(&cabecalho.proxRRN, sizeof(int),1, arqBin);
    fwrite(&cabecalho.nroRegRem, sizeof(int),1, arqBin);
    fwrite(&cabecalho.nroPagDisco, sizeof(int),1, arqBin);
    fwrite(&cabecalho.qttCompacta, sizeof(int),1, arqBin);

    for(int i = 21; i < 1600; i++)                                                  // Adiciona "lixo"
        fwrite("$", sizeof(char), 1, arqBin);

    return 0;
}

//////////////////////////////////////////////////////// INSERÇÃO

int InserirRegistrosAdap(char *nomeArq, RegDados registro)
{
    int topo, rrn, nroReg, removidos;
    long int tamanho;
    FILE *arquivo;
    RegCabecalho cabecalho;
    RegDados registro2;

    arquivo = fopen(nomeArq, "rb+");

    if(arquivo == NULL)                                 // Caso haja erro na abertura do arquivo, emite mensagem de erro
    {
        printf("Erro de abertura do arquivo\n");
        return -1;
    }

    cabecalho = LerCabecalho(arquivo);
    if(cabecalho.status == '0')                         // Caso o cabeçalho indique inconsistência no arquivo, emite mensagem de erro
    {
        printf("Erro de abertura do arquivo\n");
        return -1;
    }

    topo = cabecalho.topo;                              // O valor inicial do topo é o indicado pelo cabeçalho
    rrn = cabecalho.proxRRN;                            // O valor inicial do rrn é o indicado pelo cabeçalho
    removidos = cabecalho.nroRegRem;                    // A quantidade inicial de registros removidos é a indicada pelo cabeçalho

    cabecalho.status = '0';                             // Muda o status do cabeçalho durante a manipulação do arquivo
    EscreverCabecalho(arquivo, cabecalho);              // Atualiza o status do cabeçalho

    if(topo == -1)                                  // Caso topo = -1, não há registros removidos e a inserção é feita ao final do arquivo
    {
        //printf("TOPO == -1\n");
        rrn++;                                      // Aumenta o rrn
        fseek(arquivo, 0, SEEK_END);                // Posiciona o cursor ao final do arquivo
        escreverRegistro(arquivo, registro, rrn);   // Escreve o registro no arquivo
    }
    else if(topo != -1)                             // Caso topo != -1, a inserção deve ser feita no registro do topo
    {
        fseek(arquivo, 1600+160*topo, SEEK_SET);    // Posiciona o cursor no topo
        registro2 = lerRegistro(arquivo, nomeArq);            // Lê o registro do topo
        rrn = topo;
        topo = registro2.encadeamento;               // Atauliza o topo para o encadeamento do registro a ser substituído
        if(registro2.removido == '1')                // Confirma que o registro havia sido removido
        {
            fseek(arquivo, -5, SEEK_CUR);           // Reposiciona o cursor 
            removidos--;                            // Diminui o número de registros removidos
        }
        substituirRegistro(arquivo, registro);     // Substitui o registro removido pelo novo

    }
    
    fseek(arquivo, 0, SEEK_END);                        // Posiciona o cursor ao final do arquivo
    tamanho = ftell(arquivo);                           // Obtém o tamanho do arquivo pela posição final do cursor
    float div = (float)tamanho/1600;                    // Calcula o número de páginas de disco
    if(div > (int)div)
        (int)div++;
    cabecalho.nroPagDisco = (int)div;                   // Atualiza o número de páginas de disco
    cabecalho.status = '1';                             // Atualiza o status do cabeçalho
    cabecalho.topo = topo;                              // Atualiza o topo do cabeçalho
    cabecalho.proxRRN = rrn;                            // Atualiza o próximo RRN do cabeçalho
    cabecalho.nroRegRem = removidos;                    // Atualiza o número de registros removidos do cabeçalho

    EscreverCabecalho(arquivo, cabecalho);              // Escreve o cabeçalho com os dados atualizados
    
    fclose(arquivo);
    
    //binarioNaTela(nomeArq);
    if(cabecalho.topo == -1) return rrn-1;
    return rrn;
}


//////////////////////////////////////////////////////// IMPRESSÃO

void imprimeRegistro(RegDados registro)
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

//////////////////////////////////////////////////////// HELPERS

int tamanhoString(char *string)
{
    int i=0;
    while(string[i]!='\0')
        i++;

    return i;
}

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