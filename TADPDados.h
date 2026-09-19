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

