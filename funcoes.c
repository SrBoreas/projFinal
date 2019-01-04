#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h> 
#include <math.h>
#include <SDL.h>
#include "funcoes.h"


/*
** flushinput: Dá flush ao stdin
*/
void flushinput(void) {

    int k;

    while ((k = getchar()) != '\n' && k != EOF) { }

    return;
}

/*
** menu: Apresenta o menu e retorna a opcao escolhida
*/
int menu(void) {

    // declaração de variáveis
    int opcao; // opcao escolhida pelo utilizador
    int k; // verificador do valor de retorno do scanf

    // apresentação da lista de opções
    printf("\nLista de opções:\n");
    printf("0 - Terminar o programa\n");
    printf("1 - Mostrar a informação de uma ferrovia\n");
    printf("2 - Eliminar uma ferrovia\n");
    printf("3 - Mostrar a informação de um comboio\n");
    printf("4 - Eliminar um comboio\n");
    printf("5 - Criar um comboio\n");
    printf("6 - Simulação dos comboios\n");

    // seleção da opção
    printf("Selecione a opção: ");
    k = scanf("%d", &opcao);

    // no caso da opção ser inválida
    while (k != 1 || opcao < 0 || opcao > 6) {
        flushinput();
        printf("\nOpção inválida\n");
        printf("Selecione a opção novamente: ");
        k = scanf("%d", &opcao);
    }

    return opcao; // retorno da opção selecionada
}

/*
** lerconfig: Processa os dados do ficheiro config
*/
int lerconfig(char config[], char **dados, int *num_linhas) {

    // declaração de variáveis
    FILE *fp; // apontador para um ficheiro

    // abertura do ficheiro config
    fp = fopen(config, "r");

    // verificação da abertura com sucesso
    if (fp == NULL) {
        printf("\nNão foi possível abrir o ficheiro config\n");
        return -1;
    }

    // leitura do ficheiro
   do {
        fgets(dados[*num_linhas], LINEMAX, fp);
        if (strncmp(dados[*num_linhas], "%", sizeof(char)) != 0 && strspn(dados[*num_linhas], " \n\r\t") != strlen(dados[*num_linhas])) {
            (*num_linhas)++;
        }
   }while(feof(fp) == 0);

    // fecho do ficheiro
    fclose(fp);

    return 0; // caso de sucesso na abertura do ficheiro
}

/*
** tamanhoJanela : Recebe o config e devolve o tamanho da janela gráfica
*/
void tamanhoJanela(int *_width, int *_height, int num_linhas, char **dadosconfig) {

    // declaração das variáveis
    int i; // contador
    char str[7]; // string extra para o sscanf (7 caracters: "JANELA:")

    for (i = 0; i < num_linhas; i++) {
            if ((strncmp(dadosconfig[i], "JANELA:", 7*sizeof(char))) == 0) {
                sscanf(dadosconfig[i], "%s %d %d ", str, _width, _height);
            }
    }

    return;   
}

/*
** processacomboio: Recebe o config e forma os comboios
*/
void processacomboio(COMBOIO **comboios, int *num_comboios, int num_linhas, char **dadosconfig) {

    // declaração das variáveis
    int i; // contador
    char str[8]; // string extra para o sscanf (8 caracters: "COMBOIO:")


    // processamento dos dados
    for (i = 0; i < num_linhas; i++) {
        if ((strncmp(dadosconfig[i], "COMBOIO:", 8*sizeof(char))) == 0) {
            sscanf(dadosconfig[i], "%s %s %d %s %s %s %d", str, 
                                                           comboios[*num_comboios]->identificador, 
                                                           &(comboios[*num_comboios]->raio), 
                                                           comboios[*num_comboios]->cor, 
                                                           comboios[*num_comboios]->nome_ferrovia, 
                                                           comboios[*num_comboios]->nome_ponto, 
                                                           &(comboios[*num_comboios]->viagens));
            (*num_comboios)++;
        }
    }

    return;
}

/*
** processaferrovias: Recebe o config e forma as ferrovias
*/
int processaferrovias(LINHA **ferrovias, int *num_ferrovias, int num_linhas, char **dadosconfig) {

    // declaração das variáveis
    int i, j = 0; // contador
    int escrita = 0; // indica se está a ser processada uma linha ou não
    PONTO ponto;
    char str[13]; // string auxiliar
    char identificador[4]; // matriz que guarda o identificador

    // processamento dos dados
    for (i = 0; i < num_linhas; i++) {

        // quando deixam de haver pontos para a linha ferroviária em questão
        if (strncmp(dadosconfig[i], "FIM_DE_LINHA:", 13*sizeof(char)) == 0) {
            escrita = 0;
            j = 0;
            (*num_ferrovias)++;
        }

        if (escrita == 1) {

            // da matriz dadosconfig[] são retirados os valores para a variável "ponto"
            sscanf(dadosconfig[i], "%s %d %d %s %s", ponto.identificador, &ponto.x, &ponto.y, ponto.cor, ponto.tipo);

            // primeiro ponto da linha
            if (j == 0) {
                ferrovias[*num_ferrovias]->anterior = NULL;
                ferrovias[*num_ferrovias]->dados = (PONTO *)malloc(sizeof(PONTO));
                if(ferrovias[*num_ferrovias]->dados == NULL) {
                    printf("\nErro na alocação de memória");
                    return -1;
                }
                strcpy(ferrovias[*num_ferrovias]->dados->identificador, ponto.identificador);
                ferrovias[*num_ferrovias]->dados->x = ponto.x;
                ferrovias[*num_ferrovias]->dados->y = ponto.y;
                strcpy(ferrovias[*num_ferrovias]->dados->cor, ponto.cor);
                strcpy(ferrovias[*num_ferrovias]->dados->tipo, ponto.tipo);
                ferrovias[*num_ferrovias]->proximo = NULL;
                strcpy(ferrovias[*num_ferrovias]->identificador, identificador);
            }

            // pontos da linha depois do primeiro
            if (j > 0) {
                // alocação de memória
                ferrovias[*num_ferrovias]->proximo = (LINHA *)malloc(sizeof(LINHA));
                if (ferrovias[*num_ferrovias]->proximo == NULL) {
                    printf("\nErro na alocação de memória");
                    return -1;
                }

                ferrovias[*num_ferrovias]->proximo->dados = (PONTO *)malloc(sizeof(PONTO));
                if (ferrovias[*num_ferrovias]->proximo->dados == NULL) {
                    printf("\nErro na alocação de memória");
                    return -1;
                }

                strcpy(ferrovias[*num_ferrovias]->proximo->dados->identificador, ponto.identificador);
                ferrovias[*num_ferrovias]->proximo->dados->x = ponto.x;
                ferrovias[*num_ferrovias]->proximo->dados->y = ponto.y;
                strcpy(ferrovias[*num_ferrovias]->proximo->dados->cor, ponto.cor);
                strcpy(ferrovias[*num_ferrovias]->proximo->dados->tipo, ponto.tipo);
                ferrovias[*num_ferrovias]->proximo->anterior = ferrovias[*num_ferrovias];
                ferrovias[*num_ferrovias]->proximo->proximo = NULL;
                strcpy(ferrovias[*num_ferrovias]->proximo->identificador, identificador);
                ferrovias[*num_ferrovias] = ferrovias[*num_ferrovias]->proximo;
            }
            j++;
        }   

        // se houverem mais linhas ferroviárias para processar
        if ((strncmp(dadosconfig[i], "LINHA:", 6*sizeof(char))) == 0) { // início dos dados
            sscanf(dadosconfig[i], "%s %s", str, identificador);
            escrita = 1;
        }
    }

    return 0;
}

/*
** rewindlista: Rebobina a lista até ao head
*/
void rewindlista(LINHA *lista) {

    while (lista->anterior != NULL) {
        lista = lista->anterior;
    }

    return;
}

/*
** printlista: Imprime uma lista
*/
void printlista(LINHA *lista) {

    while(lista->proximo != NULL) {
        printf("\n%s %d %d %s %s", lista->dados->identificador, 
                                   lista->dados->x, 
                                   lista->dados->y, 
                                   lista->dados->cor, 
                                   lista->dados->tipo);
    }

    return;
}



/* -------------------------------------------- Parte Gráfica -------------------------------------------- */


/*
** InitEverything: Inicializa a biblioteca SDL2 e todos os componentes gráficos usados: fonte, janela, renderer
** \param width largura em px da janela
** \param height altura em px da janela
** \param _font fonte que será usada para texto
** \param _window representa a janela gráfica
** \param _renderer renderer responsável por apresntar a janela
*/
void InitEverything(int width, int height, TTF_Font **_font, SDL_Window** _window, SDL_Renderer** g_pRenderer)
{
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    InitFont();
    *_window = CreateWindow(width, height);
    *g_pRenderer = CreateRenderer(width, height, *_window);

    // dá load à fonte e define o tamanho de letra
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Cria uma janela gráfica
 * \param width largura em px da janela
 * \param height altura em px da janela
 * \return pointer para a janela gráfica criada
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // inicializa a janela gráfica
    window = SDL_CreateWindow( "Simulação Ferroviária", WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // vê se houveram erros
    if ( window == NULL )
    {
        printf("Erro na criação da janela : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

/*
** CreateRenderer: Cria um renderer
** \param width largura em px da janela
** \param height altura em px da janela
** \param _window representa a janela à qual o renderer está associado
** \return pointer para o renderer criado
*/
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *g_pRenderer;
    // inicializa renderer
    g_pRenderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( g_pRenderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // põe o tamanho do renderer igual ao da janela
    SDL_RenderSetLogicalSize( g_pRenderer, width, height );

    return g_pRenderer;
}

/*
** InitFont: Inicializa o SDL2_ttf (biblioteca de fontes)
*/
void InitFont()
{
    // Inicializa a biblioteca
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/*
** renderRailroad: Apresenta na janela gráfica a ferrovia (pontos e troços)
*/
void renderRailroad (SDL_Renderer * g_pRenderer, LINHA *lista, int num_ferrovias){

     while(lista->proximo != NULL) {
        // Cor usada será branca (troços ferroviários serão desta cor)
        SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 255, 255); 

        SDL_RenderDrawLine(g_pRenderer, 
                           lista->dados->x, 
                           lista->dados->y,
                           lista->proximo->dados->x, 
                           lista->proximo->dados->y);
        lista = lista->proximo;        
    }

    return;
}

/*
** filledCircleRGB: Desenha um círculo preenchido na janela gráfica
** \param _cicleX posição x em px do círculo
** \param _cicleY posição y em px do círculo
** \param _cicleR raio em px do círculo
** \param _r, _g, _b valores usados para a cor do círculo
*/
void filledCircleRGB(SDL_Renderer * g_pRenderer, int _circleX, int _circleY, int _circleR, int _r, int _g, int _b)
{
    int off_x = 0;
    int off_y = 0;
    float degree = 0.0;
    float step = M_PI / (_circleR*8);
    
    SDL_SetRenderDrawColor(g_pRenderer, _r, _g, _b, 255);
    
    while (_circleR > 0)
    {
        for (degree = 0.0; degree < M_PI/2; degree+=step)
        {
            off_x = (int)(_circleR * cos(degree));
            off_y = (int)(_circleR * sin(degree));
            SDL_RenderDrawPoint(g_pRenderer, _circleX+off_x, _circleY+off_y);
            SDL_RenderDrawPoint(g_pRenderer, _circleX-off_y, _circleY+off_x);
            SDL_RenderDrawPoint(g_pRenderer, _circleX-off_x, _circleY-off_y);
            SDL_RenderDrawPoint(g_pRenderer, _circleX+off_y, _circleY-off_x);
        }
        _circleR--;
    }
    return;
}

/*
** stringRGB: Dado um dado do tipo PONTO retorna um valor RGB 
*/
COR stringRGB (PONTO temporario){
    char valor_alterado[LINEMAX];
    COR rgb_value;
    strcpy(valor_alterado, temporario.cor);
    if(strcmp (valor_alterado, "VERMELHO") == 0){
        rgb_value.r = 255;
        rgb_value.g = 0;
        rgb_value.b = 0;
    }
    if(strcmp (valor_alterado, "ROXO") == 0){
        rgb_value.r = 102;
        rgb_value.g = 0;
        rgb_value.b = 102;
    }
    if(strcmp (valor_alterado, "AZUL") == 0){
        rgb_value.r = 0;
        rgb_value.g = 0;
        rgb_value.b = 255;
    }
    if(strcmp (valor_alterado, "CYAN") == 0){
        rgb_value.r = 0;
        rgb_value.g = 255;
        rgb_value.b = 255;
    }
    if(strcmp (valor_alterado, "VERDE") == 0){
        rgb_value.r = 0;
        rgb_value.g = 255;
        rgb_value.b = 0;
    }
    if(strcmp (valor_alterado, "AMARELO") == 0){
        rgb_value.r = 255;
        rgb_value.g = 255;
        rgb_value.b = 0;
    }
    if(strcmp (valor_alterado, "CASTANHO") == 0){
        rgb_value.r = 102;
        rgb_value.g = 51;
        rgb_value.b = 0;
    }
    if(strcmp (valor_alterado, "PRETO") == 0){
        rgb_value.r = 0;
        rgb_value.g = 0;
        rgb_value.b = 0;
    }
    if(strcmp (valor_alterado, "BRANCO") == 0){
        rgb_value.r = 255;
        rgb_value.g = 255;
        rgb_value.b = 255;
    }
    if(strcmp (valor_alterado, "CINZENTO") == 0){
        rgb_value.r = 192;
        rgb_value.g = 192;
        rgb_value.b = 192;
    }
    else{
        rgb_value.r=rgb_value.g=rgb_value.b=256;
    }
    return rgb_value;
}
