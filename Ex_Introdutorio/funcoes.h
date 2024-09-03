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

int registrarEspecie(char  *nomeArq);
int relatorioEspecies(char *nomeArq);
int buscaEspecie(char *nomeArq);
int registrarInformacao(char *nomeArq);
