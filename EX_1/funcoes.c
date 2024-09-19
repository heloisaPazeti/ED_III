#include <math.h>
#include "funcoes.h"
#include "funcoesFornecidas.h"
#include "funcoesBusca.h"
#include "structs.h"

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

    if (arqCSV == NULL)
    {
        printf("Falha no processamento do arquivo [Arq. CSV == NULL]\n");
        return -1;
    }

    scanf("%s", nomeBin);
    arqBin = fopen(nomeBin, "wb");

    fgets(line, tamRegistro, arqCSV);

    // ESCREVER CABEÇALHO

    cabecalho = IniciarCabecalho();
    fwrite(&cabecalho.status, sizeof(char),1, arqBin);
    fwrite(&cabecalho.topo, sizeof(int),1, arqBin);
    fwrite(&cabecalho.proxRRN, sizeof(int),1, arqBin);
    fwrite(&cabecalho.nroRegRem, sizeof(int),1, arqBin);
    fwrite(&cabecalho.nroPagDisco, sizeof(int),1, arqBin);
    fwrite(&cabecalho.qttCompacta, sizeof(int),1, arqBin);

    for(i = 21; i < 1600; i++)
        fwrite("$", sizeof(char), 1, arqBin);

    while(!feof(arqCSV))
    {
        RegDados newDado = IniciarRegistroDados();
        quantReg++;

        fgets(line, tamRegistro, arqCSV);
        int len = strlen(line);
        if(line[len-1] == '\n')
            line[len-1] = 0;

        tofree = str = strdup(line);

        for(int i = 0; i < 10; i++)
        {
            if(i < 9)
                token = strsep(&str, ",");
            else
                token = strsep(&str, "\r");

            switch (i)
            {
                case 0:
                    newDado.nome = token;
                    sNome = strlen(token);
                    break;

                case 1:
                    newDado.dieta = token;
                    sDieta = strlen(token);
                    break;

                case 2:
                    newDado.habitat = token;
                    sHabitat = strlen(token);
                    break;

                case 3:
                    if(*token != '\0')
                        newDado.populacao = atoi(token);
                    break;

                case 4:
                    newDado.tipo = token;
                    sTipo = strlen(token);
                    break;

                case 5:
                    if(*token != '\0')
                        newDado.velocidade = atoi(token);
                    break;

                case 6:
                    if(*token != '\0')
                        newDado.unidadeMedida = token[0];
                    break;

                case 7:
                    if(*token != '\0')
                        newDado.tamanho = atof(token);
                    break;

                case 8:
                    newDado.especie = token;
                    sEspecie = strlen(token);
                    break;

                case 9:
                    newDado.alimento = token;
                    sAlimento = strlen(token);
                    break;

                default:
                    break;
            }
        }

        fwrite(&newDado.removido, sizeof(char),1, arqBin);
        fwrite(&newDado.encadeamento, sizeof(int),1, arqBin);
        fwrite(&newDado.populacao, sizeof(int),1, arqBin);
        fwrite(&newDado.tamanho, sizeof(float),1, arqBin);
        fwrite(&newDado.unidadeMedida, sizeof(char),1, arqBin);
        fwrite(&newDado.velocidade, sizeof(int),1, arqBin);
        fwrite(newDado.nome, sizeof(char),sNome, arqBin);
        fwrite(&delim, sizeof(char),1, arqBin);
        fwrite(newDado.especie, sizeof(char),sEspecie, arqBin);
        fwrite(&delim, sizeof(char),1, arqBin);
        fwrite(newDado.habitat, sizeof(char),sHabitat, arqBin);
        fwrite(&delim, sizeof(char),1, arqBin);
        fwrite(newDado.tipo, sizeof(char),sTipo, arqBin);
        fwrite(&delim, sizeof(char),1, arqBin);
        fwrite(newDado.dieta, sizeof(char),sDieta, arqBin);
        fwrite(&delim, sizeof(char),1, arqBin);
        fwrite(newDado.alimento, sizeof(char),sAlimento, arqBin);
        fwrite(&delim, sizeof(char),1, arqBin);

        long int posicaoAtual = ftell(arqBin);
        int posicaoFinal = 1600 + (quantReg*160);

        for(i = posicaoAtual; i < posicaoFinal; i++)
            fwrite("$", sizeof(char), 1, arqBin);
    }

    qtdePagDisco = (1+(quantReg/10));
    if(qtdePagDisco*10 < quantReg+10)
        qtdePagDisco++;

    fseek(arqBin, 13, SEEK_SET);
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
