#ifndef FUNCOES_H
#define FUNCOES_H // Include guards

#define FILEMAX 10000              // número ḿáximo de linhas lidas num ficheiro
#define LINEMAX 100                // número máximo de caractéres lidos numa linha de um ficheiro
#define CMAX 50                    // número máximo de comboios
#define FMAX 50                    // número máximo de ferrovias
#define M_PI 3.14159265            // valor da constante matemática "pi"
#define WINDOW_POSX 200            // posição inicial da janela: x
#define WINDOW_POSY 200            // posição inicial da janela: y

/*
**Definição de estruturas
*/
typedef struct{
    char identificador[4];
    int x;
    int y;
    char cor[20];
    char tipo[3];
}PONTO; // pontos que unidos formam as ferrovias

typedef struct{
    int x;
    int y;
}COORDENADAS; // estrutura para satisfazer o número máximo de argumentos por função

typedef struct{
    char identificador[2];
    int raio;
    char cor[20];
    char nome_ferrovia[4];
    char nome_ponto[4];
    int viagens;
}COMBOIO;

typedef struct{
    int  r;
    int  g;
    int  b;
}COR;  // para passar de uma string para valores inteiros

typedef struct{
    int dimx;
    int dimy;
}JANELA; // para obter o tamanho da janela gráfica

typedef struct linha{
    char identificador[4];
    PONTO *dados;
    struct linha *anterior;
    struct linha *proximo;
}LINHA; // lista que servirá para formar linhas ferroviárias

/*
** protótipos de funções
*/
void flushinput(void); // função que dá flush ao stdin
int menu(void); // função que apresenta o menu e retorna a opcao escolhida
int lerconfig(char config[], char **dados, int *num_linhas); // função que processa os dados do ficheiro config
JANELA tamanhoJanela(JANELA _janela, int num_linhas, char **dadosconfig); // retira do config as dimensões da janelace
void processacomboio(COMBOIO **comboios, int *num_comboios, int num_linhas, char **dadosconfig); // recebe o config e forma os comboios
int processaferrovias(LINHA **ferrovias, int *num_ferrovias, int num_linhas, char **dadosconfig); // recebe o config e forma as ferrovias
void processaligar(LINHA **ferrovias, int *num_ferrovias, int num_linhas, char **dadosconfig); // recebe o config e liga as ferrovias
void printlista(LINHA *lista); // imprime uma lista do head até ao tail
int menu1(LINHA **lista, int num_ferrovias); // pede ao utilizador o identificador da ferrovia que deve ser impressa
int menu2(LINHA **lista, int *num_ferrovias); // elimina uma das ferrovias
int menu3(COMBOIO **comboios, int num_comboios); // apresenta o menu da 3ª opção e mostra a informação de um dos comboios
int menu4(COMBOIO **comboios, int *num_comboios); // elimina um dos comboios
int menu5(COMBOIO **comboios, int *num_comboios); // deixa o utilizador criar um comboio
void menu5_id(char *str, int k, int i, COMBOIO **comboios, int *num_comboios); // cria o identificador do comboio da opção 5
void menu5_raio(COMBOIO **comboios, int k, int *num_comboios); // cria o raio do comboio da opção 5
void menu5_cor(COMBOIO **comboios, int k, int *num_comboios, char *str); // cria a cor do comboio da opção 5
void menu5_idfer(COMBOIO **comboios, int k, int *num_comboios, char *str); // cria o identificador da ferrovia do comboio da opção 5
void menu5_idponto(COMBOIO **comboios, int k, int *num_comboios, char *str); // cria o identificador do ponto do comboio da opção 5
void menu5_viagens(COMBOIO **comboios, int k, int *num_comboios); // cria as viagens do comboio da opção 5

/*
** protótipos de funções (parte gráfica)
*/
void renderRailroad (SDL_Renderer * g_pRenderer, LINHA *lista, int num_ferrovias, TTF_Font *_font);
void renderMenu (SDL_Renderer *g_pRenderer, int width, int height, TTF_Font *_font);
COORDENADAS trainCoords (LINHA *lista, COMBOIO *comboios, int num_ferrovias);
void renderTrains (SDL_Renderer * g_pRenderer, COMBOIO *comboios, TTF_Font *_font, COORDENADAS _coords);
void InitEverything(int width, int height, TTF_Font **_font, SDL_Window** _window, SDL_Renderer** g_pRenderer);
SDL_Window* CreateWindow(int width, int height);
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window);
void InitFont();
void filledCircleRGB(SDL_Renderer * g_pRenderer, int _circleX, int _circleY, int _circleR, int _r, int _g, int _b);
COR stringRGB (char corstr[LINEMAX]);
void RenderText(COORDENADAS local, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* g_pRenderer);
COORDENADAS getCoords(LINHA *lista, int num_ferrovias, char _ident[LINEMAX], char _ponta[LINEMAX]);


#endif
