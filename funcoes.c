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
    flushinput();

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
        if (strncmp(dados[*num_linhas], "%", sizeof(char)) != 0 && 
            strspn(dados[*num_linhas], " \n\r\t") != strlen(dados[*num_linhas])) 
        {        
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
JANELA tamanhoJanela(JANELA _janela, int num_linhas, char **dadosconfig) {

    // declaração das variáveis
    int i; // contador
    char str[8]; // string extra para o sscanf (7 caracters: "JANELA:")

    for (i = 0; i < num_linhas; i++) {
            if ((strncmp(dadosconfig[i], "JANELA:", 7*sizeof(char))) == 0) {
                sscanf(dadosconfig[i], "%s %d %d", str, &_janela.dimx, &_janela.dimy);
            }
    }
    return _janela;   
}

/*
** processacomboio: Recebe o config e forma os comboios
*/
void processacomboio(COMBOIO **comboios, int *num_comboios, int num_linhas, char **dadosconfig) {

    // declaração das variáveis
    int i; // contador
    char str[8]; // string extra para o sscanf (8 caracters: "COMBOIO:")+

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
    PONTO ponto; // ponto auxiliar
    char str[13]; // string auxiliar
    char identificador[4]; // matriz que guarda o identificador

    // processamento dos dados
    for (i = 0; i < num_linhas; i++) {
        if (strncmp(dadosconfig[i], "FIM_DE_LINHA:", 13*sizeof(char)) == 0) { // se chegar ao fim da linha não há escrita no arrat
            escrita = 0;
            j = 0;
            (*num_ferrovias)++;
        }
        if (escrita == 1) { // caso a variável escrita tenha valor igual a 1, os dados são inseridos nas listas
            sscanf(dadosconfig[i], "%s %d %d %s %s", ponto.identificador, &ponto.x, &ponto.y, ponto.cor, ponto.tipo);
            if (j == 0) { // caso seja o primeiro node a ser inserido
                ferrovias[*num_ferrovias]->anterior = NULL;
                ferrovias[*num_ferrovias]->dados = (PONTO *)malloc(sizeof(PONTO));
                if(ferrovias[*num_ferrovias]->dados == NULL) {
                    printf("\nErro na alocação de memória");
                    return -1;
                }
                // inserção dos dados
                strcpy(ferrovias[*num_ferrovias]->dados->identificador, ponto.identificador);
                ferrovias[*num_ferrovias]->dados->x = ponto.x;
                ferrovias[*num_ferrovias]->dados->y = ponto.y;
                strcpy(ferrovias[*num_ferrovias]->dados->cor, ponto.cor);
                strcpy(ferrovias[*num_ferrovias]->dados->tipo, ponto.tipo);
                ferrovias[*num_ferrovias]->proximo = NULL;
                strcpy(ferrovias[*num_ferrovias]->identificador, identificador);
            }
            if (j > 0) { // caso seja depois do primeiro node a ser inserido
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
                // inserção dos dados
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
        if ((strncmp(dadosconfig[i], "LINHA:", 6*sizeof(char))) == 0) { // caso cheguemos ao princípo de uma linha a variável escrita é igualada e esta inicia
            sscanf(dadosconfig[i], "%s %s", str, identificador);
            escrita = 1;
        }
    }

    return 0;
}

/*
** recebe o config e liga as ferrovias (não está feito)

void processaligar(LINHA **ferrovias, int *num_ferrovias, int num_linhas, char **dadosconfig) {

    // declaração das variáveis
    char linha_saida[4], ponto_saida[4], linha_entrada[4], ponto_entrada[4]; // strings que guardam os locais de saída e entrada
    int i; // contador

    // processamento das linhas
    for (i = 0; i < num_linhas; i++) {

    }

    return;
}

*/

/*
** imprime uma lista do head até ao tail
*/
void printlista(LINHA *lista) {

    while (lista->anterior != NULL) { // "rebobinamento" da lista
        lista = lista->anterior;
    }

    // impressão da lista
    printf("\nIDENTIFICADOR\tX\tY\tCOR\tTIPO\n");

    /*
    ** NOTA: existem dois printfs diferentes puramente por questões estéticas relacionadas
    ** com a forma como \t funciona
    */
    while(lista->proximo != NULL) { // impressão até ao penúltimo node
        if ((strcmp(lista->dados->cor, "VERMELHO")) == 0 || 
            (strcmp(lista->dados->cor, "CASTANHO")) == 0 || 
            (strcmp(lista->dados->cor, "CINZENTO")) == 0) {
            
                printf("\n%s\t\t%d\t%d\t%s%s", lista->dados->identificador, 
                                               lista->dados->x, 
                                               lista->dados->y, 
                                               lista->dados->cor, 
                                               lista->dados->tipo);
                
                lista = lista->proximo; 
        }
        else {
                printf("\n%s\t\t%d\t%d\t%s\t%s",lista->dados->identificador, 
                                                lista->dados->x, 
                                                lista->dados->y, 
                                                lista->dados->cor, 
                                                lista->dados->tipo);

                lista = lista->proximo;
        }
    }

    // impressão do último node
    if ((strcmp(lista->dados->cor, "VERMELHO")) == 0 || 
        (strcmp(lista->dados->cor, "CASTANHO")) == 0 || 
        (strcmp(lista->dados->cor, "CINZENTO")) == 0) {
            
            printf("\n%s\t\t%d\t%d\t%s%s\n", lista->dados->identificador, 
                                             lista->dados->x, 
                                             lista->dados->y, 
                                             lista->dados->cor, 
                                             lista->dados->tipo);
            
            lista = lista->proximo;
    }
    else {
            printf("\n%s\t\t%d\t%d\t%s\t%s\n", lista->dados->identificador, 
                                               lista->dados->x, 
                                               lista->dados->y, 
                                               lista->dados->cor, 
                                               lista->dados->tipo);
            
            lista = lista->proximo;
    }

    return;
}

/*
** menu1: Pede ao utilizador o identificador da ferrovia que deve ser impressa
*/
int menu1(LINHA **lista, int num_ferrovias) {

    // declaração de variáveis
    int k = 0; // verificador do return do scanf
    char *id; // guarda o identificador que o utilizador escreve
    int i; // contador
    int verificador = 0; // diz se existe uma lista com um identificador correspondente

    // alocação de memória
    id = (char *)calloc(LINEMAX, sizeof(char));
    if (id == NULL) {
        printf("\nErro de alocação de memória");
        return -1;
    }

    // menu
    printf("\nInsira o identificador da ferrovia: ");
    k = scanf("%s", id);
    while (k != 1 || (strlen(id)) > 4) {
        flushinput();
        printf("\nIdentificador inválido. Insira um identificador com 4 caractéres no máximo: ");
        k = scanf("%s", id);
    }
    flushinput();

    // procura a ferrovia
    for (i = 0; i < num_ferrovias; i++) {
        if ((strcmp(id, lista[i]->identificador)) == 0) {
            verificador = 1;
            printlista(lista[i]);
        }
    }

    // libertação de memória
    free(id);

    // verifica se a ferrovia foi encontrada
    if (verificador == 0) {
        printf("\nNão foi possível encontrar a ferrovia\n");
    }

    return 0;
}

/*
** menu2: Elimina uma das ferrovias (não funciona, dá segmentation fault)

int menu2(LINHA **lista, int *num_ferrovias) {

    // declaração de variáveis
    int k; // verificador do return do scanf
    int i, j; // contadores
    char *id; // string que guarda o identificador que o utilizador escreve
    LINHA *aux; // lista auxiliar

    // alocação de memória
    id = (char *)calloc(LINEMAX, sizeof(char));
    if (id == NULL) {
        printf("\nErro ao alocar memória");
        return -1;
    }

    // apresentação menu
    printf("\nInsira o identificador da ferrovia que deseja eliminar: ");
    k = scanf("%s", id);
    while(k != 1 || (strlen(id)) > 4) {
        flushinput();
        printf("\nIdentificador inválido. Insira novamente: ");
        k = scanf("%s", id);
    }

   
        //procura o identificador correspondente na lista e tenta mexer todos
        //os elementos à direita do que queremos eliminar uma coluna para a esquerda
        //eliminando depois o último elemento do array de listas
        //e decrementando a variável que indica o número de ferrovias
   
    for (i = 0; i < *num_ferrovias; i++) {
        if (strcmp(lista[i]->identificador, id) == 0) {
            for (j = i; j < *num_ferrovias - 1; j++) {
                lista[j]->dados = lista[j+1]->dados;
                strcpy(lista[j]->identificador, lista[j+1]->identificador);
                lista[j]->proximo = lista[j+1]->proximo;
                lista[j]->anterior = lista[j+1]->anterior;
            }
            while (lista[*num_ferrovias] != NULL) {
                aux = lista[*num_ferrovias];
                lista[*num_ferrovias] = lista[*num_ferrovias]->anterior;
                free(aux->dados);
                free(aux);
            }
            *(num_ferrovias)--;

        }
    }

    // libertação de memória
    free(id);

    return 0;
}

*/

/*
** menu3: Apresenta o menu da 3ª opção e mostra a informação de um dos comboios
*/
int menu3(COMBOIO **comboios, int num_comboios) {

    // declaração das variáveis
    int k; // verificador do return do scanf
    int i; // contador
    char *id; // string que guarda o identificador que o utilizador escreve
    int encontrado = 0; // diz se o identificador corresponde a algum dos existentes

    // alocação de memória
    id = (char *)calloc(LINEMAX, sizeof(char));
    if (id == NULL) {
        printf("\nErro de alocação de memória");
        return -1;
    }

    // apresentação do menu
    printf("\nInsira o identificador do comboio que pretende apresentar: ");
    k =scanf("%s", id);
    if (k != 1 || strlen(id) > 2) {
        flushinput();
        printf("\nInsira um identificador com dois caractéres no máximo: ");
        k = scanf("%s", id);
    }
    flushinput();

    // procura do comboio
    for (i = 0; i < num_comboios; i++) {
        if (strcmp(comboios[i]->identificador, id) == 0) {
            encontrado = 1; // indica que é verdadeira a correspondência

            // impressão dos dados do comboio
            printf("\nIDENTIFICADOR\tRAIO\tCOR\tFERROVIA INÍCIO\tVIAGENS");
            if (strcmp(comboios[i]->cor, "VERMELHO") == 0 || 
                strcmp(comboios[i]->cor, "CASTANHO") == 0 || 
                strcmp(comboios[i]->cor, "CINZENTO") == 0) 
            {
                printf("\n%s\t\t%d\t%s %s\t %s\t%d\n", comboios[i]->identificador, 
                                                       comboios[i]->raio, 
                                                       comboios[i]->cor, 
                                                       comboios[i]->nome_ferrovia, 
                                                       comboios[i]->nome_ponto, 
                                                       comboios[i]->viagens);
            }
            else{
                printf("\n%s\t\t%d\t%s\t%s\t %s\t%d\n", comboios[i]->identificador, 
                                                        comboios[i]->raio, 
                                                        comboios[i]->cor, 
                                                        comboios[i]->nome_ferrovia, 
                                                        comboios[i]->nome_ponto, 
                                                        comboios[i]->viagens);
            }
        }
    }

    // libertação de memória
    free(id);

    // verificação da existência do identificador
    if (encontrado == 0) {
        printf("\nNão foi possível encontrar um comboio com este identificador\n");
    }

    return 0;
}

/*
** menu4: Elimina um dos comboios
*/
int menu4(COMBOIO **comboios, int *num_comboios) {

    // declaração de variáveis
    char *str; // string auxiliar
    int i, j; // contadores
    int k; // verificador do return do scanf
    int encontrado = 0; // diz se o identificador foi encontrado

    // alocação de memória
    str = (char *)calloc(LINEMAX, sizeof(char));
    if (str == NULL) {
        printf("\nErro na alocação de memória");
        return -1;
    }

    // apresentação do menu
    printf("\nInsira o identificador do comboio que deseja eliminar: ");
    k = scanf("%s", str);
    if (k != 1 || strlen(str) > 2) {
        flushinput();
        printf("\nInsira um identificador com dois caractéres no máximo: ");
        k = scanf("%s", str);
    }
    flushinput();

    // procura do comboio a eliminar
    for (i = 0; i < (*num_comboios); i++) {
        if (strcmp(comboios[i]->identificador, str) == 0) {
            encontrado = 1; // indica a correspondência do identificador
            for (j = i; j < (*num_comboios) - 1; j++) {
                /*
                ** os comboios à direita do que se pretende eliminar no array
                ** são movidos uma posição para a esquerda, sendo depois
                ** eliminado o comboio que está na última posição
                ** e sendo decrementada a variável que indica o número
                ** de comboios existentes
                */
                strcpy(comboios[j]->identificador, comboios[j + 1]->identificador);
                comboios[j]->raio = comboios[j + 1]->raio;
                strcpy(comboios[j]->nome_ferrovia, comboios[j + 1]->nome_ferrovia);
                strcpy(comboios[j]->nome_ponto, comboios[j + 1]->nome_ponto);
                comboios[j]->viagens = comboios[j + 1]->viagens;
            }
            free(comboios[(*num_comboios) - 1]);
            (*num_comboios)--;
            break;
        }
    }

    // libertação de memória
    free(str);

    // verificação da coreespondência do identificador
    if (encontrado == 0) {
        printf("\nNão foi possível encontrar um comboio com este identificador\n");
    }

    return 0;
}

/*
** menu5: Deixa o utilizador criar um comboio
*/
int menu5(COMBOIO **comboios, int *num_comboios) {

    // declaração de variáveis
    int i = 0; // contador
    int k = 0; // verificador do return do scanf
    char *str; // string auxiliar

    // alocação de memória
    str = (char *)calloc(LINEMAX, sizeof(char));
    if (str == NULL) {
        printf("\nErro na alocação de memória");
        return -1;
    }

    comboios[*num_comboios] = (COMBOIO *)malloc(sizeof(COMBOIO));
    if (comboios[*num_comboios] == NULL) {
        printf("\nErro na alocação da memória");
        return -1;
    }

    // menu
    menu5_id(str, k, i, comboios, num_comboios);
    menu5_raio(comboios, k, num_comboios);
    menu5_cor(comboios, k, num_comboios, str);
    menu5_idfer(comboios, k, num_comboios, str);
    menu5_idponto(comboios, k, num_comboios, str);
    menu5_viagens(comboios, k, num_comboios);

    // incrementação do número de comboios
    (*num_comboios)++;

    // libertação de memória
    free(str);

    return 0;
}

/*
** menu5_id: Cria o identificador do comboio da opção 5
*/
void menu5_id(char *str, int k, int i, COMBOIO **comboios, int *num_comboios) {

    printf("\nInsira o identificador do novo comboio: ");
    k = scanf("%s", str);
    for (i = 0; i < *num_comboios; i++) {
        while (k != 1 || strlen(str) > 2 || strcmp(str, comboios[i]->identificador) == 0) {
            flushinput();
            printf("\nIdentificador inválido. Insira novamente: ");
            k = scanf("%s", str);
        }
    }
    strcpy(comboios[*num_comboios]->identificador, str);
    flushinput();

    return;
}

/*
** menu5_raio: Cria o raio do comboio da opção 5
*/
void menu5_raio(COMBOIO **comboios, int k, int *num_comboios) {

    printf("\nInsira o raio de cada bola: ");
    k = scanf("%d", &comboios[*num_comboios]->raio);
    while (k != 1 || comboios[*num_comboios]->raio <= 0) {
        flushinput();
        printf("\nInsira um inteiro: ");
        k = scanf("%d", &comboios[*num_comboios]->raio);
    }
    flushinput();

    return;
}

/*
** menu5_cor: Cria a cor do comboio da opção 5
*/
void menu5_cor(COMBOIO **comboios, int k, int *num_comboios, char *str) {

    printf("\nInsira a cor do novo comboio: ");
    k = scanf("%s", str);
    while (k != 1 || strlen(str) > 20 || (strcmp(str, "VERMELHO") != 0 && 
                                          strcmp(str, "AZUL") != 0     && 
                                          strcmp(str, "AMARELO") != 0  && 
                                          strcmp(str, "VERDE") != 0    && 
                                          strcmp(str, "CINZENTO") != 0 && 
                                          strcmp(str, "CASTANHO") != 0 && 
                                          strcmp(str, "PRETO") != 0    && 
                                          strcmp(str, "CYAN") != 0     && 
                                          strcmp(str, "ROXO") != 0     && 
                                          strcmp(str, "BRANCO") != 0)) 
    {
        flushinput();
        printf("\nCor inválida. Insira novamente: ");
        k = scanf("%s", str);
    }
    strcpy(comboios[*num_comboios]->cor, str);
    flushinput();

    return;
}

/*
** menu5_idfer: Cria o identificador da ferrovia do comboio da opção 5
*/
void menu5_idfer(COMBOIO **comboios, int k, int *num_comboios, char *str) {

    printf("\nInsira o identificador da ferrovia onde circula o novo comboio: ");
    k = scanf("%s", str);
    while (k != 1 || strlen(str) > 4) {
        flushinput();
        printf("\nIdentificador inválido. Insira um identificador de ferrovia com 4 caractéres no máximo: ");
        k = scanf("%s", str);
    }
    strcpy(comboios[*num_comboios]->nome_ferrovia, str);
    flushinput();

    return;
}

/*
** menu5_idponto: Cria o identificador do ponto do comboio da opção 5
*/
void menu5_idponto(COMBOIO **comboios, int k, int *num_comboios, char *str) {

    printf("\nInsira o identificador do ponto onde inicia a circulação do novo comboio: ");
    k = scanf("%s", str);
    while (k != 1 || strlen(str) > 4) {
        flushinput();
        printf("\nIdentificador inválido. Insira um identificador de ponto com 4 caractéres no máximo: ");
        k = scanf("%s", str);
    }
    strcpy(comboios[*num_comboios]->nome_ponto, str);
    flushinput();

    return;
}

/*
** menu5_viagens: Cria as viagens do comboio da opção 5
*/
void menu5_viagens(COMBOIO **comboios, int k, int *num_comboios) {

    printf("\nInsira o número de viagens: ");
    k = scanf("%d", &comboios[*num_comboios]->viagens);
    while (k != 1 || comboios[*num_comboios]->viagens <= 0) {
        flushinput();
        printf("\nInsira um inteiro positivo: ");
        k = scanf("%d", &comboios[*num_comboios]->viagens);
    }
    flushinput();

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
    *_font = TTF_OpenFont("FreeSerif.ttf", 40);
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
    
    LINHA *ultimo = NULL; // para no fim fechar a ferrovia formando um polígono
    COR cor;

    // o renderer fica da cor branca
    SDL_SetRenderDrawColor( g_pRenderer, 255, 255, 255, 255 );

    // limpa a janela
    SDL_RenderClear( g_pRenderer );

    while (lista->anterior != NULL) { // "rebobinamento" da lista para termos o 1º valor
        lista = lista->anterior;
    }

    while(lista->proximo != NULL) {
        // Cor usada será branca (troços ferroviários serão desta cor)
        cor = stringRGB(*lista->dados);

        SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);
        // desenha os troços da ferrovia
        SDL_RenderDrawLine(g_pRenderer, 
                           lista->dados->x, 
                           lista->dados->y,
                           lista->proximo->dados->x, 
                           lista->proximo->dados->y);

        if(strcmp(lista->dados->tipo, "VIA") == 0){
           filledCircleRGB(g_pRenderer, lista->dados->x, lista->dados->y, 6, 190, 190, 190); 
        }  
        else if(strcmp(lista->dados->tipo, "EST") == 0){
           filledCircleRGB(g_pRenderer, lista->dados->x, lista->dados->y, 20, cor.r, cor.g, cor.b); 
        }
        else printf("Ponto do tipo errado.\n");

        lista = lista->proximo;
        ultimo = lista;  // guarda o último valor da lista    
    }

    while (lista->anterior != NULL) { // "rebobinamento" da lista para ligar o 1º ponto ao último
        lista = lista->anterior;
    }

    // fecha a ferrovia
    SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(g_pRenderer, lista->dados->x, lista->dados->y,ultimo->dados->x, ultimo->dados->y);

    // desenha o primeiro ponto de novo (para ficar por cima do troço)
    cor = stringRGB(*lista->dados);
    if(strcmp(lista->dados->tipo, "VIA") == 0){
       filledCircleRGB(g_pRenderer, lista->dados->x, lista->dados->y, 6, 190, 190, 190); 
    }  
    else if(strcmp(lista->dados->tipo, "EST") == 0){
       filledCircleRGB(g_pRenderer, lista->dados->x, lista->dados->y, 20, cor.r, cor.g, cor.b); 
    }
    
    // desenha o último ponto
    cor = stringRGB(*ultimo->dados);
    if(strcmp(ultimo->dados->tipo, "VIA") == 0){
           filledCircleRGB(g_pRenderer, ultimo->dados->x, ultimo->dados->y, 6, 190, 190, 190); 
    }        
    else if(strcmp(ultimo->dados->tipo, "EST") == 0){
       filledCircleRGB(g_pRenderer, ultimo->dados->x, ultimo->dados->y, 20, cor.r, cor.g, cor.b); 
    }
    else printf("Ponto do tipo errado.\n");

    return;
}

void renderMenu (SDL_Renderer *g_pRenderer, int width, int height, TTF_Font *_font){

    
    SDL_Rect R1, R2, R3;
    SDL_Color black = { 0, 0, 0 };
    R1.x = R2.x = R3.x = width - 220;
    R1.y = 20;
    R2.y = R1.y + 70;
    R3.y = R2.y + 70;
    R1.w = R2.w = R3.w = 200;
    R1.h = R2.h = R3.h = 60;

    SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 153, 255);
    SDL_RenderFillRect(g_pRenderer, &R1);
    SDL_RenderFillRect(g_pRenderer, &R2);
    SDL_RenderFillRect(g_pRenderer, &R3);

    SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(g_pRenderer, &R1);
    SDL_RenderDrawRect(g_pRenderer, &R2);
    SDL_RenderDrawRect(g_pRenderer, &R3);

    RenderText(R1.x + 25, R1.y, "Continue", _font, &black, g_pRenderer);
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
    else if(strcmp (valor_alterado, "ROXO") == 0){
        rgb_value.r = 102;
        rgb_value.g = 0;
        rgb_value.b = 102;
    }
    else if(strcmp (valor_alterado, "AZUL") == 0){
        rgb_value.r = 0;
        rgb_value.g = 0;
        rgb_value.b = 255;
    }
    else if(strcmp (valor_alterado, "CYAN") == 0){
        rgb_value.r = 0;
        rgb_value.g = 255;
        rgb_value.b = 255;
    }
    else if(strcmp (valor_alterado, "VERDE") == 0){
        rgb_value.r = 0;
        rgb_value.g = 255;
        rgb_value.b = 0;
    }
    else if(strcmp (valor_alterado, "AMARELO") == 0){
        rgb_value.r = 255;
        rgb_value.g = 255;
        rgb_value.b = 0;
    }
    else if(strcmp (valor_alterado, "CASTANHO") == 0){
        rgb_value.r = 102;
        rgb_value.g = 51;
        rgb_value.b = 0;
    }
    else if(strcmp (valor_alterado, "PRETO") == 0){
        rgb_value.r = 0;
        rgb_value.g = 0;
        rgb_value.b = 0;
    }
    else if(strcmp (valor_alterado, "BRANCO") == 0){
        rgb_value.r = 255;
        rgb_value.g = 255;
        rgb_value.b = 255;
    }
    else if(strcmp (valor_alterado, "CINZENTO") == 0){
        rgb_value.r = 192;
        rgb_value.g = 192;
        rgb_value.b = 192;
    }
    else{
        rgb_value.r=rgb_value.g=rgb_value.b=256;
    }
    return rgb_value;
}

/**
 * RenderText function: Renders some text on a position inside the app window
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string with the text to be written
 * \param _font TTF font used to render the text
 * \param _color color of the text
 * \param g_pRenderer renderer to handle all rendering in a window
 */
void RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* g_pRenderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(g_pRenderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(g_pRenderer, text_texture, NULL, &solidRect);
    // clear memory
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return;
}