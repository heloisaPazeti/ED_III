#include "funcoesArvoreB.h"
#include "funcoesAuxiliares.h"
#include "funcoesInsereArvore.h"
#include "funcoesBuscaArvore.h"

///////////////////////////////////////////////////////////////// CRIAR ARVORE (7)

int CriarArvore(char *nomeArq, char *nomeArqArvore)
{
    RegDados registro;
    int retorno, rrn, nroReg, regRem;
    long int offset;
    CabecalhoArvBin cabecalho;
    NoPos resultado;
    FILE *arqArvBin;
    FILE *arqBin;

    arqArvBin = fopen(nomeArqArvore, "wb+");        // Cria o arquivo da árvore
    fclose(arqArvBin);


    cabecalho = CriarCabecalhoArvBin();             // Cria o cabeçalho da árvore 
    cabecalho.status = '1';                         // Atualiza o status para 1 durante a manipulação do arquivo
    AlterarCabecalho(nomeArqArvore, '1', cabecalho.noRaiz, cabecalho.RRNproxNo);

    nroReg = 0;                                     // O nro inicial de registros é 0, bem como o nro de registros apagados
    regRem = 0;
    arqBin = fopen(nomeArq, "rb");                  // Abre o arquivo binário para leitura
    
    int qtdeReg = 0;
    registro.removido = '0';
    //(nroReg - regRem) < 25
    while(registro.removido != '2')                                        //  Percorre o arquivo binário e acrescenta os dados à árvore
    {
        offset = 1600+(nroReg*160);                 // Calcula o offset de cada registro
        //printf("OFFSET: %ld\n", offset);

        registro = IniciarRegistroDados();          // Inicializa o registro de dados
        fseek(arqBin, offset, SEEK_SET);            // Posiciona o cursor
        registro = lerRegistro(arqBin, nomeArq);    // Lê o registro
        
        if(registro.removido == '1')                           // Se o registro foi removido, aumenta o nro de registros removidos
            regRem++;
        else if(registro.removido == '0')                       // Se o registro não foi removido, busca o registro na árvore
        {
            resultado = BuscarNoArvore(nomeArqArvore, converteNome(registro.nome));         // Encontra a posição do nó na árvore

            if(resultado.pos <= -1)                              // Se o elemento não foi encontrado na árvore, continua
            {                                     
                //resultado.no.nroChavesNo++;                     // Aumenta o nro de chaves no nó
            
                RegistroInfo info;
                info.C = converteNome(registro.nome);
                info.PR = offset;

                if(resultado.posInsercao == -2)                 // Insere na árvore vazia
                    InserirArvoreVazia(nomeArqArvore, info);
                else                                            // Insere na árvore já ocupada
                { 
                    
                    if(resultado.no.nroChavesNo >= tamCPR)       // Ocorre overflow do nó
                    {
                        InserirNoComOverflow(nomeArqArvore, resultado, info, -1);
                    }
                    else                
                    {
                        InserirNoSemOverflow(nomeArqArvore, resultado.no, resultado.posInsercao, -1, info);
                    }                        // Não ocorre overflow do nó
                }
            }

            //liberaNoPos(&resultado);
            qtdeReg++;
        }
        nroReg++;
    }

    fclose(arqBin);
    //printf("%d - %d = %d\n", nroReg, regRem, qtdeReg);
    binarioNaTela(nomeArqArvore);

    return 0;
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


    cabecalho = LerCabecalhoArvore(nomeArqArvore);
    if(cabecalho.status == 1) return -1;
    scanf("%d", &n);

    for(i=0; i<n; i++)
    {
        RegDados registro = lerTeclado();
        rrn = InserirRegistrosAdap(nomeArq, registro);
        resultado = BuscarNoArvore(nomeArqArvore, converteNome(registro.nome));
        
        if(resultado.pos != -1) continue;                      // Encontrou já na árvore
        resultado.no.nroChavesNo++;

        RegistroInfo info;
        info.C = converteNome(registro.nome);
        info.PR = (rrn*tamRegistro) + tamTotalCabecalho;

        if(resultado.posInsercao == -2)
            InserirArvoreVazia(nomeArqArvore, info);
        else
        { 
            
            if(resultado.no.nroChavesNo > tamCPR)                                // Ocorre overflow do nó 
                InserirNoComOverflow(nomeArqArvore, resultado, info, -1);
            else 
                InserirNoSemOverflow(nomeArqArvore, resultado.no, -1, resultado.posInsercao, info);
        }

    }


    binarioNaTela(nomeArqArvore);
    return retorno;
}

