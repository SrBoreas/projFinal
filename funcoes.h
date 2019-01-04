#ifndef FUNCOES_H
#define FUNCOES_H // Include guards

#define FILEMAX 10000              // número ḿáximo de linhas lidas num ficheiro
#define LINEMAX 100                // número máximo de caractéres lidos numa linha de um ficheiro
#define CMAX 50                    // número máximo de comboios
#define FMAX 50                    // número máximo de ferrovias
#define M_PI 3.14159265            // valor da constante matemática "pi"
#define WINDOW_POSX 200            // posição inicial da janela: x
#define WINDOW_POSY 200            // posição inicial da janela: y

SDL_Renderer* g_pRenderer = NULL;  // variável global (renderer)
SDL_Window *window = NULL;         // vaŕiável global (janela gráfica)


/*
**Definição de estruturas
*/
typedef struct{
    char identificador[4];
    int  x;
    int  y;
    char cor[20];
    char tipo[3];
}PONTO; // pontos que unidos formam as ferrovias

typedef struct{
    char identificador[2];
    int  raio;
    char cor[20];
    char nome_ferrovia[4];
    char nome_ponto[4];
    int  viagens;
}COMBOIO;

typedef struct{
    int  r;
    int  g;
    int  b;
}COR;  // para passar de uma string para valores inteiros

typedef struct linha{
    char   identificador[4];
    PONTO  *dados;
    struct linha *anterior;
    struct linha *proximo;
}LINHA; // lista que servirá para formar linhas ferroviárias

/*
** protótipos de funções
*/
void flushinput(void); // dá flush ao stdin
int menu(void); // apresenta o menu e retorna a opcao escolhida
int lerconfig(char config[], char **dados, int *num_linhas); // processa os dados do ficheiro config
void tamanhoJanela(int *_width, int *_height, int num_linhas, char **dadosconfig); // retira do config as dimensões da janela
void processacomboio(COMBOIO **comboios, int *num_comboios, int num_linhas, char **dadosconfig); // forma os comboios
int processaferrovias(LINHA **ferrovias, int *num_ferrovias, int num_linhas, char **dadosconfig); // forma as ferrovias
void rewindlista(LINHA *lista); // rebobina a lista até ao head
void printlista(LINHA *lista); // imprime uma lista

/*
** protótipos de funções (parte gráfica)
*/
void renderRailroad (SDL_Renderer * g_pRenderer, LINHA *lista, int num_ferrovias);
void InitEverything(int width, int height, TTF_Font **_font, SDL_Window** _window, SDL_Renderer** g_pRenderer);
SDL_Window* CreateWindow(int width, int height);
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window);
void InitFont();
void filledCircleRGB(SDL_Renderer * g_pRenderer, int _circleX, int _circleY, int _circleR, int _r, int _g, int _b);
COR stringRGB (PONTO temporario);

#endif
