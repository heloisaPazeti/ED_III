#include "funcoes.h"
#include "funcoesFornecidas.h"
#include "funcoesAuxiliares.h"
#include "funcoesBusca.h"
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

        EscreverRegistro(arqBin, novoReg, quantReg);        // Escreve o novo registro no arquivo binario
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
    novoRegistro = lerRegistro(arqBin);                 // Lê o primeiro registro
    atualRRN++;                                         // Aumenta o RRN atual

    while(((atualRRN) <= cabecalho.proxRRN))            // Verifica se acabou o arquivo
    {
        if(novoRegistro.removido != '1')                // Se for removido ignora
            imprimirRegistro(novoRegistro);
        
        novoRegistro = lerRegistro(arqBin);             // Lê o próximo
        atualRRN++;
    }

    printf("\nNumero de paginas de disco: %d", cabecalho.nroPagDisco);      // Printa quantidade de páginas de disco
    fclose(arqBin);
    return 0;

}

///////////////////////////////////////////////////////////////// BUSCAR REGISTRO (3)

// Realiza buscas conforme o campo e o valor especificados pelo usuário
// Escreve os registros correspondentes e o número de páginas de disco acessadas na busca
int BuscarRegistros(char *nomeArq)
{
    int n;
    int i;
    int j=0;
    int valorCampoInt;
    int tipoPesquisa, numPag;
    int offset, encontrou, aux2;
    float valorCampoFloat;
    char nomeCampo[15], valorCampo[61], aux1[1];
    RegDados reg;
    FILE *arquivo;

    arquivo = fopen(nomeArq, "rb");                     // Inicializa o arquivo em modo de leitura
    if(arquivo == NULL)                                 // Se a inicialização falhar, emite mensagem de erro
    {
        printf("Erro na abertura do arquivo \n");
        return -1;
    }

    encontrou = 0;  
    
    numPag = numPagDisco(arquivo);                      // Obtém o número de páginas de disco a partir do cabeçalho
    scanf("%d", &n);                                    // Lê o número de buscas a serem realizadas
    for(i=0; i<n; i++)
    {
        fseek(arquivo, 1600, SEEK_SET);                 // Posiciona o cursor após o cabeçalho
        j=0;
        encontrou = 0;
        scanf("%s", nomeCampo);                         // Lê o nome do campo

        if(strncmp(nomeCampo, "populacao",3)==0 || strncmp(nomeCampo, "velocidade",3)==0)   // Leitura caso o valor do campo seja um inteiro
        {
            scanf("%d", &valorCampoInt);
        }

        else if(strncmp(nomeCampo, "tamanho",3)==0)      // Leitura caso o valor do campo seja um float
        {
            scanf("%f", &valorCampoFloat);
        }

        else
            scan_quote_string(valorCampo);                 // Leitura caso o valor do campo seja uma string
        
        tipoPesquisa = definirTipo(nomeCampo);              // Define o tipo de pesquisa a ser realizada conforme o nome do campo
        printf("Busca %d\n", i+1); 

        while(1)
        {
            offset = 1600+j*160;                            // Calcula o offset ligado à busca
            reg = lerRegistro(arquivo);                     // Lê o registro no arquivo binário
            
            if (reg.removido=='2') break;                   // removido == '2' é uma flag criada em "lerRegistro" sinalizando o fim do arquivo
            if (reg.removido=='1') 
                fseek(arquivo, offset, SEEK_SET);           // Caso o registro tenha sido removido, avança o cursor

            if(reg.removido == '0')                         // Caso o registro não tenha sido removido
            {
                switch(tipoPesquisa)                        // Realiza a busca conforme o campo selecionado. Caso encontre um registro 
                                                            // correspondente, imprime-o e aumenta a variável de registros encontrados
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
                    default:
                        printf("Erro\n");
                        break;
                }
            }
            
            j++;
        }
       
        if(encontrou==0) printf("Registro inexistente.\n\n");   // Caso nenhum registro tenha sido encontrado, informa ao usuário

        printf("Numero de paginas de disco: %d\n\n", numPag);   // Mostra o número de páginas de disco
    }

    return 0;
}

///////////////////////////////////////////////////////////////// REMOVER REGISTRO (4)

int RemoverRegistros(char *nomeArq)
{
    int n, i, aux, j;
    int valorCampoInt, tipoPesquisa, topo, offset, remocoes;
    float valorCampoFloat;
    char nomeCampo[15], valorCampo[61], aux1[1];
    RegDados reg;
    RegCabecalho cabecalho;

    FILE *arquivo;

    arquivo = fopen(nomeArq, "rb+");
    if(arquivo == NULL)
    {
        printf("Erro na abertura do arquivo \n");
        return -1;
    }
   

    cabecalho = LerCabecalho(arquivo);
    if(cabecalho.status == '0')
    {
        printf("Erro na abertura do arquivo");
        return -1;
    }

    cabecalho.status = '0';
    remocoes = cabecalho.nroRegRem;
    topo = cabecalho.topo;

    aux = EscreverCabecalho(arquivo, cabecalho);

    scanf("%d", &n);


    for(i=0; i<n; i++)
    {
        fseek(arquivo, 1600, SEEK_SET);

        j=-1;

        scanf("%s", nomeCampo);

        if(strncmp(nomeCampo, "populacao",3)==0 || strncmp(nomeCampo, "velocidade",3)==0)
        {
            scanf("%d", &valorCampoInt);
        }

        else if(strncmp(nomeCampo, "tamanho",3)==0)
        {
            scanf("%f", &valorCampoFloat);
        }

        else
            scan_quote_string(valorCampo);
        
        tipoPesquisa = definirTipo(nomeCampo);

        while(1)
        {
            offset = 1600+j*160;
            reg = lerRegistro(arquivo);
            if (reg.removido=='2') break;
            if (reg.removido=='1') fseek(arquivo, offset, SEEK_SET);

            if(reg.removido == '0')
            {
                switch(tipoPesquisa)
                {
                    case 1:
                        if(reg.populacao == valorCampoInt)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = j;
                            remocoes++;
                        }
                        break;
                    
                    case 2:
                        if(reg.tamanho == valorCampoFloat)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = j;
                            remocoes++;
                        }
                        break;
                    case 3:
                        if(strcmp(valorCampo, &reg.unidadeMedida)==0)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = j;
                            remocoes++;
                        }
                        break;
                    case 4:
                        if(reg.velocidade == valorCampoInt)
                        {
                            eliminarRegistro(arquivo,topo);
                            topo = j;
                            remocoes++;
                        }
                        break;
                    case 5:
                        if(strcmp(valorCampo, reg.nome)==0)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = j;
                            remocoes++;
                        }
                        break;
                    case 6:
                        if(strcmp(valorCampo, reg.especie)==0)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = j;
                            remocoes++;
                        }
                        break;
                    case 7:
                        if(strcmp(valorCampo, reg.habitat)==0)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = j;
                            remocoes++;
                        }
                        break;
                    case 8:
                        if(strcmp(valorCampo, reg.tipo)==0)
                        {
                            eliminarRegistro(arquivo, topo);
                            topo = j;
                            remocoes++;
                        }
                        break;
                    case 9:
                        if(strcmp(valorCampo, reg.dieta)==0)
                        {
                            eliminarRegistro(arquivo,topo);
                            topo = j;
                            remocoes++;
                        }
                        break;
                    case 10:
                        if(strcmp(valorCampo, reg.alimento)==0)
                        {
                            eliminarRegistro(arquivo, topo);
                            cabecalho.topo = j;
                            remocoes++;
                        }
                        break;
                    default:
                        printf("Erro\n");
                        break;
                }
            }
            j++;
        }
    }

    cabecalho.topo = topo;
    cabecalho.nroRegRem = remocoes;
    cabecalho.status = '1';

    aux = EscreverCabecalho(arquivo, cabecalho);
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

    cabecalho = LerCabecalho(arqBinOriginal);                   // Lê o cabeçalho
    EscreverCabecalho(arqBinFinal, cabecalho);                  // Escreve no novo arquivo
    novoRegistro = lerRegistro(arqBinOriginal);                 // Lê o primeiro registro
    atualRRN++;                                                 // Atualiza total de registros

    while(((atualRRN) <= cabecalho.proxRRN))                    // Verifica se acabou o arquivo
    {
        if(novoRegistro.removido != '1')                        // Se for removido ignora
        {
            qtdeRRN++;                                          // Adiciona um novo registro
            EscreverRegistro(arqBinFinal, novoRegistro, qtdeRRN);
        }
            
    
        novoRegistro = lerRegistro(arqBinOriginal);             // Lê o próximo
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