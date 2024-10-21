#include "funcoesArvoreB.h"
#include "funcoesAuxiliares.h"
#include "funcoesInsereArvore.h"
#include "funcoesBuscaArvore.h"

///////////////////////////////////////////////////////////////// CRIAR ARVORE (7)

int CriarArvore(char *nomeArq, char *nomeArqArvore)
{

}

///////////////////////////////////////////////////////////////// BUSCAR REGISTRO (8)

int BuscarRegistroArvore(char *nomeArq, char *nomeArqArvore)
{
    char *campo, *chave;
    NoArvBin no;
    NoPos resultado;
    RegDados reg;
    FILE *arq;

    if(ChecarCabecalho(LerCabecalhoArvore(nomeArqArvore)) < 0) return -1;

    campo = calloc(10, sizeof(char));
    chave = calloc(160, sizeof(char));

    scanf("%s", campo);                                 // Escaneia campo da chave
    scan_quote_string(chave);                           // Escaneia chave de procura

    if(strncmp(campo, "nome", 4) != 0)
    {
        printf("A arvore esta indexada de acordo com o campo nome. Nao com: %s\n", campo);
        return -1;
    }
    
    resultado = BuscarNoArvore(nomeArqArvore, converteNome(chave));      // Busca na arvore -> retorna com no e posicao

    if(resultado.pos == -1)
    {
        printf("Registro inexistente.\n");
        return 0;
    }

    if (resultado.pos == -2)           // Não foi encontrado ou encontrou algum erro
        return -1;

    arq = fopen(nomeArq, "rb");                         // Abre arquivo de registros
    if(ChecarIntegridadeArquivo(arq, nomeArq) < 0) return -1;

    fseek(arq, resultado.no.info[resultado.pos].PR, SEEK_SET);     // Vai para a posicao da chave
    reg = lerRegistro(arq, nomeArq);                             // Lê o registro
    imprimeRegistro(reg);                              // Imprime o registro

    fclose(arq); 
    return 0;
}

///////////////////////////////////////////////////////////////// ADICIONAR REGISTRO (9)

int AdicionarRegistroArvore(char *nomeArq, char *nomeArqArvore)
{
    int n, i, retorno;
    int rrn;
    NoPos resultado;
    char *chave;
    CabecalhoArvBin cabecalho;

    //binarioNaTela(nomeArqArvore);


    cabecalho = LerCabecalhoArvore(nomeArqArvore);
    if(cabecalho.status == 1) return -1;

    //if(AlterarCabecalho(nomeArqArvore, '0', cabecalho.noRaiz, cabecalho.RRNproxNo) == -1) return -1;

    scanf("%d", &n);

    for(i=0; i<n; i++)
    {
        RegDados registro = lerTeclado();
        //imprimeRegistro(registro);
        
        rrn = InserirRegistrosAdap(nomeArq, registro);
        resultado = BuscarNoArvore(nomeArqArvore, converteNome(registro.nome));

        
        if(resultado.pos != -1) continue;                      // Encontrou já na árvore
        resultado.no.nroChavesNo++;

        if(resultado.posInsercao == -2)
            retorno = InserirArvoreVazia(nomeArqArvore, registro.nome, rrn);
        else
        { 
            RegistroInfo info;
            info.C = converteNome(registro.nome);
            info.PR = rrn*tamRegistro + tamTotalCabecalho;
            if(resultado.no.nroChavesNo == tamCPR)               // Ocorre overflow do nó    
                retorno = InserirNoComOverflow(nomeArqArvore, resultado, info);                      
            else                                                            // Há espaço no nó
                retorno = InserirNoSemOverflow(nomeArqArvore, resultado, info);
        
        }

        //break;  // PRA FAZER APENAS UMA VEZ -> RETIRAR DEPOIS
    }

    //if (AlterarCabecalho(nomeArqArvore, '1', 0, 1) == -1) return -1;

    binarioNaTela(nomeArqArvore);
    return retorno;
}

