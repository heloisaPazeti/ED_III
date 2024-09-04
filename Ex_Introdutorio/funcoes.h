//////////////////////////////////////////// TAMANHO REGISTROS

#define idSize 4
#define tamanhoRegistro 131

//////////////////////////////////////////// STRUCT

typedef struct Especie
{
    int id;
    char nome[41];
    char nomeCient[61];
    int populacao;
    char status[9];
    float locX, locY;
    int impacto;
} Especie;
 
//////////////////////////////////////////// OPÇÕES DE REGISTROS

int registrarEspecie(char  *nomeArq);
int relatorioEspecies(char *nomeArq);
int buscarEspecie(char *nomeArq);
int registrarInformacao(char *nomeArq);

//////////////////////////////////////////// FUNÇÕES AUXILIARES

FILE* abrirArquivo(char *nomeArq, char *mode);
Especie criarEspecie(void);
void mostrarRelatorio(Especie especie);
int montarEspecie(Especie *especie, FILE *arquivo);
