#include "funcoes.h"
#include "funcoesFornecidas.h"
#include "funcoesBusca.h"

// int EscreverArquivo()
// {
//     char arqCSV[15];
//     char arqBin[15];
//     RegCabecalho newCabecalho = IniciarCabecalho();
//     RegDados newDado = IniciarRegistroDados();

//     scanf("%s", arqCSV);
//     scanf("%s", arqBin);

//     while(newDado.populacao != -1)
//     {
//         //newDado = LerCSV(arqCSV);
//         //Escrever no arquivo
//     }
   
// }

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
    for(i=0; i<n; i++)
    {
        fseek(arquivo, 1600, SEEK_SET);
        j=0;
        encontrou = 0;
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
        printf("Busca %d\n", i+1);

        while(1)
        {
            offset = 1600+j*160;
            reg = lerRegistro(arquivo);
            
            if (reg.removido=='2') break;


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
                    if(reg.tamanho*100 == valorCampoFloat*100)
                    {
                        imprimirRegistro(reg);
                        ++encontrou;
                    }
                    break;
                case 3:
                    if(strcmp(valorCampo, &reg.unidadeMedida)==0)
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
            j++;
        }
       
        if(encontrou==0) printf("Registro inexistente.\n\n");

        printf("Numero de paginas de disco: %d\n\n", numPag);
        
    }
    
    fclose(arquivo);

    return 0;
}

int RemoverRegistros(char *nomeArq)
{
    int n;
    int i;
    int valorCampoInt;
    int tipoPesquisa;
    int encontrou, aux2;
    int proxRRN;
    float valorCampoFloat;
    char nomeCampo[15], valorCampo[61], aux1[1];
    RegDados reg;
    FILE *arquivo;

    arquivo = fopen(nomeArq, "rb+");
    if(arquivo == NULL)
    {
        printf("Erro na abertura do arquivo \n");
        return -1;
    }

    encontrou = 0;
    proxRRN = buscarRRN(arquivo);

    scanf("%d", &n);
    for(i=0; i<n; i++)
    {
        fseek(arquivo, 1600, SEEK_SET);
        encontrou = 0;
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
        {
            scan_quote_string(valorCampo);
        }

        tipoPesquisa = definirTipo(nomeCampo);

        while(1)
        {
            reg = lerRegistro(arquivo);
            
            if (reg.removido=='2') break;


            switch(tipoPesquisa)
            {
                case 1:
                    if(reg.populacao == valorCampoInt)
                    {
                        eliminarRegistro(arquivo, proxRRN);
                        ++encontrou;
                    }
                    break;
                
                case 2:
                    if(reg.tamanho*100 == valorCampoFloat*100)
                    {
                        eliminarRegistro(arquivo, proxRRN);
                        ++encontrou;
                    }
                    break;
                case 3:
                    if(strcmp(valorCampo, &reg.unidadeMedida)==0)
                    {
                        eliminarRegistro(arquivo, proxRRN);
                        ++encontrou;
                    }
                    break;
                case 4:
                    if(reg.velocidade == valorCampoInt)
                    {
                        eliminarRegistro(arquivo, proxRRN);
                        ++encontrou;
                    }
                    break;
                case 5:
                    if(strcmp(valorCampo, reg.nome)==0)
                    {
                        eliminarRegistro(arquivo, proxRRN);
                        ++encontrou;
                    }
                    break;
                case 6:
                    if(strcmp(valorCampo, reg.especie)==0)
                    {
                        eliminarRegistro(arquivo, proxRRN);
                        ++encontrou;
                    }
                    break;
                case 7:
                    if(strcmp(valorCampo, reg.habitat)==0)
                    {
                        eliminarRegistro(arquivo, proxRRN);
                        ++encontrou;
                    }
                    break;
                case 8:
                    if(strcmp(valorCampo, reg.tipo)==0)
                    {
                        eliminarRegistro(arquivo, proxRRN);
                        ++encontrou;
                    }
                    break;
                case 9:
                    if(strcmp(valorCampo, reg.dieta)==0)
                    {
                        eliminarRegistro(arquivo, proxRRN);
                        ++encontrou;
                    }
                    break;
                case 10:
                    if(strcmp(valorCampo, reg.alimento)==0)
                    {
                        eliminarRegistro(arquivo, proxRRN);
                        ++encontrou;
                    }
                    break;
                default:
                    printf("Erro\n");
                    break;
            }
        }
       
    }

    fclose(arquivo);

    binarioNaTela(nomeArq);
    
    return 0;
}