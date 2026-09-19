struct pcampos // Lista Generalizada
{
	char campo[50];
	char tipo, pk;
	struct pdados *pdados, *patual;
	struct pcampos *fk, *prox; //*cabeca, *cauda;
};
typedef struct pcampos PCampos;

PCampos *CriaCampo(char campo[50], char tipo[20])
{
	PCampos *novoCampo = (PCampos *)malloc(sizeof(PCampos));
	if (strcmp(tipo, "INTEGER") == 0)
		novoCampo->tipo = "I";
	else if (strcmp(tipo, "NUMERIC") == 0)
		novoCampo->tipo = "N";
	else if (strcmp(tipo, "DATE") == 0)
		novoCampo->tipo = "D";
	else if (strcmp(tipo, "CHARACTER(1)") == 0)
		novoCampo->tipo = "C";
	else
		novoCampo->tipo = "T";

	novoCampo->pk = "N";
	novoCampo->pdados = novoCampo->patual = novoCampo->fk = novoCampo->prox = NULL;
	strcpy(novoCampo->campo, campo);
	return novoCampo;
}

PCampos *BuscaCampo(PTabelas *tabela, char nomeCampo[50])
{
    PCampos *campo = tabela->pcampo;

    while (campo != NULL && strcmp(campo->campo, nomeCampo) != 0)
        campo = campo->prox;

    return campo;
}

