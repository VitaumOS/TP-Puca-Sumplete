#ifndef FUNCOES_H
#define FUNCOES_H

#define M 266
#define TAM 256
#define QUANTJOGADOR 5

typedef struct{
    
    int *linha;
    int *coluna;

}Soma;

typedef struct{

    int **mat;
    int **gabarito;
    int **resposta;

    int tam;
    int quant_manter;
    int quant_remover;

    char tam_c;
    char dificuldade;

    char opcao[M];

}Tabela;

typedef struct{

    char nome[TAM];
    int tam_nome;
    int tempoF;
    int tempoI;
    int TempoT;
    
}Jogador;

typedef struct{

    char ***nome;
    int **tempo;
}Ranking;

typedef struct{

    Soma s;
    Jogador j;
    Tabela t;

    int parametro;
}Geral;




// cores e formato de texto
#define ANSI_RESET            "\x1b[0m"  // desativa os efeitos anteriores
#define ANSI_BOLD             "\x1b[1m"  // coloca o texto em negrito
#define ANSI_COLOR_BLACK      "\x1b[30m"
#define ANSI_COLOR_RED        "\x1b[31m"
#define ANSI_COLOR_GREEN      "\x1b[32m"
#define ANSI_COLOR_YELLOW     "\x1b[33m"
#define ANSI_COLOR_BLUE       "\x1b[34m"
#define ANSI_COLOR_MAGENTA    "\x1b[35m"
#define ANSI_COLOR_CYAN       "\x1b[36m"
#define ANSI_COLOR_WHITE      "\x1b[37m"
#define ANSI_BG_COLOR_BLACK   "\x1b[40m"
#define ANSI_BG_COLOR_RED     "\x1b[41m"
#define ANSI_BG_COLOR_GREEN   "\x1b[42m"
#define ANSI_BG_COLOR_YELLOW  "\x1b[43m"
#define ANSI_BG_COLOR_BLUE    "\x1b[44m"
#define ANSI_BG_COLOR_MAGENTA "\x1b[45m"
#define ANSI_BG_COLOR_CYAN    "\x1b[46m"
#define ANSI_BG_COLOR_WHITE   "\x1b[47m"

// macros para facilitar o uso
#define BOLD(string)       ANSI_BOLD             string ANSI_RESET
#define BLACK(string)      ANSI_COLOR_BLACK      string ANSI_RESET
#define BLUE(string)       ANSI_COLOR_BLUE       string ANSI_RESET
#define RED(string)        ANSI_COLOR_RED        string ANSI_RESET
#define GREEN(string)      ANSI_COLOR_GREEN      string ANSI_RESET
#define YELLOW(string)     ANSI_COLOR_YELLOW     string ANSI_RESET
#define BLUE(string)       ANSI_COLOR_BLUE       string ANSI_RESET
#define MAGENTA(string)    ANSI_COLOR_MAGENTA    string ANSI_RESET
#define CYAN(string)       ANSI_COLOR_CYAN       string ANSI_RESET
#define WHITE(string)      ANSI_COLOR_WHITE      string ANSI_RESET
#define BG_BLACK(string)   ANSI_BG_COLOR_BLACK   string ANSI_RESET
#define BG_BLUE(string)    ANSI_BG_COLOR_BLUE    string ANSI_RESET
#define BG_RED(string)     ANSI_BG_COLOR_RED     string ANSI_RESET
#define BG_GREEN(string)   ANSI_BG_COLOR_GREEN   string ANSI_RESET
#define BG_YELLOW(string)  ANSI_BG_COLOR_YELLOW  string ANSI_RESET
#define BG_BLUE(string)    ANSI_BG_COLOR_BLUE    string ANSI_RESET
#define BG_MAGENTA(string) ANSI_BG_COLOR_MAGENTA string ANSI_RESET
#define BG_CYAN(string)    ANSI_BG_COLOR_CYAN    string ANSI_RESET
#define BG_WHITE(string)   ANSI_BG_COLOR_WHITE   string ANSI_RESET

// caracteres uteis para tabelas
#define TAB_HOR "\u2501" // ━ (horizontal)
#define TAB_VER "\u2503" // ┃ (vertical)
#define TAB_TL  "\u250F" // ┏ (top-left)
#define TAB_ML  "\u2523" // ┣ (middle-left)
#define TAB_BL  "\u2517" // ┗ (bottom-left)
#define TAB_TJ  "\u2533" // ┳ (top-join)
#define TAB_MJ  "\u254B" // ╋ (middle-join)
#define TAB_BJ  "\u253B" // ┻ (bottom-join)
#define TAB_TR  "\u2513" // ┓ (top-right)
#define TAB_MR  "\u252B" // ┫ (middle-right)
#define TAB_BR  "\u251B" // ┛ (bottom-right)
 

void salvaArquivo(int l, Tabela t, Soma s, Jogador j);
void limparBuffer();
void opcoes();
void limpamatriz(int **mat, int n);
void montarTab(Tabela t, Soma vet);
void limpavetor(int *vet);
void ranking(char *nome,int tempo, int n, int param);
void mostraRanking(Ranking r);

Soma criaLinhaColuna(Tabela tab);

Geral jogo(Tabela t, Soma s, Jogador j, int parametro);
Geral abreArquivo(char * nome_arq);

Ranking armazenaRanking(Ranking r);
Ranking alocaRanking(Ranking r);
Ranking adicionaNovoRanking(char *nome,int tempo, int n, Ranking r);

int verificaVitoria(Tabela t);

char * dividePalavra(char *op);

int * criaVetor(int n);

int ** resposta(int l, char *op, int** resposta, int n);
int ** geravalores(int **mat, int n);
int ** criaMatriz(int n);
int ** criarMatrizEspelho(int n);
int ** resolver(Tabela t);
int ** dica(Tabela t);



#endif