#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Biblioteca para limpar o buffer, verifica se é linux
#ifdef __linux__
#include <stdio_ext.h>
#endif

#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#elif __APPLE__
#include <unistd.h>
#endif

/*
Usei essa biblioteca unistd.h e windows.h para usar a função usleep(), que para o tempo de execução do programa.
Foi preciso usar um #ifdef para que os comandos funcionem tanto no Windows quanto no Linux.
FONTE: https://www.ufsm.br/pet/sistemas-de-informacao/2020/03/10/melhorando-suas-aplicacoes-em-c
*/
// https://cplusplus.com/forum/general/246274/

#define JOGOFACIL 5
#define JOGOMEDIO 10
#define JOGODIFICIL 20
#define ESPACOBRANCO 0
#define MAXENTRADA 25

// cores e formato de texto
#define ANSI_RESET "\x1b[0m" // desativa os efeitos anteriores
#define ANSI_BOLD "\x1b[1m"  // coloca o texto em negrito
#define ANSI_COLOR_BLACK "\x1b[30m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_WHITE "\x1b[37m"
#define ANSI_BG_COLOR_BLACK "\x1b[40m"
#define ANSI_BG_COLOR_RED "\x1b[41m"
#define ANSI_BG_COLOR_GREEN "\x1b[42m"
#define ANSI_BG_COLOR_YELLOW "\x1b[43m"
#define ANSI_BG_COLOR_BLUE "\x1b[44m"
#define ANSI_BG_COLOR_MAGENTA "\x1b[45m"
#define ANSI_BG_COLOR_CYAN "\x1b[46m"
#define ANSI_BG_COLOR_WHITE "\x1b[47m"

// macros para facilitar o uso
#define BOLD(string) ANSI_BOLD string ANSI_RESET
#define BLACK(string) ANSI_COLOR_BLACK string ANSI_RESET
#define BLUE(string) ANSI_COLOR_BLUE string ANSI_RESET
#define RED(string) ANSI_COLOR_RED string ANSI_RESET
#define GREEN(string) ANSI_COLOR_GREEN string ANSI_RESET
#define YELLOW(string) ANSI_COLOR_YELLOW string ANSI_RESET
#define BLUE(string) ANSI_COLOR_BLUE string ANSI_RESET
#define MAGENTA(string) ANSI_COLOR_MAGENTA string ANSI_RESET
#define CYAN(string) ANSI_COLOR_CYAN string ANSI_RESET
#define WHITE(string) ANSI_COLOR_WHITE string ANSI_RESET
#define BG_BLACK(string) ANSI_BG_COLOR_BLACK string ANSI_RESET
#define BG_BLUE(string) ANSI_BG_COLOR_BLUE string ANSI_RESET
#define BG_RED(string) ANSI_BG_COLOR_RED string ANSI_RESET
#define BG_GREEN(string) ANSI_BG_COLOR_GREEN string ANSI_RESET
#define BG_YELLOW(string) ANSI_BG_COLOR_YELLOW string ANSI_RESET
#define BG_BLUE(string) ANSI_BG_COLOR_BLUE string ANSI_RESET
#define BG_MAGENTA(string) ANSI_BG_COLOR_MAGENTA string ANSI_RESET
#define BG_CYAN(string) ANSI_BG_COLOR_CYAN string ANSI_RESET
#define BG_WHITE(string) ANSI_BG_COLOR_WHITE string ANSI_RESET

// caracteres uteis para tabelas
#define TAB_HOR "\u2501" // ━ (horizontal)
#define TAB_VER "\u2503" // ┃ (vertical)
#define TAB_TL "\u250F"  // ┏ (top-left)
#define TAB_ML "\u2523"  // ┣ (middle-left)
#define TAB_BL "\u2517"  // ┗ (bottom-left)
#define TAB_TJ "\u2533"  // ┳ (top-join)
#define TAB_MJ "\u254B"  // ╋ (middle-join)
#define TAB_BJ "\u253B"  // ┻ (bottom-join)
#define TAB_TR "\u2513"  // ┓ (top-right)
#define TAB_MR "\u252B"  // ┫ (middle-right)
#define TAB_BR "\u251B"  // ┛ (bottom-right)

typedef struct
{
    int **array_do_jogo;
    int numero_da_matriz;
    int espaco_vazio_i;
    int espaco_vazio_j;
    int numero_de_jogadas;
} Jogo;

void limpar_buffer();
void dorme_programa(int milesimo_de_segundo);
void remove_nova_linha(char *str);

Jogo *criaMatriz(int numero_da_matriz);
void liberaMatriz(Jogo *matriz);
void mostrar_matriz(Jogo *matriz);
void embaralha(Jogo *matriz, int nivel_de_dificuldade);

int move_baixo(Jogo *matriz);
int move_alto(Jogo *matriz);
int move_esquerda(Jogo *matriz);
int move_direita(Jogo *matriz);

void encontrar_pecas(Jogo *matriz, int numero, int *linha, int *coluna);
int move_peca_e_encontra_peca(Jogo *matriz, int numero);
int verificacao_vencer(Jogo *matriz);

int e_numero_ou_nao(const char *entrada);

void salvar(Jogo *matriz, const char *nomeArquivo);
void jogar(Jogo **matriz, int *menu, int *inicio_meio_fim);
void carregar(Jogo *matriz, const char *nomeArquivo);
void menu();

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int menu = 0;
    int inicio_meio_fim = 1;
    char sentenca;
    char nomeArquivo[MAXENTRADA];
    Jogo *matriz = NULL;
    Jogo *matriz_salva = NULL;

    while (inicio_meio_fim != 0)
    {
        while (menu == 0)
        {
            printf(WHITE(BG_BLUE("%10s")), "Bem vindo ao Quebra-Cabeças\n");
            printf(WHITE(BG_BLUE("%10s")), "X. Sair do Jogo\n");
            printf(WHITE(BG_BLUE("%10s")), "N. Iniciar um novo jogo\n");
            printf(WHITE(BG_BLUE("%10s")), "C. Continuar o jogo atual\n");
            printf(WHITE(BG_BLUE("%10s")), "S. Continuar um jogo salvo\n");
            printf(WHITE(BG_BLUE("%10s")), "A. Ajuda com as instruções\n");
            printf(BG_BLACK(WHITE("%10s")), "Escolha a opção (Digite voltar em qualquer lugar para voltar ao menu principal): ");

            scanf(" %c", &sentenca);
            limpar_buffer();

            switch (sentenca)
            {
            case 'X':
            case 'x':
                menu = 1;
                inicio_meio_fim = 0;
                break;
            case 'N':
            case 'n':
                jogar(&matriz, &menu, &inicio_meio_fim);
                break;
            case 'C':
            case 'c':
                if (matriz == NULL)
                {
                    printf("%10s", "Nenhum jogo foi iniciado \n");
                }
                else
                {
                    jogar(&matriz, &menu, &inicio_meio_fim);
                }
                break;
            case 'S':
            case 's':
                matriz_salva = (Jogo *)malloc(sizeof(Jogo));

                if (matriz_salva == NULL)
                {
                    printf("%10s", "Erro ao alocar memória para matriz salva.\n");
                    break;
                }

                printf("%10s", "Digite o nome do arquivo salvo: ");
                fgets(nomeArquivo, MAXENTRADA, stdin);
                remove_nova_linha(nomeArquivo);

                carregar(matriz_salva, nomeArquivo);

                if (matriz_salva->array_do_jogo != NULL)
                {
                    printf("%10s", "Bem vindo ao Quebra-Cabeças\n");
                    jogar(&matriz_salva, &menu, &inicio_meio_fim);
                }
                else
                {
                    printf("%10s", "Falha ao carregar o jogo.\n");
                    free(matriz_salva);
                    matriz_salva = NULL;
                }
                break;
            case 'A':
            case 'a':
                printf("%10s", "Esse jogo é um quebra-cabeças, onde você poderá escolher a dimensão do tabuleiro quando iniciar um novo jogo.\n");
                printf("%10s", "Ganha o jogo quando todas as peças estiverem ordenadas e o espaço em branco estiver na última posição.\n");
                printf("%10s", "Você pode utilizar funções como salvar [nomedoarquivo.extensão].\n");
                printf("%10s", "Caso você tenha voltado ao menu principal, pode continuar jogando apertando C.\n");
                break;
            default:
                printf("%10s", "Opção inválida, tente novamente.\n");
                break;
            }
        }
    }

    if (matriz != NULL)
    {
        liberaMatriz(matriz);
    }
    if (matriz_salva != NULL)
    {
        liberaMatriz(matriz_salva);
    }

    return 0;
}

void limpar_buffer()
{
#ifdef __linux__
    __fpurge(stdin);
#elif defined(_WIN32)
    fflush(stdin);
#elif __APPLE__
    int c = getchar();
    while (c != '\n' && c != EOF)
        ;
#else
    int c = getchar();
    while (c != '\n' && c != EOF)
        ;
#endif
}

void dorme_programa(int milesimo_de_segundo)
{
#ifdef __linux__
    usleep(milesimo_de_segundo * 1000);
#elif __WIN32
    Sleep(milesimo_de_segundo);
#elif __MAC__
    usleep(milesimo_de_segundo * 1000);
#endif
}

void remove_nova_linha(char *str)
{
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
        str[len - 1] = '\0';
    }
}

Jogo *criaMatriz(int numero_da_matriz)
{
    Jogo *matriz = (Jogo *)malloc(sizeof(Jogo));
    matriz->numero_da_matriz = numero_da_matriz;
    matriz->array_do_jogo = (int **)malloc(numero_da_matriz * sizeof(int *));
    for (int i = 0; i < numero_da_matriz; i++)
    {
        matriz->array_do_jogo[i] = (int *)malloc(numero_da_matriz * sizeof(int));
    }
    int contabilizador = 1;
    for (int i = 0; i < matriz->numero_da_matriz; i++)
    {
        for (int j = 0; j < matriz->numero_da_matriz; j++)
        {
            if (i == matriz->numero_da_matriz - 1 && j == matriz->numero_da_matriz - 1)
            {
                matriz->array_do_jogo[i][j] = ESPACOBRANCO;
                matriz->espaco_vazio_i = i;
                matriz->espaco_vazio_j = j;
            }
            else
            {
                matriz->array_do_jogo[i][j] = contabilizador;
            }
            contabilizador++;
        }
    }
    matriz->numero_de_jogadas = 0;
    return matriz;
}


void mostrar_matriz(Jogo *matriz)
{
    int tamanho = matriz->numero_da_matriz;

    // Imprime a linha superior da tabela
    printf(TAB_TL);
    for (int i = 0; i < tamanho - 1; i++)
    {
        printf(TAB_HOR TAB_TJ); // Horizontal + junção
    }
    printf(TAB_HOR TAB_TR "\n"); // Fecha a linha superior

    // Imprime o corpo da tabela (linhas com números)
    for (int i = 0; i < tamanho; i++)
    {
        printf(TAB_VER); // Imprime a borda vertical esquerda
        for (int j = 0; j < tamanho; j++)
        {
            if (matriz->array_do_jogo[i][j] != ESPACOBRANCO)
            {
                printf("%d", matriz->array_do_jogo[i][j]); // Número com largura fixa de 3 caracteres
            }
            else

            {
                printf("%s", " "); // Espaço vazio com a mesma largura
            }

            // Garante a borda vertical direita
            if (j == tamanho - 1)
            {
                printf(" %s ", TAB_VER); // Imprime a borda direita no final de cada linha
            }
            else
            {
                printf(" "); // Espaçamento entre os números
            }
        }
        printf("\n");

        // Imprime a linha de separação entre as linhas de números
        if (i < tamanho - 1)
        {
            printf(TAB_ML); // Borda esquerda para linha intermediária
            for (int k = 0; k < tamanho - 1; k++)
            {
                printf(TAB_HOR TAB_MJ); // Horizontal + junção
            }
            printf(TAB_HOR TAB_MR "\n"); // Fecha a linha intermediária
        }
    }

    // Imprime a linha inferior da tabela
    printf(TAB_BL);
    for (int i = 0; i < tamanho - 1; i++)
    {
        printf(TAB_HOR TAB_BJ); // Horizontal + junção
    }
    printf(TAB_HOR TAB_BR "\n"); // Fecha a linha inferior
}



void liberaMatriz(Jogo *matriz)
{
    for (int i = 0; i < matriz->numero_da_matriz; i++)
    {
        free(matriz->array_do_jogo[i]);
    }
    free(matriz->array_do_jogo);
    free(matriz);
}
int move_baixo(Jogo *matriz)
{
    if (matriz->espaco_vazio_i == 0)
    {
        return 0; // Não pode mover para baixo se já está na linha mais baixa
    }
    // Mover a peça para o espaço vazio
    matriz->array_do_jogo[matriz->espaco_vazio_i][matriz->espaco_vazio_j] =
        matriz->array_do_jogo[matriz->espaco_vazio_i - 1][matriz->espaco_vazio_j];
    matriz->array_do_jogo[matriz->espaco_vazio_i - 1][matriz->espaco_vazio_j] = ESPACOBRANCO;
    matriz->espaco_vazio_i--; // Atualiza a posição do espaço vazio
    return 1;                 // Movimento bem-sucedido
}

int move_alto(Jogo *matriz)
{
    if (matriz->espaco_vazio_i == matriz->numero_da_matriz - 1)
    {
        return 0; // Não pode mover para cima se já está na linha mais alta
    }
    matriz->array_do_jogo[matriz->espaco_vazio_i][matriz->espaco_vazio_j] =
        matriz->array_do_jogo[matriz->espaco_vazio_i + 1][matriz->espaco_vazio_j];
    matriz->array_do_jogo[matriz->espaco_vazio_i + 1][matriz->espaco_vazio_j] = ESPACOBRANCO;
    matriz->espaco_vazio_i++; // Atualiza a posição do espaço vazio
    return 1;                 // Movimento bem-sucedido
}

int move_esquerda(Jogo *matriz)
{
    if (matriz->espaco_vazio_j == matriz->numero_da_matriz - 1)
    {
        return 0; // Movimento inválido se o espaço vazio estiver na borda direita
    }

    // Move a peça à direita do espaço vazio para a esquerda
    matriz->array_do_jogo[matriz->espaco_vazio_i][matriz->espaco_vazio_j] = matriz->array_do_jogo[matriz->espaco_vazio_i][matriz->espaco_vazio_j + 1];
    matriz->array_do_jogo[matriz->espaco_vazio_i][matriz->espaco_vazio_j + 1] = ESPACOBRANCO; // Espaço vazio
    matriz->espaco_vazio_j++;                                                                 // Atualiza a posição do espaço vazio
    return 1;                                                                                 // Movimento realizado com sucesso
}

int move_direita(Jogo *matriz)
{
    if (matriz->espaco_vazio_j == 0)
    {
        return 0; // Movimento inválido se o espaço vazio estiver na borda esquerda
    }

    // Move a peça à esquerda do espaço vazio para a direita
    matriz->array_do_jogo[matriz->espaco_vazio_i][matriz->espaco_vazio_j] = matriz->array_do_jogo[matriz->espaco_vazio_i][matriz->espaco_vazio_j - 1];
    matriz->array_do_jogo[matriz->espaco_vazio_i][matriz->espaco_vazio_j - 1] = ESPACOBRANCO; // Espaço vazio
    matriz->espaco_vazio_j--;                                                                 // Atualiza a posição do espaço vazio
    return 1;                                                                                 // Movimento realizado com sucesso
}

void embaralha(Jogo *matriz, int nivel_de_dificuldade)
{
    srand(time(NULL));
    for (int i = 0; i < nivel_de_dificuldade * matriz->numero_da_matriz; i++) // Nível de dificuldade vezes a quantidade de linhas da matriz
    {
        // Harvey M.; DEITEL, Paul J. C++: como programar. 5. ed. São Paulo: Pearson Prentice Hall, 2006. p.235
        int movimento = rand() % 4;
        switch (movimento)
        {
        case 0:
            move_baixo(matriz);
            break;
        case 1:
            move_alto(matriz);
            break;
        case 2:
            move_direita(matriz);
            break;
        case 3:
            move_esquerda(matriz);
            break;
        }
    }
}

void encontrar_pecas(Jogo *matriz, int numero, int *linha, int *coluna)
{
    for (int i = 0; i < matriz->numero_da_matriz; i++)
    {
        for (int j = 0; j < matriz->numero_da_matriz; j++)
        {
            if (matriz->array_do_jogo[i][j] == numero)
            {
                /*
                Essa função tem um for alinhado, que compara cada valor da matriz com o número digitado pelo usuário,
                como o número será uma string, deverá ser convertido primeiro utilizando a função isdigit da função ctype.h,
                depois que convertido e com o for percorrido, é atribuido no conteúdo da memória a posição da váriavel linha e da váriavel coluna. Nesse sentido,
                essa função será utilizada depois que o usuário digitar a dificuldade
                */
                *linha = i;
                *coluna = j;
                return;
            }
        }
    }
    *linha = -1; // Se for atribuido -1 na váriavel linha ou coluna, a posição digitada pelo usuário não é válida, logo após fazer o códico, é preciso passar por
                 // uma reavaliação para um tratamento de erros
    *coluna = -1;
}

int move_peca_e_encontra_peca(Jogo *matriz, int numero)
{
    int linha_peca, coluna_peca;
    encontrar_pecas(matriz, numero, &linha_peca, &coluna_peca);

    // Verifica se a peça foi encontrada
    if (linha_peca == -1 || coluna_peca == -1)
    {
        printf("Peça não encontrada.\n");
        return 0; // A peça não foi encontrada
    }

    // Verifica se a peça pode ser movida para o espaço vazio
    if (linha_peca == matriz->espaco_vazio_i - 1 && coluna_peca == matriz->espaco_vazio_j)
    {
        move_baixo(matriz); // A peça está acima do espaço vazio
    }
    else if (linha_peca == matriz->espaco_vazio_i + 1 && coluna_peca == matriz->espaco_vazio_j)
    {
        move_alto(matriz); // A peça está abaixo do espaço vazio
    }
    else if (linha_peca == matriz->espaco_vazio_i && coluna_peca == matriz->espaco_vazio_j - 1)
    {
        move_direita(matriz); // A peça está à esquerda do espaço vazio
    }
    else if (linha_peca == matriz->espaco_vazio_i && coluna_peca == matriz->espaco_vazio_j + 1)
    {
        move_esquerda(matriz); // A peça está à direita do espaço vazio
    }
    else
    {
        printf("Movimento inválido.\n"); // Movimento não permitido
        return 0;                        // Movimento inválido
    }

    matriz->numero_de_jogadas++; // Incrementa o número de jogadas
    return 1;                    // Movimento bem-sucedido
}

int e_numero_ou_nao(const char *entrada)
{
    // Verifica se a string é vazia
    if (*entrada == '\0')
        return 0;

    while (*entrada)
    {
        if (!isdigit(*entrada))
        {
            return 0; // Retorna 0 se não for um dígito
        }
        entrada++;
    }
    return 1; // Retorna 1 se todos os caracteres forem dígitos
}

int verificacao_vencer(Jogo *matriz)
{
    int contador = 1;
    for (int i = 0; i < matriz->numero_da_matriz; i++)
    {
        for (int j = 0; j < matriz->numero_da_matriz; j++)
        {
            if (i == matriz->numero_da_matriz - 1 && j == matriz->numero_da_matriz - 1)
            {
                if (matriz->array_do_jogo[i][j] != 0)
                {
                    return 0;
                }
            }
            else
            {
                if (matriz->array_do_jogo[i][j] != contador)
                {
                    return 0;
                }
                contador++;
            }
        }
    }
    return 1;
}

void salvar(Jogo *matriz, const char *nomeArquivo)
{
    /*
    Os caracteres são compostos por bits, os campos são compostos por caracteres. Um campo é um grupo de caracteres que possuem um significado.
    Nessa lógica, os itens formam uma hierarquia de dados, os quais itens de dados se tornam maiores e mais complexos na estrutura, formando
    registros. Um arquivo é um grupo de registros relacionados. por isso para facilitar cada campo possui uma chave de registro que relaciona a indentificação
    de determinada coisa.
        Cada arquivo determina um marcador, ou em um byte específico, cujo o número é gravado em uma estrutura administrativa de dados,
        Os strems associados são a entrada-padrão, a saída-padrão e o erro-padrão, que fornece uma saída de comunicação. Abrir um arquivo retorna um ponteiro
        para uma estrutura (registros, chaves, significados) que contém informações para processar um arquivo no descritor de arquivos (FCB). A administração
        dos arquivos são formadas usando ponteiros de arquivo (stdin, stdout, stderr).

    */
    FILE *cfPtr;
    if ((cfPtr = fopen(nomeArquivo, "w")) == NULL)
    {
        printf("Arquivo não pode ser criado tente novamente");
        return;
    }
    else
    {
        fprintf(cfPtr, "%d", matriz->numero_da_matriz);
        fprintf(cfPtr, "\n");
        for (int i = 0; i < matriz->numero_da_matriz; i++)
        {
            for (int j = 0; j < matriz->numero_da_matriz; j++)
            {
                fprintf(cfPtr, "%-5d", matriz->array_do_jogo[i][j]);
            }
            fprintf(cfPtr, "\n");
        }
        fprintf(cfPtr, "%d", matriz->numero_de_jogadas);
    }
    fclose(cfPtr);
}

void jogar(Jogo **matriz, int *menu, int *inicio_meio_fim)
{
    char *digita = malloc(MAXENTRADA * sizeof(char));
    int numero_da_peca;
    int dimensao_tabuleiro;
    char nivel_de_dificuldade;
    char nomeArquivo[10];

    do
    {
        printf(BG_BLACK(WHITE("%-10s")), "Digite a dimensão do tabuleiro (3 ≤ n ≤ 10): ");
        scanf(" %d", &dimensao_tabuleiro);
        limpar_buffer();
    } while (dimensao_tabuleiro > 10 || dimensao_tabuleiro < 3);

    *matriz = criaMatriz(dimensao_tabuleiro);
    printf(BG_BLACK(WHITE("%-10s")), "Tabuleiro ordenado!\n");

    printf(BG_BLACK(WHITE("%-10s")), "Digite o nível de dificuldade para embaralhar, fácil (F), Médio (M) ou difícil (D): ");
    scanf(" %c", &nivel_de_dificuldade);

    switch (nivel_de_dificuldade)
    {
    case 'f':
    case 'F':
        printf(BG_BLACK(WHITE("%-10s")), "Tabuleiro embaralhado. Vamos começar o jogo! \n");
        embaralha(*matriz, JOGOFACIL);
        break;
    case 'm':
    case 'M':
        printf(BG_BLACK(WHITE("%-10s")), "Tabuleiro embaralhado. Vamos começar o jogo! \n");
        embaralha(*matriz, JOGOMEDIO);
        break;
    case 'd':
    case 'D':
        printf(BG_BLACK(WHITE("%-10s")), "Tabuleiro embaralhado. Vamos começar o jogo! \n");
        embaralha(*matriz, JOGODIFICIL);
        break;
    default:
        break;
    }

    limpar_buffer();
    while (1)
    {
        mostrar_matriz(*matriz);
        printf(BG_BLACK(WHITE("%-10s")), "Escolha a opção (digite \"voltar\" em qualquer lugar para voltar ao menu principal): ");
        fgets(digita, MAXENTRADA, stdin);

        remove_nova_linha(digita);

        if (strncmp(digita, "salvar", 6) == 0)
        {
            if (sscanf(digita + 7, "%s", nomeArquivo) == 1)
            {
                salvar(*matriz, nomeArquivo);
                printf(BG_BLACK(WHITE("%-10s")), "Jogo salvo com sucesso \n");
            }
            else
            {
                printf(BG_BLACK(WHITE("%-10s")), "Nome do arquivo inválido \n");
                limpar_buffer();
            }
            continue;
        }

        if (strcmp(digita, "voltar") == 0)
        {
            *menu = 0;
            free(digita);
            return;
        }

        if (e_numero_ou_nao(digita))
        {
            numero_da_peca = atoi(digita);

            if (move_peca_e_encontra_peca(*matriz, numero_da_peca))
            {
                if (verificacao_vencer(*matriz) == 1)
                {
                    mostrar_matriz(*matriz);
                    printf("%s", "Parabéns, você venceu o jogo!\n");
                    free(digita);
                    *inicio_meio_fim = 0;
                    *menu = 1;
                    return;
                }
                printf("%10s %3d \n", "Quantidade de jogadas até agora:", (*matriz)->numero_de_jogadas);
            }
            else
            {
                printf("%s %d\n", "Não é possível mover a peça", numero_da_peca);
            }
        }
        else
        {
            printf("%s", "Número não encontrado.\n");
        }
    }
}

void carregar(Jogo *matriz, const char *nomeArquivo)
{
    FILE *cfPtr;

    cfPtr = fopen(nomeArquivo, "r");
    if (cfPtr == NULL)
    {
        printf("Erro ao abrir o arquivo. Verifique se o nome do arquivo está correto.\n");
        return;
    }

    if (fscanf(cfPtr, "%d", &(matriz->numero_da_matriz)) != 1)
    {
        printf("Erro ao ler o tamanho da matriz.\n");
        fclose(cfPtr);
        return;
    }

    matriz->array_do_jogo = (int **)malloc(matriz->numero_da_matriz * sizeof(int *));
    for (int i = 0; i < matriz->numero_da_matriz; i++)
    {
        matriz->array_do_jogo[i] = (int *)malloc(matriz->numero_da_matriz * sizeof(int));
    }

    for (int i = 0; i < matriz->numero_da_matriz; i++)
    {
        for (int j = 0; j < matriz->numero_da_matriz; j++)
        {
            if (fscanf(cfPtr, "%d", &(matriz->array_do_jogo[i][j])) != 1)
            {
                printf("Erro ao ler o estado do tabuleiro.\n");
                fclose(cfPtr);
                return;
            }

            if (matriz->array_do_jogo[i][j] == 0)
            {
                matriz->espaco_vazio_i = i;
                matriz->espaco_vazio_j = j;
            }
        }
    }

    if (fscanf(cfPtr, "%d", &(matriz->numero_de_jogadas)) != 1)
    {
        printf("Erro ao ler o número de jogadas.\n");
        fclose(cfPtr);
        return;
    }

    printf("Jogo carregado com sucesso.\n");
    fclose(cfPtr);
}
