#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h> 

struct reg
{
    char nome[20];
    char cpf[15];  
    char tel[20];
    char email[20];
    char status;
};

int tamanho(FILE *arq);
void cadastrar(FILE *arq);
void consultar(FILE *arq);
void geraarqtxt(FILE *arq);
void excluir(FILE *arq);

int main()
{
    setlocale(LC_ALL, "Portuguese"); 

    int op;
    FILE *arq;

    if ((arq = fopen("socio_torcedores.dat", "rb+")) == NULL)
    {
        if ((arq = fopen("socio_torcedores.dat", "wb+")) == NULL)
        {
            printf("Falha ao abrir o arquivo!\n");
            system("pause");
            return 1;
        }
    }

    do
    {
        system("CLS");
        printf("\n======= Cadastro de Sócio-Torcedores ======= \n");
        printf("1. Cadastrar Sócio-Torcedor\n");
        printf("2. Consultar por Código\n");
        printf("3. Gerar Arquivo de Sócios\n");
        printf("4. Excluir Sócio\n");
        printf("5. Sair\n");
        printf("===========Total de Sócios: %d =\n", tamanho(arq));
        printf("Opção: ");
        scanf("%d", &op);
        getchar(); 

        switch (op)
        {
        case 1:
            cadastrar(arq);
            break;
        case 2:
            consultar(arq);
            break;
        case 3:
            geraarqtxt(arq);
            break;
        case 4:
            excluir(arq);
            break;
        case 5:
            fclose(arq);
            break;
        }
    } while (op != 5);

    return 0;
}

void cadastrar(FILE *arq)
{
    struct reg socio;
    char confirma;
    socio.status = ' ';
    getchar(); 

    printf("Cadastrando novo sócio-torcedor:\n");
    printf("\nNúmero do registro: %d\n", tamanho(arq) + 1);
    printf("Nome.............: ");
    fgets(socio.nome, 20, stdin);
    socio.nome[strcspn(socio.nome, "\n")] = '\0'; 
    printf("CPF..............: ");
    fgets(socio.cpf, 15, stdin);
    socio.cpf[strcspn(socio.cpf, "\n")] = '\0';
    printf("Telefone.........: ");
    fgets(socio.tel, 20, stdin);
    socio.tel[strcspn(socio.tel, "\n")] = '\0';
    printf("E-mail...........: ");
    fgets(socio.email, 20, stdin);
    socio.email[strcspn(socio.email, "\n")] = '\0';
    printf("\nConfirma <s/n>: ");
    scanf(" %c", &confirma);

    if (toupper(confirma) == 'S')
    {
        printf("\nGravando...\n\n");
        fseek(arq, 0, SEEK_END);
        fwrite(&socio, sizeof(struct reg), 1, arq);
    }
    system("pause");
}

void consultar(FILE *arq)
{
    struct reg socio;
    int nr;
    printf("\nConsulta pelo código\n");
    printf("\nInforme o Código: ");
    scanf("%d", &nr);

    if ((nr <= tamanho(arq)) && (nr > 0))
    {
        fseek(arq, (nr - 1) * sizeof(struct reg), SEEK_SET);
        fread(&socio, sizeof(struct reg), 1, arq);
        if (socio.status == ' ')
        {
            printf("\nNome......: %s", socio.nome);
            printf("\nCPF.......: %s", socio.cpf);
            printf("\nTelefone..: %s", socio.tel);
            printf("\nE-mail....: %s\n\n", socio.email);
        }
        else
            printf("\nRegistro excluído! \n");
    }
    else
    {
        printf("\nNúmero de registro inválido!\n");
    }
    system("pause");
}

void geraarqtxt(FILE *arq)
{
    char nomearq[20];
    printf("Nome do arquivo texto: ");
    scanf("%s", nomearq);
    strcat(nomearq, ".txt");
    FILE *arqtxt = fopen(nomearq, "w");
    if (!arqtxt)
    {
        printf("Não foi possível criar esse arquivo!\n");
        system("pause");
        return;
    }
    fprintf(arqtxt, "Nome                CPF           Telefone    E-mail                   Status\n");
    fprintf(arqtxt, "=========================================================================\n");
    int nr;
    struct reg socio;
    for (nr = 0; nr < tamanho(arq); nr++)
    {
        fseek(arq, nr * sizeof(struct reg), SEEK_SET);
        fread(&socio, sizeof(struct reg), 1, arq);
        fprintf(arqtxt, "%-20s%-15s%-12s%-25s- %c\n", socio.nome, socio.cpf, socio.tel, socio.email, socio.status);
    }
    fprintf(arqtxt, "=========================================================================\n");
    fclose(arqtxt);
}

void excluir(FILE *arq)
{
    struct reg socio;
    char confirma;
    int nr;

    printf("\nInforme o código do sócio para excluir: ");
    scanf("%d", &nr);

    if ((nr <= tamanho(arq)) && (nr > 0))
    {
        fseek(arq, (nr - 1) * sizeof(struct reg), SEEK_SET);
        fread(&socio, sizeof(struct reg), 1, arq);
        if (socio.status == ' ')
        {
            printf("\nNome......: %s", socio.nome);
            printf("\nCPF.......: %s", socio.cpf);
            printf("\nTelefone..: %s", socio.tel);
            printf("\nE-mail....: %s\n", socio.email);
            printf("\nConfirma a exclusão: <s/n>\n");
            getchar(); 
            scanf(" %c", &confirma);

            if (toupper(confirma) == 'S')
            {
                printf("\nExcluindo...\n\n");
                fseek(arq, (nr - 1) * sizeof(struct reg), SEEK_SET);
                socio.status = '*';
                fwrite(&socio, sizeof(struct reg), 1, arq);
            }
        }
        else
            printf("Registro inexistente! \n");
    }
    else
    {
        printf("\nNúmero de registro inválido!\n");
    }
    system("pause");
}

int tamanho(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(struct reg);
}