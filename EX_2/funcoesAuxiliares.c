#include "funcoesAuxiliares.h"
#include "structs.h"

//////////////////////////////////////////////////////// FUNCOES DE LEITURA

CabecalhoArvBin LerCabecalhoArvore(char *arquivo)
{
    CabecalhoArvBin cabecalho = CriarCabecalhoArvBin(); // Inicializa Arvore
    FILE *arqBin = fopen(arquivo, "rb");

    if (ChecarIntegridadeArquivo(arqBin, arquivo) < 0) return cabecalho;
    
    fread(&cabecalho.status, sizeof(char), 1, arqBin);
    fread(&cabecalho.noRaiz, sizeof(int), 1, arqBin);
    fread(&cabecalho.RRNproxNo, sizeof(int), 1, arqBin);
    fread(cabecalho.lixo, sizeof(char), 83, arqBin);

    fclose(arqBin);
    return cabecalho;
}

// Le o nó no rrn especifico
NoArvBin LerNoArvore(char *arquivo, int rrn)
{
    NoArvBin no = CriarNo();                    // Inicializa no
    FILE *arqBin = fopen(arquivo, "rb");
    int i;

    if (ChecarIntegridadeArquivo(arqBin, arquivo) < 0) return no;
        
    fseek(arqBin, (rrn * tamNo) + tamCabecalhoArvore, SEEK_SET);

    fread(&no.folha, sizeof(char), 1, arqBin);
    fread(&no.nroChavesNo, sizeof(int), 1, arqBin);
    fread(&no.RRNdoNo, sizeof(int), 1, arqBin);

    for (i = 0; i < no.nroChavesNo; i++)        // Le todos os Pi, Ci, PRi
    {
        fread(&no.P[i], sizeof(int), 1, arqBin);
        fread(&no.info[i].C, sizeof(long), 1, arqBin);
        fread(&no.info[i].PR, sizeof(long), 1, arqBin);
    }

    fread(&no.P[i], sizeof(int), 1, arqBin);    // Le o ponteiro final
    fclose(arqBin);
    return no;
}

//////////////////////////////////////////////////////// FUNCOES DE ESCRITA

/*
EscreveNo(FILE *arqArvBin, NoArvBin no, int offset)
{
    int i;

    fseek(arqArvBin, offset, SEEK_SET);

    fwrite(no.folha, sizeof(char), 1, arqArvBin);
    fwrite(no.nroChavesNo, sizeof(int), 1, arqArvBin);
    fwrite(no.RRNdoNo, sizeof(int), 1, arqArvBin);

    for (i = 0; i < no.nroChavesNo; i++)        
    {
        fwrite(no.P[i], sizeof(int), 1,  arqArvBin);
        fwrite(no.C[i], sizeof(long), 1,  arqArvBin);
        fwrite(no.PR[i], sizeof(long), 1,  arqArvBin);
    }
}*/

int EscreveNo(char *nomeArq, NoArvBin no, int offset)
{
    int i;
    FILE *arqArvBin = fopen(nomeArq, "wb+");
    if(ChecarIntegridadeArquivo(arqArvBin, nomeArq) == -1) return -1;

    fseek(arqArvBin, offset, SEEK_SET);
    fwrite(no.folha, sizeof(char), 1, arqArvBin);
    fwrite(no.nroChavesNo, sizeof(int), 1, arqArvBin);
    fwrite(no.RRNdoNo, sizeof(int), 1, arqArvBin);

    for (i = 0; i < no.nroChavesNo; i++)        
    {
        fwrite(no.P[i], sizeof(int), 1,  arqArvBin);
        fwrite(no.info[i].C, sizeof(long), 1,  arqArvBin);
        fwrite(no.info[i].PR, sizeof(long), 1,  arqArvBin);
    }
}

//////////////////////////////////////////////////////// FUNCOES DE INSERCAO

int InserirArvoreVazia(char *nomeArqArvore, char *chave, int pr)
{
    CabecalhoArvBin cabecalho;
    NoArvBin no;
    int resultado;

    no = CriarNo();
    no.info[0].C = converteNome(chave);
    no.info[0].PR = pr;
    no.RRNdoNo = 0;

    if (EscreveNo(nomeArqArvore, no, tamCabecalhoArvore) == -1) return -1;

    return 0;
}

int InserirNoSemOverflow(char *nomeArqArvore, NoPos resultado, char *chave, int pr)
{
    NoArvBin no;
    no = OrdenaNo(no, resultado, converteNome(chave), pr);
    if(EscreveNo(nomeArqArvore, no, no.RRNdoNo*tamNo + tamCabecalhoArvore) == -1) return -1;

    return 0;

    //long int convert = converteNome(chave);
    //int offset;
    //int pos;
    //FILE *arqArvBin;
    
    //arqArvBin = fopen(nomeArqArvore, "wb+");
    //if(ChecarIntegridadeArquivo(arqArvBin, nomeArqArvore) == -1) return;

    //offset = tamCabecalho + resultado.no.RRNdoNo*tamNo + pos*tamRegistro;
    //pos = resultado.posInsercao;
    // Árvore vazia: cria um novo nó, o qual é raiz e folha 
    /*
    if(resultado.pos == -2)
    {
        CabecalhoArvBin cabecalho = CriarCabecalhoArvBin(); // Inicializa Arvore
        offset = tamCabecalho;
        fseek(arqArvBin, offset, SEEK_SET);
        no = CriarNo();
        
        no.folha = '1'; 
        no.nroChavesNo = 1;
        no.RRNdoNo = 1;
        no.C[0] = convert;
        no.P[0]= -1;
        no.PR[0]= rrn;

        EscreveNo(arqArvBin, no, offset);
    }*/
    
    // Insere em um nó existente PRECISA ORDENAR AS CHAVES <3
}

int InserirNoComOverflow(char *nomeArqArvore, NoPos resultado, RegistroInfo info)
{
    RegistroInfo *infosOrdenadas;
    NoArvBin noEsquerdo = CriarNo();
    NoArvBin noDireito = CriarNo();
    int posicaoPromovida = 2;                   // (m/2 + 0.5)-1 => Como ordem é impar soma-se 0.5 e retira-se 1 pela contagem começar no zero. 

    infosOrdenadas = OrdenaInfos(ordemArvore, resultado, info);

    for(int i = 0; i < 5; i++)
    {
        if(i < posicaoPromovida)
            noEsquerdo.info[i] = infosOrdenadas[i];
        else if(i > posicaoPromovida)
            noDireito.info[i-posicaoPromovida-1] = infosOrdenadas[i];
    }

    //PromoveChave(resultado.no.infos[posicaoPromovida]);

    noEsquerdo.folha = resultado.no.folha;
    noDireito.folha = resultado.no.folha;

    noEsquerdo.nroChavesNo = posicaoPromovida;
    noDireito.nroChavesNo = posicaoPromovida;

    noEsquerdo.RRNdoNo = resultado.no.RRNdoNo;
    noDireito.RRNdoNo = LerCabecalho(fopen(nomeArqArvore, "rb")).proxRRN;

}

//////////////////////////////////////////////////////// FUNCOES DE BUSCA

// Retorna uma struct com no + pos na arvore na qual está / deveria estar o registro
// Se houver retorna o no + rrn
// Se não houver retorna na posicao -1
// Retorna -2 na posicao se der algum erro
NoPos BuscarNoArvore(char *arquivo, char *chave)
{
    CabecalhoArvBin cabecalho = LerCabecalhoArvore(arquivo);
    NoArvBin no;
    NoPos noPos;
    int i, nextRrr;

    noPos.pos = -2;

    if ((ChecarCabecalho(cabecalho) < 0) || (ChecarArvoreVazia(cabecalho, 1) < 0)) return noPos;

    nextRrr = cabecalho.noRaiz;                     // Pega o rrn da raiz
    while (noPos.pos < -1)                          // Enquanto não encontrar o fim ou a chave
    {
        no = LerNoArvore(arquivo, nextRrr);         // Le o no
        for (int i = 0; i < no.nroChavesNo; i++)    // Checa todas as chaves no no
        {
            if (no.info[i].C == converteNome(chave))     // Se encontrou
            {
                noPos.no = no;                      // Seleciona o no
                noPos.pos = i;                      // Seleciona a pos
                break;
            }
            else if (no.info[i].C > converteNome(chave)) // Se a chave atual for maior que a chave procurada
            {
                nextRrr = no.P[i];                  // "Desce" para o no Pi
                noPos.posInsercao = i;
                break;
            }
            else if (i == (no.nroChavesNo - 1))     // Se chegar ao final da arvore
            {
                nextRrr = no.P[i+1];                // "Desce" para o no Pi+1
                noPos.posInsercao = i+1;
            }
        }

        if (nextRrr == -1)                          // Caso nao tenha encontrado
        {
            noPos.pos = -1;
            noPos.posInsercao = i;
        }
    }

    if (noPos.pos == -1)                            // Registro não existe
        printf("Registro inexistente.\n");

    return noPos;
}

//////////////////////////////////////////////////////// CHECAGENS

int ChecarCabecalho(CabecalhoArvBin cabecalho)
{
    if (cabecalho.status == '0')
    {
        printf("Cabecalho corrompido.\n");
        return -1;
    }

    return 0;
}

int ChecarArvoreVazia(CabecalhoArvBin cabecalho, int printError)
{
    if (cabecalho.noRaiz == -1)
    {
        if(printError == 1) printf("Essa arvore esta vazia.\n");
        return -1;
    }

    return 0;
}

int ChecarIntegridadeArquivo(FILE *arquivo, char *nomeArq)
{
    if(arquivo == NULL)
    {
        printf("Foi impossível de abrir o arquivo: %s\n", nomeArq);
        return -1;
    }

    return 0;
}


//////////////////////////////////////////////////////// HELPERS

int AlterarCabecalho(char *nomeArq, char status, int noRaiz, int rrnProxNo)
{
    FILE *arq = fopen(nomeArq, "wb+");

    if(ChecarIntegridadeArquivo(arq, nomeArq) == -1) return -1;

    fwrite(status, sizeof(char), 1, arq);
    fwrite(noRaiz, sizeof(int), 1, arq);
    fwrite(rrnProxNo, sizeof(int), 1, arq);
    fclose(arq);
    return 0;
}

NoArvBin OrdenaNo(NoArvBin noOriginal, NoPos noPos, long int chave, long int pr)
{
    NoArvBin noFinal = noOriginal;

    noFinal.info[noPos.posInsercao].C = chave;
    noFinal.info[noPos.posInsercao].PR = pr;

    for(int i = noPos.posInsercao+1; i < noFinal.nroChavesNo; i++)
    {
        noFinal.info[i].C = noOriginal.info[i-1].C;
        noFinal.info[i].PR = noOriginal.info[i-1].PR;
    }

    return noFinal;
}

RegistroInfo* OrdenaInfos(int size, NoPos resultado, RegistroInfo info)
{
    RegistroInfo *infosOrdenadas = calloc(size, sizeof(RegistroInfo));

    for(int i = 0; i < size; i++)
    {
        if(i < resultado.posInsercao)
        {
            infosOrdenadas[i].C = resultado.no.info[i].C;
            infosOrdenadas[i].PR = resultado.no.info[i].PR;
        }
        else if(i == resultado.posInsercao)
        {
            infosOrdenadas[i].C = info.C;
            infosOrdenadas[i].PR = info.PR;
        }
        else
        {
            infosOrdenadas[i].C = resultado.no.info[i-1].C;
            infosOrdenadas[i].PR = resultado.no.info[i-1].PR;
        }
    }

    return infosOrdenadas;
}
//////////////////////////////////////////////////////// FUNCOES TRABALHO 1

// Lê um registro e retorna
RegDados lerRegistro(FILE *arqBin, char *arquivo)
{
    RegDados temp, fim;
    char dado[142], *linha;

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
    if(temp.removido != '1')                                // Caso o campo não tenha sido removido, a leitura dos campos é finalizada
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

void imprimirRegistro(RegDados registro)
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

// Calcula o tamanho de uma string
int tamanhoString(char *string)
{
    int i=0;
    while(string[i]!='\0')
        i++;

    return i;
}

RegDados lerDadosDoTeclado()
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
        binarioNaTela(nomeArq);
        return -1;
    }

    topo = cabecalho.topo;                              // O valor inicial do topo é o indicado pelo cabeçalho
    rrn = cabecalho.proxRRN;                            // O valor inicial do rrn é o indicado pelo cabeçalho
    removidos = cabecalho.nroRegRem;                    // A quantidade inicial de registros removidos é a indicada pelo cabeçalho

    cabecalho.status = '0';                             // Muda o status do cabeçalho durante a manipulação do arquivo
    EscreverCabecalho(arquivo, cabecalho);              // Atualiza o status do cabeçalho

    if(topo == -1)                                  // Caso topo = -1, não há registros removidos e a inserção é feita ao final do arquivo
    {
        //registro = IniciarRegistroDados();          // Inicializa um novo registro
        //registro = lerDadosDoTeclado();             // Lê o registro do teclado
        rrn++;                                      // Aumenta o rrn
        fseek(arquivo, 0, SEEK_END);                // Posiciona o cursor ao final do arquivo
        escreverRegistro(arquivo, registro, rrn);   // Escreve o registro no arquivo
    }
    else if(topo != -1)                             // Caso topo != -1, a inserção deve ser feita no registro do topo
    {
        fseek(arquivo, 1600+160*topo, SEEK_SET);    // Posiciona o cursor no topo
        registro2 = lerRegistro(arquivo);            // Lê o registro do topo
        topo = registro2.encadeamento;               // Atauliza o topo para o encadeamento do registro a ser substituído

        //registro = IniciarRegistroDados();         // Inicializa um segundo registro de dados
        //registro = lerDadosDoTeclado();            // Lê o registro de dados do teclado
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
    binarioNaTela(nomeArq);
    return rrn;
}

///////////////////////////////////////////////////////////////// ESCREVER NO ARQUIVO (1)

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

///////////////////////////////////////////////////////////////// PRINTAR REGISTROS (2)

// Le os dados do cabeçalho de um arquivo e retorna uma struct cabeçalho
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

///////////////////////////////////////////////////////////////// ADICIONAR REGISTROS (5)

// Lê dados do teclado e atribui os valroes a uma variável de registro
RegDados lerDadosDoTeclado()
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


