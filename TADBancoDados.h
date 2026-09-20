struct banco_dados
{
    char nome[20];
    struct ptabelas *ptabela;
};
typedef struct banco_dados BancoDados;

void create_database(char linha[], BancoDados *bd);
void create_table(FILE *Ptr, char linha[], BancoDados *bd);
void alter_table(FILE *Ptr, char linha[], BancoDados *bd);
void lerScript(FILE *Ptr, BancoDados *bd);
PTabelas *BuscaTabela(BancoDados *bd, char nomeTabela[20]);
PCampos *BuscaCampo(PTabelas *tabela, char nomeCampo[50]);

void lerScript(FILE *Ptr, BancoDados *bd)
{
    char linha[300];

    while (!feof(Ptr))
    {
        fgets(linha, 300, Ptr);
        if (strncmp(linha, "CREATE DATABASE", 15) == 0)
            create_database(linha, bd);

        else if (strncmp(linha, "CREATE TABLE", 12) == 0)
            create_table(Ptr, linha, bd);

        else if (strncmp(linha, "ALTER TABLE", 11) == 0)
            alter_table(Ptr, linha, bd);
    }
}

PTabelas *BuscaTabela(BancoDados *bd, char nomeTabela[20])
{
    PTabelas *tabela = bd->ptabela;

    while (tabela != NULL && strcmp(tabela->tabela, nomeTabela) != 0)
        tabela = tabela->prox;

    return tabela;
}

PCampos *BuscaCampo(PTabelas *tabela, char nomeCampo[50])
{
    PCampos *campo = tabela->pcampo;

    while (campo != NULL && strcmp(campo->campo, nomeCampo) != 0)
        campo = campo->prox;

    return campo;
}

void show_database(BancoDados bd)
{
    PTabelas *auxt = bd.ptabela;
    PCampos *auxp;

    printf("BANCO DE DADOS: %s\n\n", bd.nome);

    while (auxt != NULL)
    {
        auxp = auxt->pcampo;

        printf("TABELA: %s\n", auxt->tabela);
        printf("---------------------------------------------\n");
        printf("%-20s %-6s %-6s %-15s\n", "CAMPO", "TIPO", "PK", "FK");
        printf("---------------------------------------------\n");

        while (auxp != NULL)
        {
            printf("%-20s %-6c %-6c ", auxp->campo, auxp->tipo, auxp->pk);

            if (auxp->fk != NULL)
                printf("%-15s", auxp->fk->campo);
            else
                printf("%-15s", "-");

            printf("\n");

            auxp = auxp->prox;
        }

        printf("---------------------------------------------\n\n");

        auxt = auxt->prox;
    }
}

void create_database(char linha[], BancoDados *bd)
{
    int i = 16;
    int j = 0;

    while (linha[i] == ' ')
        i++;

    while (linha[i] != ';' && linha[i] != '\0')
    {
        bd->nome[j] = linha[i];
        i++;
        j++;
    }

    bd->nome[j] = '\0';
}

void alter_table(FILE *Ptr, char linha[], BancoDados *bd)
{
    int i = 12;
    char tabelaAlterada[20], tabelaReferenciada[20];
    char campoTabelaAlterada[50], campoTabelaReferenciada[50];
    PTabelas *tabelaAlt, *tabelaRef;
    PCampos *campoAlt, *campoRef;

    pega_palavra(linha, &i, tabelaAlterada);

    tabelaAlt = BuscaTabela(bd, tabelaAlterada);

    if (tabelaAlt != NULL)
    {
        fgets(linha, 300, Ptr);

        i = 0;

        pega_palavra_entre_parenteses(linha, &i, campoTabelaAlterada);

        campoAlt = BuscaCampo(tabelaAlt, campoTabelaAlterada);

        if (campoAlt != NULL)
        {
            i += 12;

            pega_palavra(linha, &i, tabelaReferenciada);

            tabelaRef = BuscaTabela(bd, tabelaReferenciada);

            if (tabelaRef != NULL)
            {
                pega_palavra_entre_parenteses(linha, &i, campoTabelaReferenciada);

                campoRef = BuscaCampo(tabelaRef, campoTabelaReferenciada);

                if (campoRef != NULL)
                    campoAlt->fk = campoRef;
                else
                    printf("ERRO Campo %s na tabela %s nao encontrado\n", campoTabelaReferenciada, tabelaReferenciada);
            }
            else
                printf("ERRO Tabela %s nao encontrada\n", tabelaReferenciada);
        }
        else
            printf("ERRO Campo %s na tabela %s nao encontrado\n", campoTabelaAlterada, tabelaAlterada);
    }
    else
        printf("ERRO Tabela %s nao encontrada\n", tabelaAlterada);
}

void create_table(FILE *Ptr, char linha[], BancoDados *bd)
{
    int i = 13;
    int j = 0;
    int k;
    int p;
    char nomeTabela[20];
    char nomeCampo[50];
    char tipo[20];
    char camposPK[100];
    char campoPK[50];
    PTabelas *novaTabela;
    PTabelas *auxTabela;
    PCampos *novoCampo;
    PCampos *auxCampo;

    pega_palavra(linha, &i, nomeTabela);

    novaTabela = NovaTabela(nomeTabela);

    if (bd->ptabela == NULL)
        bd->ptabela = novaTabela;
    else
    {
        auxTabela = bd->ptabela;

        while (auxTabela->prox != NULL)
            auxTabela = auxTabela->prox;

        auxTabela->prox = novaTabela;
        novaTabela->ant = auxTabela;
    }

    while (fgets(linha, 300, Ptr) != NULL && (linha[0] != ')' || linha[1] != ';'))
    {
        i = 0;

        pega_palavra(linha, &i, nomeCampo);

        if (strcmp(nomeCampo, "CONSTRAINT") == 0)
        {
            i = 0;

            pega_palavra_entre_parenteses(linha, &i, camposPK);

            p = 0;

            while (camposPK[p] != '\0')
            {
                pula_espacos(camposPK, &p);

                k = 0;

                while (camposPK[p] != ',' && camposPK[p] != '\0')
                {
                    campoPK[k] = camposPK[p];
                    k++;
                    p++;
                }

                campoPK[k] = '\0';

                auxCampo = BuscaCampo(novaTabela, campoPK);

                if (auxCampo != NULL)
                    auxCampo->pk = 'S';
                else
                    printf("ERRO: campo PK [%s] nao encontrado na tabela [%s]\n", campoPK, novaTabela->tabela);

                if (camposPK[p] == ',')
                    p++;
            }
        }
        else
        {
            j = 0;

            pula_espacos(linha, &i);

            while (linha[i] != ',' && linha[i] != ' ' && !(linha[i] == '(' && strcmp(tipo, "NUMERIC") == 0))
            {
                tipo[j] = linha[i];
                j++;
                i++;
                tipo[j] = '\0';
            }

            novoCampo = CriaCampo(nomeCampo, tipo);

            if (novaTabela->pcampo == NULL)
                novaTabela->pcampo = novoCampo;
            else
            {
                auxCampo = novaTabela->pcampo;

                while (auxCampo->prox != NULL)
                    auxCampo = auxCampo->prox;

                auxCampo->prox = novoCampo;
            }
        }
    }
}
