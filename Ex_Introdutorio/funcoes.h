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
int procurarRegistro(char *nomeArq);
int alterarRegistro(char *nomeArq);

//////////////////////////////////////////// FUNÇÕES AUXILIARES

int buscarEspecie(char *nomeArq);
int registrarInformacao(char *nomeArq);
Especie criarEspecie(void);
void mostrarRelatorio(Especie especie);
