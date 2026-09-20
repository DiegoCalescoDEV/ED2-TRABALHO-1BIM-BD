void pega_palavra_entre_parenteses(char linha[], int *i, char texto[])
{
    int j = 0;

    while (linha[*i] != '(' && linha[*i] != '\0')
        (*i)++;

    if (linha[*i] == '(')
        (*i)++;

    while (linha[*i] != ')' && linha[*i] != '\0')
    {
        texto[j] = linha[*i];
        j++;
        (*i)++;
    }

    texto[j] = '\0';
}

void pula_espacos(char linha[], int *i)
{
    while (linha[*i] == ' ')
        (*i)++;
}

void pega_palavra(char linha[], int *i, char palavra[])
{
    int j = 0;

    pula_espacos(linha, i);

    while (linha[*i] != ' ' && linha[*i] != '\0' && linha[*i] != '\n')
    {
        palavra[j] = linha[*i];
        j++;
        (*i)++;
    }

    palavra[j] = '\0';
}
