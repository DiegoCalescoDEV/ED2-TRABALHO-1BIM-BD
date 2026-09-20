#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio2.h>

#include "TADAuxiliares.h"
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

	show_database(bd);
	show_estrutura(bd);

	return 0;	
};
