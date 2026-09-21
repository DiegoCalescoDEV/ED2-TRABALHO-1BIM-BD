union SValores 
{
	int valorI;
	float valorN;
	char valorT[20];
	char valorD[10];
	char valorC;
};

struct pdados 
{
	union SValores valor;
	struct pdados *prox;
};
typedef struct pdados PDados;

PDados *CriaDado(char valor[50], char tipo)
{
    PDados *novo;

    novo = (PDados *)malloc(sizeof(PDados));
    novo->prox = NULL;

    if (tipo == 'I')
        novo->valor.valorI = atoi(valor);

    else if (tipo == 'N')
        novo->valor.valorN = atof(valor);

    else if (tipo == 'D')
        strcpy(novo->valor.valorD, valor);

    else if (tipo == 'C')
        novo->valor.valorC = valor[0];

    else if (tipo == 'T')
        strcpy(novo->valor.valorT, valor);

    return novo;
}
