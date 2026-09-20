#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio2.h>

#include "TADPDados.h"
#include "TADPCampos.h"
#include "TADPTabelas.h"
#include "TADBancoDados.h"

int main()
{
	BancoDados bd;
	bd.ptabela = NULL;

	FILE *Ptr = fopen("script.txt", "r");

	if(Ptr == NULL)
		printf("Erro ao ler script");
	else
		lerScript(Ptr, &bd);

	// BancoDados aux = bd;
	// PTabelas *auxt = bd.ptabela;
	// PCampos *auxp;
	// printf("DB %s\n", aux.nome);

	// while(auxt != NULL)
	// {
	// 	auxp = auxt->pcampo;
	// 	printf("\tTABELA %s\n", auxt->tabela);
	// 	while(auxp != NULL)
	// 	{
	// 		printf("\t\t%s %c %c\n", auxp->campo, auxp->tipo, auxp->pk);
	// 		auxp = auxp->prox;
	// 	}
	// 	printf("\n");
	// 	auxt = auxt->prox;
	// }

	return 0;	
};

