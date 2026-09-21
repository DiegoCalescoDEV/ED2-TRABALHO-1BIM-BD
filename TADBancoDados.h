struct banco_dados
{
    char nome[20];
    struct ptabelas *ptabela;
};
typedef struct banco_dados BancoDados;

int ExisteDado(PCampos *campo, char valor[50]);
void create_database(char linha[], BancoDados **bd);
void create_table(FILE *Ptr, char linha[], BancoDados *bd);
void alter_table(FILE *Ptr, char linha[], BancoDados *bd);
void InsereDado(PCampos *campo, char valor[50]);
void insert_into(char comando[300], BancoDados *bd);
void lerScript(char arquivo[20], BancoDados **bd);
void lerSQL(char comando[100], BancoDados *bd);
PTabelas *BuscaTabela(BancoDados *bd, char nomeTabela[20]);
PCampos *BuscaCampo(PTabelas *tabela, char nomeCampo[50]);

void show_dados(BancoDados *bd)
{
    PTabelas *tabela;
    PCampos *campo;
    int possuiDados;

    tabela = bd->ptabela;

    while (tabela != NULL)
    {
        printf("\n\nTABELA: %s\n", tabela->tabela);
        printf("------------------------------------------------------------\n");

        campo = tabela->pcampo;

        while (campo != NULL)
        {
            campo->patual = campo->pdados;

            printf("%-20s", campo->campo);

            campo = campo->prox;
        }

        printf("\n");
        printf("------------------------------------------------------------\n");

        possuiDados = 1;

        while (possuiDados)
        {
            campo = tabela->pcampo;

            if (campo == NULL || campo->patual == NULL)
                possuiDados = 0;

            if (possuiDados)
            {
                while (campo != NULL)
                {
                    if (campo->patual != NULL)
                    {
                        if (campo->tipo == 'I')
                            printf("%-20d", campo->patual->valor.valorI);

                        else if (campo->tipo == 'N')
                            printf("%-20.2f", campo->patual->valor.valorN);

                        else if (campo->tipo == 'D')
                            printf("%-20s", campo->patual->valor.valorD);

                        else if (campo->tipo == 'C')
                            printf("%-20c", campo->patual->valor.valorC);

                        else if (campo->tipo == 'T')
                            printf("%-20s", campo->patual->valor.valorT);

                        campo->patual = campo->patual->prox;
                    }
                    else
                        printf("%-20s", "NULL");

                    campo = campo->prox;
                }

                printf("\n");
            }
        }

        printf("------------------------------------------------------------\n");

        tabela = tabela->prox;
    }
}

int ExisteDado(PCampos *campo, char valor[50])
{
    PDados *dado;
    int existe = 0;
    dado = campo->pdados;

    while (dado != NULL)
    {
        if (campo->tipo == 'I')
        {
            if (dado->valor.valorI == atoi(valor))
                existe = 1;
        }
        else if (campo->tipo == 'N')
        {
            if (dado->valor.valorN == atof(valor))
                existe = 1;
        }
        else if (campo->tipo == 'D')
        {
            if (strcmp(dado->valor.valorD, valor) == 0)
                existe = 1;
        }
        else if (campo->tipo == 'C')
        {
            if (dado->valor.valorC == valor[0])
                existe = 1;
        }
        else if (campo->tipo == 'T')
        {
            if (strcmp(dado->valor.valorT, valor) == 0)
                existe = 1;
        }

        dado = dado->prox;
    }

    return existe;
}

void lerSQL(char comando[300], BancoDados *bd)
{
    int i = 0;

    pula_espacos(comando, &i);

    if (strncmp(&comando[i], "INSERT INTO", 11) == 0)
        insert_into(comando, bd);

    // else if (&comando[i], "UPDATE", 6) == 0)
    //     update_set(comando, bd);

    // else if (&comando[i], "DELETE FROM", 11) == 0)
    //     delete_from(comando, bd);

    // else if (&comando[i], "SELECT", 6) == 0)
    //     select_from(comando, bd);

    else
        printf("Comando SQL invalido!\n\n");
}

void lerScript(char arquivo[20], BancoDados **bd)
{
    char linha[300];
    FILE *Ptr = fopen(arquivo, "r");

    if (Ptr == NULL)
    {
        printf("\n[ERRO] Arquivo nao encontrado!\n");
        getch();
    }
    else
    {
        while (!feof(Ptr))
        {
            fgets(linha, 300, Ptr);
            if (strncmp(linha, "CREATE DATABASE", 15) == 0)
                create_database(linha, bd);

            else if (strncmp(linha, "CREATE TABLE", 12) == 0)
                create_table(Ptr, linha, *bd);

            else if (strncmp(linha, "ALTER TABLE", 11) == 0)
                alter_table(Ptr, linha, *bd);
        }

        fclose(Ptr);
        printf("\n[OK] Script importado com sucesso!");
        getch();
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

void show_database(BancoDados *bd)
{
    system("cls");
    printf("==== SHOW DATABASE ====\n\n");
    if (bd == NULL)
        printf("\n[ERRO] Nao existe nenhum banco de dados para exibir.\n");
    else
    {
        PTabelas *auxt = bd->ptabela;
        PCampos *auxp;

        printf("BANCO DE DADOS: %s\n\n", bd->nome);

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
}

void create_database(char linha[], BancoDados **bd)
{
    int i = 16;
    int j = 0;

    *bd = (BancoDados *)malloc(sizeof(BancoDados));

    (*bd)->ptabela = NULL;

    while (linha[i] == ' ')
        i++;

    while (linha[i] != ';' && linha[i] != '\0')
    {
        (*bd)->nome[j] = linha[i];
        i++;
        j++;
    }

    (*bd)->nome[j] = '\0';
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
                {
                    printf("\n[ERRO] Campo %s na tabela %s nao encontrado\n", campoTabelaReferenciada, tabelaReferenciada);
                    getch();
                }
            }
            else
            {
                printf("\n[ERRO] Tabela %s nao encontrada\n", tabelaReferenciada);
                getch();
            }
        }
        else
        {
            printf("\n[ERRO] Campo %s na tabela %s nao encontrado\n", campoTabelaAlterada, tabelaAlterada);
            getch();
        }
    }
    else
    {
        printf("\n[ERRO] Tabela %s nao encontrada\n", tabelaAlterada);
        getch();
    }
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
                    printf("[ERRO]: campo PK [%s] nao encontrado na tabela [%s]\n", campoPK, novaTabela->tabela);

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

void InsereDado(PCampos *campo, char valor[50])
{
    PDados *novoDado;
    PDados *auxDado;

    novoDado = CriaDado(valor, campo->tipo);

    if (campo->pdados == NULL)
    {
        campo->pdados = novoDado;
        campo->patual = novoDado;
    }
    else
    {
        auxDado = campo->pdados;

        while (auxDado->prox != NULL)
            auxDado = auxDado->prox;

        auxDado->prox = novoDado;
        campo->patual = novoDado;
    }
}

void insert_into(char comando[300], BancoDados *bd)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int inicioValores = 0;
    int temCampos = 0;
    int erro = 0;

    char nomeTabela[20];
    char listaCampos[200];
    char nomeCampo[50];
    char valor[50];

    PTabelas *tabela;
    PCampos *campo;

    pula_espacos(comando, &i);

    i += 11;

    pega_palavra(comando, &i, nomeTabela);

    tabela = BuscaTabela(bd, nomeTabela);

    if (tabela == NULL)
    {
        printf("ERRO: Tabela [%s] nao encontrada!\n\n", nomeTabela);
    }
    else
    {
        pula_espacos(comando, &i);

        if (comando[i] == '(')
        {
            temCampos = 1;

            pega_palavra_entre_parenteses(comando, &i, listaCampos);

            k = 0;

            while (listaCampos[k] != '\0' && erro == 0)
            {
                pula_espacos(listaCampos, &k);

                j = 0;

                while (listaCampos[k] != ',' && listaCampos[k] != '\0')
                {
                    nomeCampo[j] = listaCampos[k];
                    j++;
                    k++;
                }

                nomeCampo[j] = '\0';

                j--;

                while (j >= 0 && nomeCampo[j] == ' ')
                {
                    nomeCampo[j] = '\0';
                    j--;
                }

                campo = BuscaCampo(tabela, nomeCampo);

                if (campo == NULL)
                {
                    printf("ERRO: Campo [%s] nao encontrado!\n\n", nomeCampo);

                    erro = 1;
                }

                if (listaCampos[k] == ',')
                    k++;
            }
        }

        if (erro == 0)
        {
            while (comando[i] != '\0' && strncmp(&comando[i], "VALUES", 6) != 0)
                i++;

            if (comando[i] == '\0')
            {
                printf("ERRO: VALUES nao encontrado!\n\n");
                erro = 1;
            }
        }

        if (erro == 0)
        {
            i += 6;

            while (comando[i] != '(' && comando[i] != '\0')
                i++;

            if (comando[i] == '\0')
            {
                printf("ERRO: Valores nao encontrados!\n\n");
                erro = 1;
            }
            else
            {
                i++;
                inicioValores = i;
            }
        }

        if (erro == 0)
        {
            if (temCampos == 1)
                k = 0;
            else
                campo = tabela->pcampo;

            while (comando[i] != ')' && comando[i] != '\0' && erro == 0)
            {

                if (temCampos == 1)
                {
                    pula_espacos(listaCampos, &k);

                    if (listaCampos[k] == '\0')
                    {
                        printf("ERRO: Quantidade de valores maior que a quantidade de campos!\n\n");
                        erro = 1;
                    }
                    else
                    {
                        j = 0;

                        while (listaCampos[k] != ',' && listaCampos[k] != '\0')
                        {
                            nomeCampo[j] = listaCampos[k];
                            j++;
                            k++;
                        }

                        nomeCampo[j] = '\0';

                        j--;

                        while (j >= 0 && nomeCampo[j] == ' ')
                        {
                            nomeCampo[j] = '\0';
                            j--;
                        }

                        campo = BuscaCampo(tabela, nomeCampo);

                        if (listaCampos[k] == ',')
                            k++;
                    }
                }
                else
                {
                    if (campo == NULL)
                    {
                        printf("ERRO: Quantidade de valores maior que a quantidade de campos!\n\n");
                        erro = 1;
                    }
                }

                if (erro == 0)
                {
                    pula_espacos(comando, &i);

                    j = 0;

                    if (comando[i] == '\'')
                    {
                        i++;

                        while (comando[i] != '\'' && comando[i] != '\0')
                        {
                            valor[j] = comando[i];
                            j++;
                            i++;
                        }

                        if (comando[i] == '\'')
                            i++;
                    }

                    else
                    {
                        while (comando[i] != ',' && comando[i] != ')' && comando[i] != '\0')
                        {
                            valor[j] = comando[i];
                            j++;
                            i++;
                        }
                    }

                    valor[j] = '\0';

                    j--;

                    while (j >= 0 && valor[j] == ' ')
                    {
                        valor[j] = '\0';
                        j--;
                    }

                    if (erro == 0 && campo->pk == 'S')
                    {
                        if (ExisteDado(campo, valor))
                        {
                            printf("ERRO: Valor [%s] ja existe na chave primaria [%s]!\n\n", valor, campo->campo);
                            erro = 1;
                        }
                    }

                    if (erro == 0 && campo->fk != NULL)
                    {
                        if (!ExisteDado(campo->fk, valor))
                        {
                            printf("ERRO: Valor [%s] nao existe no campo referenciado [%s]!\n\n", valor, campo->fk->campo);

                            erro = 1;
                        }
                    }

                    if (temCampos == 0)
                        campo = campo->prox;

                    pula_espacos(comando, &i);

                    if (comando[i] == ',')
                        i++;
                }
            }
        }

        if (erro == 0)
        {

            if (temCampos == 0 && campo != NULL)
            {
                printf("ERRO: Quantidade de valores menor que a quantidade de campos!\n\n");
                erro = 1;
            }

            if (temCampos == 1)
            {
                pula_espacos(listaCampos, &k);

                if (listaCampos[k] != '\0')
                {
                    printf("ERRO: Quantidade de valores menor que a quantidade de campos!\n\n");
                    erro = 1;
                }
            }
        }

        if (erro == 0)
        {
            i = inicioValores;

            if (temCampos == 1)
                k = 0;
            else
                campo = tabela->pcampo;

            while (comando[i] != ')' && comando[i] != '\0')
            {
                if (temCampos == 1)
                {
                    pula_espacos(listaCampos, &k);

                    j = 0;

                    while (listaCampos[k] != ',' && listaCampos[k] != '\0')
                    {
                        nomeCampo[j] = listaCampos[k];
                        j++;
                        k++;
                    }

                    nomeCampo[j] = '\0';

                    j--;

                    while (j >= 0 && nomeCampo[j] == ' ')
                    {
                        nomeCampo[j] = '\0';
                        j--;
                    }

                    campo = BuscaCampo(tabela, nomeCampo);

                    if (listaCampos[k] == ',')
                        k++;
                }

                pula_espacos(comando, &i);

                j = 0;

                if (comando[i] == '\'')
                {
                    i++;

                    while (comando[i] != '\'' && comando[i] != '\0')
                    {
                        valor[j] = comando[i];
                        j++;
                        i++;
                    }

                    if (comando[i] == '\'')
                        i++;
                }
                else
                {
                    while (comando[i] != ',' && comando[i] != ')' && comando[i] != '\0')
                    {
                        valor[j] = comando[i];
                        j++;
                        i++;
                    }
                }

                valor[j] = '\0';

                j--;

                while (j >= 0 && valor[j] == ' ')
                {
                    valor[j] = '\0';
                    j--;
                }

                InsereDado(campo, valor);

                if (temCampos == 0)
                    campo = campo->prox;

                pula_espacos(comando, &i);

                if (comando[i] == ',')
                    i++;
            }

            printf("Registro inserido com sucesso!\n\n");
        }
    }
}