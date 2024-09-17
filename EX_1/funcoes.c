#include "funcoes.h"
#include "funcoesFornecidas.h"
#include "funcoesBusca.h"

int EscreverArquivo()
{
    char arqCSV[15];
    char arqBin[15];
    RegCabecalho newCabecalho = IniciarCabecalho();
    RegDados newDado = IniciarRegistroDados();

    scanf("%s", arqCSV);
    scanf("%s", arqBin);

    while(newDado.populacao != -1)
    {
        //newDado = LerCSV(arqCSV);
        //Escrever no arquivo
    }
   
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