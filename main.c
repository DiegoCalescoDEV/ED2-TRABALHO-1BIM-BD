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

char menu()
{
	clrscr();
	printf("==== MENU DE COMANDOS SQL ====\n\n");
	printf("[A] Importar Sript SQL\n");
	printf("[B] Show Database\n");
	printf("[C] Executar comando SQL\n");

	printf("\n[ESC] Sair\n");

	return toupper(getch());
}

int main()
{
	char opcao, nomeScript[20], comandoSQL[100];
	BancoDados *bd = NULL;

	do
	{
		opcao = menu();
		switch (opcao)
		{
			case 'A':
				printf("\nDigite o arquivo script com a extensao: ");
				fflush(stdin);
				gets(nomeScript);
				lerScript(nomeScript, &bd);
				break;
			case 'B':
				show_database(bd);
				getch();
				break;
			
			default:
				break;
		}
	} while (opcao != 27);


	return 0;	
};
