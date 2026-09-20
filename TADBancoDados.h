struct banco_dados
{
    char nome[20];
    struct PTabelas *ptabela;
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
    int j = 0;
    char tabelaAlterada[20], tabelaReferenciada[20];
    char campoTabelaAlterada[50], campoTabelaReferenciada[50];

    PTabelas *tabelaAlt, *tabelaRef;
    PCampos *campoAlt, *campoRef;
    while (linha[i] == ' ')
        i++;

    while (linha[i] != ' ')
    {
        tabelaAlterada[j] = linha[i];
        i++;
        j++;
    }

    tabelaAlterada[j] = '\0';

    tabelaAlt = BuscaTabela(bd, tabelaAlterada);

    if (tabelaAlt != NULL)
    {
        fgets(linha, 300, Ptr);
        i = 0;

        while (linha[i] != '(')
            i++;

        if (linha[i] == '(')
            i++;

        j = 0;

        while (linha[i] != ')')
        {
            campoTabelaAlterada[j] = linha[i];
            i++;
            j++;
        }

        campoTabelaAlterada[j] = '\0';

        campoAlt = BuscaCampo(tabelaAlt, campoTabelaAlterada);

        if (campoAlt != NULL)
        {
            i += 12;
            j = 0;

            while (linha[i] == ' ')
                i++;

            while (linha[i] != ' ')
            {
                tabelaReferenciada[j] = linha[i];
                i++;
                j++;
            }

            tabelaReferenciada[j] = '\0';

            tabelaRef = BuscaTabela(bd, tabelaReferenciada);
            if (tabelaRef != NULL)
            {
                while (linha[i] != '(')
                    i++;

                if (linha[i] == '(')
                    i++;

                j = 0;

                while (linha[i] != ')')
                {
                    campoTabelaReferenciada[j] = linha[i];
                    i++;
                    j++;
                }

                campoTabelaReferenciada[j] = '\0';

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
    char nomeTabela[20];
    char nomeCampo[50];
    char tipo[20];
    char campoPK[50];

    PTabelas *novaTabela;
    PTabelas *auxTabela;

    PCampos *novoCampo;
    PCampos *auxCampo;

    while (linha[i] == ' ')
        i++;

    while (linha[i] != '(' && linha[i] != ' ' && linha[i] != '\0')
    {
        nomeTabela[j] = linha[i];
        i++;
        j++;
    }

    nomeTabela[j] = '\0';

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
        j = 0;

        while (linha[i] == ' ')
            i++;

        while (linha[i] != ' ')
        {
            nomeCampo[j] = linha[i];
            j++;
            i++;
        }

        nomeCampo[j] = '\0';

        if (strcmp(nomeCampo, "CONSTRAINT") == 0)
        {
            i = 0;

            while (linha[i] != '(' && linha[i] != '\0')
                i++;

            if (linha[i] == '(')
                i++;

            while (linha[i] != ')' && linha[i] != '\0')
            {
                while (linha[i] == ' ')
                    i++;

                k = 0;

                while (linha[i] != ',' && linha[i] != ')' && linha[i] != '\0')
                {
                    campoPK[k] = linha[i];
                    k++;
                    i++;
                }

                campoPK[k] = '\0';

                auxCampo = BuscaCampo(novaTabela, campoPK);

                if (auxCampo != NULL)
                    auxCampo->pk = 'S';
                else
                    printf("ERRO: campo PK [%s] nao encontrado na tabela [%s]\n", campoPK, novaTabela->tabela);

                if (linha[i] == ',')
                    i++;
            }
        }
        else
        {
            j = 0;
            i++;

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
