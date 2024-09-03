#include <stdlib.h>
#include <stdio.h>
#include "funcoes.h"

int main()
{
    int n;
    char arquivo[15];

    scanf("%d ", &n);
    scanf("%s", arquivo);

    if(n==1)
        registrarEspecie(arquivo);

    return 0;
}
