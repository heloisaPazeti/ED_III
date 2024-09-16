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
    int valorCampoInt;
    int tipoPesquisa;
    int offset, encontrou;
    char nomeCampo[15], valorCampo[61];
    RegDados reg;
    FILE *arquivo;

    arquivo = fopen(nomeArq, "rb");
    if(arquivo == NULL)
    {
        printf("Erro na abertura do arquivo\n");
        return -1;
    }
    
    scanf("%d", &n);
    fseek(arquivo, 1600, SEEK_SET);
    for(i=0; i<n; i++)
    {
        offset = i*160;
        scanf("%s", nomeCampo);
        getchar();

        if(strncmp(nomeCampo, "populacao",3)==0 || strncmp(nomeCampo, "tamanho",3)==0 || strncmp(nomeCampo, "velocidade",3)==0)
        {
            scanf("%d", &valorCampoInt);
        }

        else
            fgets(valorCampo, 61, stdin);

        tipoPesquisa = definirTipo(valorCampo);
        reg = lerRegistro(arquivo);

        printf("Busca %d", i+1);
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
                if(reg.tamanho == valorCampoInt)
                {
                    imprimirRegistro(reg);
                    ++encontrou;
                }
                break;
            case 3:
                if(strncmp(valorCampo, reg.unidadeMedida, 1)==0)
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
                if(strncmp(valorCampo, reg.nome, 1)==0)
                {
                    imprimirRegistro(reg);
                    ++encontrou;
                }
                break;
            case 6:
                if(strncmp(valorCampo, reg.especie, 1)==0)
                {
                    imprimirRegistro(reg);
                    ++encontrou;
                }
                break;
            case 7:
                if(strncmp(valorCampo, reg.habitat, 1)==0)
                {
                    imprimirRegistro(reg);
                    ++encontrou;
                }
                break;
            case 8:
                if(strncmp(valorCampo, reg.tipo, 1)==0)
                {
                    imprimirRegistro(reg);
                    ++encontrou;
                }
                break;
            case 9:
                if(strncmp(valorCampo, reg.dieta, 1)==0)
                {
                    imprimirRegistro(reg);
                    ++encontrou;
                }
                break;
            case 10:
                if(strncmp(valorCampo, reg.alimento, 1)==0)
                {
                    imprimirRegistro(reg);
                    ++encontrou;
                }
                break;
        }
        if(encontrou==0) printf("Registro Inexistente\n");
        fseek(arquivo, offset, SEEK_CUR);
    }
    
}