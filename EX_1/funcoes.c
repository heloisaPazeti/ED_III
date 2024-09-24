#include "funcoes.h"
#include "funcoesFornecidas.h"
#include "funcoesAuxiliares.h"
#include "funcoesBuscaManipulacao.h"
#include "structs.h"

///////////////////////////////////////////////////////////////// ESCREVER NO ARQUIVO (1)

// Escreve um arquivo binario a partir de um CSV
// Escreve dados + cabeçalho
int EscreverArquivo(char *nomeCSV)
{
    int  i;
    int  qtdePagDisco, quantReg = 0;
    char nomeBin[15];
    char *token, *tofree, *str;
    char delim = '#';
    char line[tamRegistro];
    FILE *arqBin; 
    FILE *arqCSV = fopen(nomeCSV, "r");
    RegCabecalho cabecalho;

    scanf("%s", nomeBin);
    arqBin = fopen(nomeBin, "wb");


    if (arqCSV == NULL || arqBin == NULL)
    {
        printf("Falha no processamento do arquivo [Arq. CSV == NULL]\n");
        return -1;
    }

    cabecalho = IniciarCabecalho();                    // Inicializa novo Cabeçalho
    EscreverCabecalho(arqBin, cabecalho);              // Escreve no arquivo os dados iniciais do cabeçalho

    if(cabecalho.status == '2')
    {
        printf("Falha ao escrever cabeçalho.\n");
        return -1;
    }
        
    fgets(line, tamRegistro, arqCSV);                   // Le header do arquivo
    while(!feof(arqCSV))
    {
        RegDados novoReg = IniciarRegistroDados();      // Inicializa novo registro
        quantReg++;                                     // Adiciona um registro

        fgets(line, tamRegistro, arqCSV);               // Pega a linha do registro do csv
        int len = strlen(line);
        if(line[len-1] == '\n')                         // Retira o \n
            line[len-1] = 0;

        tofree = str = strdup(line);                    // Reserva espaço de armazenamento

        for(int i = 0; i < 10; i++)                     // Lê cada campo de registro
        {
            if(i < 9)
                token = strsep(&str, ",");          
            else
                token = strsep(&str, "\r");             // O último campo tem como separador o \r

            switch (i)
            {
                case 0:
                    novoReg.nome = token;
                    break;

                case 1:
                    novoReg.dieta = token;
                    break;

                case 2:
                    novoReg.habitat = token;
                    break;

                case 3:
                    if(*token != '\0')                  // Caso token seja vazio
                        novoReg.populacao = atoi(token);
                    break;

                case 4:
                    novoReg.tipo = token;
                    break;

                case 5:
                    if(*token != '\0')
                        novoReg.velocidade = atoi(token);
                    break;

                case 6:
                    if(*token != '\0')
                        novoReg.unidadeMedida = token[0];
                    break;

                case 7:
                    if(*token != '\0')
                        novoReg.tamanho = atof(token);
                    break;

                case 8:
                    novoReg.especie = token;
                    break;

                case 9:
                    novoReg.alimento = token;
                    break;

                default:
                    break;
            }
        }

        escreverRegistro(arqBin, novoReg, quantReg);        // Escreve o novo registro no arquivo binario
    }

    qtdePagDisco = (1+(quantReg/10));                       // Calcula quantidade de páginas de disco
    if(qtdePagDisco*10 < quantReg+10)
        qtdePagDisco++;

    fseek(arqBin, 13, SEEK_SET);                            // Altera dados do cabeçalho
    fwrite(&qtdePagDisco, sizeof(int),1, arqBin);
    fseek(arqBin, 5, SEEK_SET);
    fwrite(&quantReg, sizeof(int), 1, arqBin);
    fseek(arqBin, 0, SEEK_SET);
    fwrite("1", sizeof(char),1, arqBin);

    fclose(arqCSV);
    fclose(arqBin);
    binarioNaTela(nomeBin);

    return 0;
}

///////////////////////////////////////////////////////////////// PRINTAR REGISTROS (2)

// Escreve na tela todos os registros presentes no arquivo
int EscreverRegistros(char *nomeArq)
{
    int atualRRN = 0;
    FILE *arqBin;
    RegDados novoRegistro;
    RegCabecalho cabecalho;

    arqBin = fopen(nomeArq, "rb");
    if(arqBin == NULL)
    {
        printf("Erro na abertura do arquivo \n");
        return -1;
    }

    cabecalho = LerCabecalho(arqBin);                   // Lê o cabeçalho
    if(cabecalho.status == '0')                         // Caso o arquivos esteja inconsistente (status = 0), emite mensagem de erro
    {
        printf("Erro na abertura do arquivo");
        return -1;
    }

    cabecalho.status = '0';                             // Atualiza o status do arquivo durante a manipulação do arquivo

    EscreverCabecalho(arqBin, cabecalho);               // Reescreve o cabeçalho com os novos dados
    
    novoRegistro = lerRegistro(arqBin);                 // Lê o primeiro registro
    atualRRN++;                                         // Aumenta o RRN atual

    while(((atualRRN) <= cabecalho.proxRRN))            // Verifica se acabou o arquivo
    {
        if(novoRegistro.removido == '0')                // Se for removido ignora
            imprimirRegistro(novoRegistro);

        if(novoRegistro.removido == '1')
            fseek(arqBin, 155, SEEK_CUR);
        
        novoRegistro = lerRegistro(arqBin);             // Lê o próximo
        atualRRN++;
    }

    printf("Numero de paginas de disco: %d\n\n", cabecalho.nroPagDisco);      // Printa quantidade de páginas de disco

    cabecalho.status = '1';                             // Atualiza o status do cabeçalho
    EscreverCabecalho(arqBin, cabecalho);

    fclose(arqBin);
    return 0;

}


///////////////////////////////////////////////////////////////// BUSCAR REGISTRO (3)

// Realiza buscas conforme o campo e o valor especificados pelo usuário
// Escreve os registros correspondentes e o número de páginas de disco acessadas na busca
int BuscarRegistros(char *nomeArq)
{
    int n, i;
    int valorCampoInt;
    int tipoPesquisa, numPag, encontrou;
    float valorCampoFloat;
    char nomeCampo[15], valorCampo[61], aux1[1];
    RegDados reg;
    RegCabecalho cabecalho;
    FILE *arquivo;

    arquivo = fopen(nomeArq, "rb");                     // Inicializa o arquivo em modo de leitura
    if(arquivo == NULL)                                 // Se a inicialização falhar, emite mensagem de erro
    {
        printf("Erro na abertura do arquivo \n");
        return -1;
    }

    cabecalho = LerCabecalho(arquivo);                  // Lê o cabeçalho do arquivo
    if(cabecalho.status == '0')                         // Caso o arquivos esteja inconsistente (status = 0), emite mensagem de erro
    {
        printf("Erro na abertura do arquivo");
        return -1;
    }

    cabecalho.status = '0';                                     // Atualiza o status do arquivo durante a manipulação do arquivo
    
    EscreverCabecalho(arquivo, cabecalho);                      // Reescreve o cabeçalho com os novos dados
    encontrou = 0;                                      // Inicializa o número de registros encontrados

    numPag = cabecalho.nroPagDisco;                     // Obtém o número de páginas de disco a partir do cabeçalho
    scanf("%d", &n);                                    // Lê o número de buscas a serem realizadas
    for(i=0; i<n; i++)
    {
        fseek(arquivo, 1600, SEEK_SET);                 // Posiciona o cursor após o cabeçalho
        encontrou = 0;
        scanf("%s", nomeCampo);                         // Lê o nome do campo

        tipoPesquisa = definirTipo(nomeCampo);          // Define um inteiro para o tipo de pesquisa

        if(tipoPesquisa == 1 || tipoPesquisa == 4)      // Caso o campo seja um inteiro
        {
            scanf("%d", &valorCampoInt);
        }

        else if(tipoPesquisa == 2)                       // Caso o campo seja um float
        {
            scanf("%f", &valorCampoFloat);
        }

        else                                               // Caso o campo seja uma string
        {
            scan_quote_string(valorCampo);                      
        }
        printf("Busca %d\n", i+1); 

        while(1)
        {                        
            reg = lerRegistro(arquivo);                     // Lê o registro no arquivo binário

            if (reg.removido=='2') break;                   // removido == '2' é uma flag criada em "lerRegistro" sinalizando o fim do arquivo
            if (reg.removido=='1') 
                fseek(arquivo, 155, SEEK_CUR);              // Caso o registro tenha sido removido, avança o cursor

            if(reg.removido == '0')                         // Caso o registro não tenha sido removido
            {
                switch(tipoPesquisa)                        // Realiza a busca conforme o campo selecionado. Caso encontre um registro correspondente, imprime-o e aumenta a variável de registros encontrados
                {                                           
                    case 1:
                        if(reg.populacao == valorCampoInt)
                        {
                            imprimirRegistro(reg);
                            ++encontrou;
                        }
                        break;
                    case 2:
                        if(reg.tamanho == valorCampoFloat)
                        {
                            imprimirRegistro(reg);
                            ++encontrou;
                        }
                        break;
                    case 3:
                        if(strncmp(valorCampo, &reg.unidadeMedida, 1)==0)
                        {
                            imprimirRegistro(reg);
                            ++encontrou;
                        }
                        break;
                    case 4:
                        if(reg.velocidade == valorCampoInt)
                        {
                            imprimirRegistro(reg);
                            ++encontrou;
                        }
                        break;
                    case 5:
                        if(strcmp(valorCampo, reg.nome)==0)
                        {
                            imprimirRegistro(reg);
                            ++encontrou;
                        }
                        break;
                    case 6:
                        if(strcmp(valorCampo, reg.especie)==0)
                        {
                            imprimirRegistro(reg);
                            ++encontrou;
                        }
                        break;
                    case 7:
                        if(strcmp(valorCampo, reg.habitat)==0)
                        {
                            imprimirRegistro(reg);
                            ++encontrou;
                        }
                        break;
                    case 8:
                        if(strcmp(valorCampo, reg.tipo)==0)
                        {
                            imprimirRegistro(reg);
                            ++encontrou;
                        }
                        break;
                    case 9:
                        if(strcmp(valorCampo, reg.dieta)==0)
                        {
                            imprimirRegistro(reg);
                            ++encontrou;
                        }
                        break;
                    case 10:
                        if(strcmp(valorCampo, reg.alimento)==0)
                        {
                            imprimirRegistro(reg);
                            ++encontrou;
                        }
                        break;
                    default:                                    // Caso o campo tenha valor inválido
                        printf("Erro\n");
                        break;
                }
            }
        }

        if(encontrou==0) printf("Registro inexistente.\n\n");   // Caso nenhum registro tenha sido encontrado, informa ao usuário

        printf("Numero de paginas de disco: %d\n\n", numPag);   // Mostra o número de páginas de disco
    }

    cabecalho.status = '1';                             // Atualiza o status do cabeçalho
    EscreverCabecalho(arquivo, cabecalho);

    fclose(arquivo);

    return 0;
}

///////////////////////////////////////////////////////////////// REMOVER REGISTRO (4)

// Elimina registros segundo critérios escolhidos pelo usuário
// Atualiza os dados do arquivo
int RemoverRegistros(char *nomeArq)
{
    int n, i, proxRRN;
    int valorCampoInt, tipoPesquisa, topo, offset, remocoes;
    float valorCampoFloat;
    char nomeCampo[15], valorCampo[61];
    RegDados reg;
    RegCabecalho cabecalho;

    FILE *arquivo;

    arquivo = fopen(nomeArq, "rb+");                            // Abre o arquivo em modo de leitura e escrita
    if(arquivo == NULL)                                         // Caso haja erro na abertura do arquivo, emite mensagem de erro
    {
        printf("Erro na abertura do arquivo \n");
        return -1;
    }


    cabecalho = LerCabecalho(arquivo);                          // Lê o cabeçalho do arquivo
    if(cabecalho.status == '0')                                 // Caso o cabeçalho indique inconsistência no arquivo, emite mensagem de erro
    {
        printf("Erro na abertura do arquivo");
        return -1;
    }

    cabecalho.status = '0';                                     // Atualiza o status do arquivo durante a manipulação do arquivo
    remocoes = cabecalho.nroRegRem;                             // O número inicial de remoções é o indicado no cabeçalho
    topo = cabecalho.topo;                                      // O topo incial é o indicado pelo cabeçalho

    EscreverCabecalho(arquivo, cabecalho);                      // Reescreve o cabeçalho com os novos dados

    scanf("%d", &n);                                            // Lê o número de dados a serem removidos


    for(i=0; i<n; i++)                        
    {
        fseek(arquivo, 1600, SEEK_SET);                         // Posiciona o cursor para o começo dos dados no arquivo
        proxRRN=-1;                                             // Inicializa a variável que indica o RRN do próximo removido

        scanf("%s", nomeCampo);                                 // Lê o nome do campo a ser removido

        tipoPesquisa = definirTipo(nomeCampo);                  // Define um inteiro de acordo com o campo a ser removido

        if(tipoPesquisa == 1 || tipoPesquisa == 4)              // Caso o campo a ser removido seja um inteiro
        {
            scanf("%d", &valorCampoInt);
        }

        else if(tipoPesquisa == 2)                              // Caso o campo a ser removido seja um float
        {
            scanf("%f", &valorCampoFloat);
        }

        else                                                    // Caso o campo a ser removido seja uma string
            scan_quote_string(valorCampo);

        while(1)
        {
            proxRRN++;                                          // Aumenta o valor do RRN conforme percorre os registros
            reg = lerRegistro(arquivo);                         // Lê o registro
            if (reg.removido=='2') break;                       // reg.removido = '2' indica fim do arquivo (função lerRegistro), sai do while
            if (reg.removido=='1')                              // Caso o registro tenha sido removido, reposiciona o cursor ao final dele
                fseek(arquivo, 155, SEEK_CUR);
            if(reg.removido == '0')                             // Caso o registro não tenha sido removido, realiza a busca conforme o especificado
            {                                                   // Quando encontra um registro, elimina-o, atualiza o valor do topo para o RRN correspondente e aumenta o número de remoções
                switch(tipoPesquisa)
                {
                    case 1:
                        if(reg.populacao == valorCampoInt)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = proxRRN;
                            remocoes++;
                        }
                        break;

                    case 2:
                        if(reg.tamanho == valorCampoFloat)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = proxRRN;
                            remocoes++;
                        }
                        break;
                    case 3:
                        if(strcmp(valorCampo, &reg.unidadeMedida)==0)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = proxRRN;
                            remocoes++;
                        }
                        break;
                    case 4:
                        if(reg.velocidade == valorCampoInt)
                        {
                            eliminarRegistro(arquivo,topo);
                            topo = proxRRN;
                            remocoes++;
                        }
                        break;
                    case 5:
                        if(strcmp(valorCampo, reg.nome)==0)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = proxRRN;
                            remocoes++;
                        }
                        break;
                    case 6:
                        if(strcmp(valorCampo, reg.especie)==0)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = proxRRN;
                            remocoes++;
                        }
                        break;
                    case 7:
                        if(strcmp(valorCampo, reg.habitat)==0)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = proxRRN;
                            remocoes++;
                        }
                        break;
                    case 8:
                        if(strcmp(valorCampo, reg.tipo)==0)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = proxRRN;
                            remocoes++;
                        }
                        break;
                    case 9:
                        if(strcmp(valorCampo, reg.dieta)==0)
                        {
                            eliminarRegistro(arquivo,topo);
                            topo = proxRRN;
                            remocoes++;
                        }
                        break;
                    case 10:
                        if(strcmp(valorCampo, reg.alimento)==0)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = proxRRN;
                            remocoes++;
                        }
                        break;
                    default:                                    // Se o valor do campo for inválido, emite mensagem de erro
                        printf("Erro\n");
                        break;
                }
            }
        }
    }

    cabecalho.topo = topo;                                      // Atualiza o valor do topo do cabeçalho
    cabecalho.nroRegRem = remocoes;                             // Atualiza o número de remoções no cabeçalho
    cabecalho.status = '1';                                     // Atualiza o status do arquivo

    EscreverCabecalho(arquivo, cabecalho);                      // Reescreve o cabeçalho com as novas informações
    fclose(arquivo);
    binarioNaTela(nomeArq);

    return 0;
}

///////////////////////////////////////////////////////////////// INSERIR REGISTROS (5)

// Insere registros em um arquivo binário seguindo o conceito de lista de registros logicamente removidos
int InserirRegistros(char *nomeArq)
{
    int n, i, topo, rrn, nroReg, removidos;
    long int tamanho;
    FILE *arquivo;
    RegCabecalho cabecalho;
    RegDados registro, registro2;

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
        binarioNaTela(nomeArq);
        return -1;
    }

    topo = cabecalho.topo;                              // O valor inicial do topo é o indicado pelo cabeçalho
    rrn = cabecalho.proxRRN;                            // O valor inicial do rrn é o indicado pelo cabeçalho
    removidos = cabecalho.nroRegRem;                    // A quantidade inicial de registros removidos é a indicada pelo cabeçalho

    cabecalho.status = '0';                             // Muda o status do cabeçalho durante a manipulação do arquivo
    

    EscreverCabecalho(arquivo, cabecalho);              // Atualiza o status do cabeçalho

    scanf("%d", &n);                                    // Lê a quantidade de inserções a serem feitas

    for(i=0; i<n; i++)                                  
    {
        if(topo == -1)                                  // Caso topo = -1, não há registros removidos e a inserção é feita ao final do arquivo
        {
            registro = IniciarRegistroDados();          // Inicializa um novo registro
            registro = lerDadosDoTeclado();             // Lê o registro do teclado
            rrn++;                                      // Aumenta o rrn
            fseek(arquivo, 0, SEEK_END);                // Posiciona o cursor ao final do arquivo
            escreverRegistro(arquivo, registro, rrn);   // Escreve o registro no arquivo
        }
        else if(topo != -1)                             // Caso topo != -1, a inserção deve ser feita no registro do topo
        {
            fseek(arquivo, 1600+160*topo, SEEK_SET);    // Posiciona o cursor no topo
            registro = lerRegistro(arquivo);            // Lê o registro do topo
            topo = registro.encadeamento;               // Atauliza o topo para o encadeamento do registro a ser substituído

            registro2 = IniciarRegistroDados();         // Inicializa um segundo registro de dados
            registro2 = lerDadosDoTeclado();            // Lê o registro de dados do teclado
            if(registro.removido == '1')                // Confirma que o registro havia sido removido
            {
                fseek(arquivo, -5, SEEK_CUR);           // Reposiciona o cursor 
                removidos--;                            // Diminui o número de registros removidos
            }
            substituirRegistro(arquivo, registro2);     // Substitui o registro removido pelo novo

        }
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
    binarioNaTela(nomeArq);
    return 0;
}
///////////////////////////////////////////////////////////////// COMPACTADOR (6)

// Cria novo arquivo removendo registros excluidos
// Adiciona novas informações ao cabeçalho
int Compactador(char *nomeArq)
{
    int atualRRN = 0, qtdePagDisco, qtdeRRN = 0;
    char *nomeArqFinal = "binario.bin";
    FILE *arqBinOriginal, *arqBinFinal;
    RegCabecalho cabecalho;
    RegDados novoRegistro;

    arqBinOriginal = fopen(nomeArq, "rb");
    arqBinFinal = fopen(nomeArqFinal, "wb");

    cabecalho = LerCabecalho(arqBinOriginal);           // Lê o cabeçalho

    if(cabecalho.status == '0')                         // Caso o arquivos esteja inconsistente (status = 0), emite mensagem de erro
    {
        printf("Erro na abertura do arquivo");
        return -1;
    }

    cabecalho.status = '0';                                     // Atualiza o status do arquivo durante a manipulação do arquivo
    EscreverCabecalho(arqBinFinal, cabecalho);                  // Escreve no novo arquivo
    novoRegistro = lerRegistro(arqBinOriginal);                 // Lê o primeiro registro
    atualRRN++;                                                 // Atualiza total de registros

    while(((atualRRN) <= cabecalho.proxRRN))                    // Verifica se acabou o arquivo
    {
        if(novoRegistro.removido == '0')                        // Se for removido ignora
        {
            qtdeRRN++;                                          // Adiciona um novo registro
            escreverRegistro(arqBinFinal, novoRegistro, qtdeRRN);
        }
            
    
        novoRegistro = lerRegistro(arqBinOriginal);             // Lê o próximo

         if(novoRegistro.removido == '1')
            fseek(arqBinOriginal, 155, SEEK_CUR);

        atualRRN++;
    }

    qtdePagDisco = (1+(qtdeRRN/10));                            // Calcula quantidade de páginas de disco
    if(qtdePagDisco*10 < qtdeRRN+10)
        qtdePagDisco++;

    cabecalho.status = '1';                                     // Atualiza cabeçalho
    cabecalho.topo = -1;
    cabecalho.proxRRN = qtdeRRN;
    cabecalho.nroRegRem = 0;
    cabecalho.nroPagDisco = qtdePagDisco;
    cabecalho.qttCompacta += 1;

    EscreverCabecalho(arqBinFinal, cabecalho);                  // Altera cabeçalho no arquivo final

    fclose(arqBinOriginal);
    fclose(arqBinFinal);
    binarioNaTela(nomeArqFinal);                               // Escreve binario
    return 0;
}