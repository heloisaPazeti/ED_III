#include "funcoes.h"
#include "funcoesFornecidas.h"
#include "funcoesAuxiliares.h"
#include "funcoesBusca.h"
#include "structs.h"

///////////////////////////////////////////////////////////////// ESCREVER NO ARQUIVO (1)

int EscreverArquivo(char *nomeCSV)
{
    int  i;
    int  qtdePagDisco, quantReg = 0;
    int  sNome, sEspecie, sHabitat, sTipo, sDieta, sAlimento;
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

    cabecalho = EscreverCabecalho(arqBin);              // Escreve no arquivo os dados iniciais do cabeçalho

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

    arquivo = fopen(nomeArq, "rb");
    if(arquivo == NULL)
    {
        printf("Erro na abertura do arquivo \n");
        return -1;
    }

    encontrou = 0;
    
    numPag = numPagDisco(arquivo);
    scanf("%d", &n);
    fseek(arquivo, 1600, SEEK_SET);
    for(i=0; i<n; i++)
    {
        fseek(arquivo, 1600, SEEK_SET);
        j=0;
        encontrou = 0;
        scanf("%s", nomeCampo);
        getchar();

        if(strncmp(nomeCampo, "populacao",3)==0 || strncmp(nomeCampo, "velocidade",3)==0)
        {
            scanf("%d", &valorCampoInt);
            getchar();
        }

        if(strncmp(nomeCampo, "tamanho",3)==0)
        {
            scanf("%f", &valorCampoFloat);
        }

        else
            scan_quote_string(valorCampo);

        getchar();
        
        tipoPesquisa = definirTipo(nomeCampo);
        printf("Busca %d\n", i+1);

        while(1)
        {
            offset = 1600+j*160;
            reg = lerRegistro(arquivo);
            
            if (reg.tamanho==-1) break;

            switch(tipoPesquisa)
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
                    if(strncmp(valorCampo, reg.nome, 10)==0)
                    {
                        imprimirRegistro(reg);
                        ++encontrou;
                    }
                    break;
                case 6:
                    if(strncmp(valorCampo, reg.especie, 10)==0)
                    {
                        imprimirRegistro(reg);
                        ++encontrou;
                    }
                    break;
                case 7:
                    if(strncmp(valorCampo, reg.habitat, 10)==0)
                    {
                        imprimirRegistro(reg);
                        ++encontrou;
                    }
                    break;
                case 8:
                    if(strncmp(valorCampo, reg.tipo, 10)==0)
                    {
                        imprimirRegistro(reg);
                        ++encontrou;
                    }
                    break;
                case 9:
                    if(strncmp(valorCampo, reg.dieta, 10)==0)
                    {
                        imprimirRegistro(reg);
                        ++encontrou;
                    }
                    break;
                case 10:
                    if(strncmp(valorCampo, reg.alimento, 10)==0)
                    {
                        imprimirRegistro(reg);
                        ++encontrou;
                    }
                    break;
            }
            
            fseek(arquivo, offset, SEEK_SET);
            j++;
        }
       
        if(encontrou==0) printf("Registro inexistente.\n\n");

        printf("Numero de paginas de disco: %d\n\n", numPag);
        
    }
    
}

///////////////////////////////////////////////////////////////// COMPACTADOR (6)