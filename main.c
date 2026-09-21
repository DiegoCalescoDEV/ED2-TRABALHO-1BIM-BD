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
	system("cls");
	printf("==== MENU DE COMANDOS SQL ====\n\n");
	printf("[A] Importar Sript SQL\n");
	printf("[B] Executar comandos SQL\n");
	printf("[C] Show Database\n");

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
			if (bd != NULL)
			{
				printf("\n[ERRO] Ja foi importado um script SQL.\n");
				getch();
			}
			else
			{
				printf("\nDigite o arquivo script com a extensao: ");
				fflush(stdin);
				gets(nomeScript);
				lerScript(nomeScript, &bd);
			}

			break;
		case 'B':
			system("cls");
			printf("==== PROMPT SQL ====\n\n\n");
			if (bd == NULL)
			{
				printf("[ERRO] Nao existe nenhum banco de dados para executar comandos SQL.\n");
				getch();
			}
			else
			{
				do
				{
					printf("SQL> ");
					fflush(stdin);
					gets(comandoSQL);
					lerSQL(comandoSQL, bd);
				} while (strcmp(comandoSQL, "exit") != 0);
			}
			break;
		case 'C':
			show_database(bd);
			getch();

			break;
		case 'D':
			show_dados(bd);
			getch();
			break;
		case 27:
			break;

		default:
			printf("\nERRO Comando invalido!\n");
			getch();
			break;
		}
	} while (tolower(opcao) != 27);

	return 0;
};
