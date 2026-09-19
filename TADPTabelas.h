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
	caixa->ant = caixa->prox = caixa->pcampo = NULL;		
	strcpy(caixa->tabela, tabela); 							
	return caixa;		 									
}

PTabelas *BuscaTabela(BancoDados *bd, char nomeTabela[20])
{
    PTabelas *tabela = bd->ptabela;

    while (tabela != NULL && strcmp(tabela->tabela, nomeTabela) != 0)
        tabela = tabela->prox;

    return tabela;
}