struct ptabelas // Lista Duplamente Encadeada
{
	char tabela[20];
	struct pcampos *pcampo;
	struct ptabelas *ant, *prox;	
};
typedef struct ptabelas PTabelas; 


PTabelas* NovaTabela(char tabela[20])
{
	PTabelas *caixa = (PTabelas*)malloc(sizeof(PTabelas)); 	
	caixa->ant = NULL;	
	caixa->prox = NULL;	
	caixa->pcampo = NULL;		
	strcpy(caixa->tabela, tabela); 							
	return caixa;		 									
}
