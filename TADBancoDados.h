struct banco_dados 
{
	char nome[20];
	struct PTabelas *ptabela;
};
typedef struct banco_dados BancoDados;