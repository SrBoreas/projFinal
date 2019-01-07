#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL_ttf.h> 
#include <math.h>
#include <SDL.h>
#include "funcoes1.h"


/*
** função que dá flush ao stdin
*/
void flushinput(void) {

    int k;

    while ((k = getchar()) != '\n' && k != EOF) { }

    return;
}

/*
** função que apresenta o menu e retorna a opcao escolhida
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
** função que processa os dados do ficheiro config
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
** recebe o config e forma os comboios
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
                                                           &comboios[*num_comboios]->raio, 
                                                           comboios[*num_comboios]->cor, 
                                                           comboios[*num_comboios]->nome_ferrovia, 
                                                           comboios[*num_comboios]->nome_ponto, 
                                                           &comboios[*num_comboios]->viagens);
            (*num_comboios)++;
        }
    }

    return;
}

/*
** recebe o config e forma as ferrovias
*/
int processaferrovias(LINHA **ferrovias, int *num_ferrovias, int num_linhas, char **dadosconfig) {

    // declaração das variáveis
    int i, j = 0; // contadores
    int escrita = 0; // indica se está a ser processada uma linha ou não
    PONTO ponto; // ponto auxiliar
    char str[13]; // string auxiliar
    char identificador[4]; // matriz que guarda o identificador
    int verificador = 0; // verifica que a memória foi alocada e que o sscanf leu os argumentos todos

    // processamento dos dados
    for (i = 0; i < num_linhas; i++) {
        if (strncmp(dadosconfig[i], "FIM_DE_LINHA:", 13*sizeof(char)) == 0) { // se chegar ao fim da linha não há escrita no arrat
            escrita = 0;
            j = 0;
            (*num_ferrovias)++;
        }
        if (escrita == 1) { // caso a variável escrita tenha valor igual a 1, os dados são inseridos nas listas
            verificador = sscanf(dadosconfig[i], "%s %d %d %s %s", ponto.identificador, &ponto.x, &ponto.y, ponto.cor, ponto.tipo);
            if ((j == 0) && (verificador == 5)) {
                // inserção dos dados
                ferrovias[*num_ferrovias]->proximo1 = NULL;
                ferrovias[*num_ferrovias]->proximo2 = NULL;
                strcpy(ferrovias[*num_ferrovias]->identificador, identificador);
                strcpy(ferrovias[*num_ferrovias]->dados.identificador, ponto.identificador);
                ferrovias[*num_ferrovias]->dados.x = ponto.x;
                ferrovias[*num_ferrovias]->dados.y = ponto.y;
                strcpy(ferrovias[*num_ferrovias]->dados.cor, ponto.cor);
                strcpy(ferrovias[*num_ferrovias]->dados.tipo, ponto.tipo);
                ferrovias[*num_ferrovias]->dados.entrada = 1;
                ferrovias[*num_ferrovias]->dados.saida = 1;
            }
            if ((j > 0) && (verificador == 5)) { // caso seja depois do primeiro node a ser inserido
                // inserção dos dados
                verificador = inserirponto(ferrovias[*num_ferrovias], ponto, identificador);
                if (verificador == -1)
                    return -1;
            }
            j++;
        }
        if ((strncmp(dadosconfig[i], "LINHA:", 6*sizeof(char))) == 0) { // caso cheguemos ao princípo de uma linha 
                                                                        // a variável escrita é igualada e esta inicia
            
            verificador = sscanf(dadosconfig[i], "%s %s", str, identificador);
            if (verificador == 2)
                escrita = 1;
        }
    }

    return 0;
}

/*
** recebe o config e liga as ferrovias (não está feito)
*/
int processaligar(LINHA **ferrovias, int num_ferrovias, int num_linhas, char **dadosconfig) {

    // declaração das variáveis
    char linha_saida[4], ponto_saida[4], linha_entrada[4], ponto_entrada[4]; // strings que guardam os locais de saída e entrada
    int i, j; // contadores
    int pos_saida = -1, pos_entrada = -1; // guardam as posições no array da ferrovia de saída e da de entrada
    char aux[6]; // string auxiliar
    int verificador; // verifica a alocação de memória em ligalista

    // procura das ligações no config
    for (i = 0; i < num_linhas; i++) {
        if (strncmp(dadosconfig[i], "LIGAR:", sizeof(char)*6) == 0) {
            sscanf(dadosconfig[i], "%s %s %s %s %s", aux, linha_saida, ponto_saida, linha_entrada, ponto_entrada);
            // procura as ferrovias com identificadores correspondentes
            for (j = 0; j < num_ferrovias; j++) {
                if (strcmp(linha_saida, ferrovias[j]->identificador) == 0) {
                    pos_saida = j;
                }
                if (strcmp(linha_entrada, ferrovias[j]->identificador) == 0) {
                    pos_entrada = j;
                }
            }

            if (pos_entrada != -1 && pos_saida != -1 && pos_entrada != pos_saida) {
                verificador = ligalista(ferrovias[pos_entrada], ferrovias[pos_saida], ponto_entrada, ponto_saida);
            }

            if (verificador == -1) {
                return -1;
            }
        }
    }

    return 0;
}


/*
** imprime uma lista do head até ao tail
*/
void printlista(LINHA *lista) {

    // declaração de variáveis
    LINHA *atual = lista;

    // impressão da lista
    printf("\nIDENTIFICADOR\tX\tY\tCOR\tTIPO\tNºENTR.\tNºSA\t\n");

    /*
    ** NOTA: existem dois printfs diferentes puramente por questões estéticas relacionadas
    ** com a forma como \t funciona
    */
    while(atual->proximo1 != NULL) { // impressão até ao penúltimo node
        if ((strcmp(atual->dados.cor, "VERMELHO")) == 0 || (strcmp(atual->dados.cor, "CASTANHO")) == 0 || 
                                                           (strcmp(atual->dados.cor, "CINZENTO")) == 0) 
        {
            printf("\n%s\t\t%d\t%d\t%s%s\t%d\t%d", atual->dados.identificador, 
                                                   atual->dados.x, 
                                                   atual->dados.y, 
                                                   atual->dados.cor, 
                                                   atual->dados.tipo, 
                                                   atual->dados.entrada, 
                                                   atual->dados.saida);
            atual = atual->proximo1;
        }
        else 
        {
            printf("\n%s\t\t%d\t%d\t%s\t%s\t%d\t%d", atual->dados.identificador, 
                                                     atual->dados.x, 
                                                     atual->dados.y, 
                                                     atual->dados.cor, 
                                                     atual->dados.tipo, 
                                                     atual->dados.entrada, 
                                                     atual->dados.saida);
            atual = atual->proximo1;
        }
    }

    // impressão do último node
    if ((strcmp(atual->dados.cor, "VERMELHO")) == 0 || (strcmp(atual->dados.cor, "CASTANHO")) == 0 || 
                                                       (strcmp(atual->dados.cor, "CINZENTO")) == 0) 
    {
        printf("\n%s\t\t%d\t%d\t%s%s\t%d\t%d\n", atual->dados.identificador, 
                                                 atual->dados.x, 
                                                 atual->dados.y, 
                                                 atual->dados.cor, 
                                                 atual->dados.tipo, 
                                                 atual->dados.entrada, 
                                                 atual->dados.saida);
        atual = atual->proximo1;
    }
    else 
    {
        printf("\n%s\t\t%d\t%d\t%s\t%s\t%d\t%d\n", atual->dados.identificador, 
                                                   atual->dados.x, 
                                                   atual->dados.y, 
                                                   atual->dados.cor, 
                                                   atual->dados.tipo, 
                                                   atual->dados.entrada, 
                                                   atual->dados.saida);
        atual = atual->proximo1;
    }

    return;
}

/*
** pede ao utilizador o identificador da ferrovia que deve ser impressa
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
** apresenta o menu da 3ª opção e mostra a informação de um dos comboios
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
            if (strcmp(comboios[i]->cor, "VERMELHO") == 0 || strcmp(comboios[i]->cor, "CASTANHO") == 0 || 
                                                             strcmp(comboios[i]->cor, "CINZENTO") == 0) 
            {
                printf("\n%s\t\t%d\t%s %s\t %s\t%d\n", comboios[i]->identificador, 
                                                       comboios[i]->raio, 
                                                       comboios[i]->cor, 
                                                       comboios[i]->nome_ferrovia, 
                                                       comboios[i]->nome_ponto, 
                                                       comboios[i]->viagens);
            }
            else
            {
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
** elimina um dos comboios
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
** deixa o utilizador criar um comboio
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
** cria o identificador do comboio da opção 5
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
** cria o raio do comboio da opção 5
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
** cria a cor do comboio da opção 5
*/
void menu5_cor(COMBOIO **comboios, int k, int *num_comboios, char *str) {

    printf("\nInsira a cor do novo comboio: ");
    k = scanf("%s", str);
    while (k != 1 || strlen(str) > 20 || (strcmp(str, "VERMELHO") != 0 && 
                                          strcmp(str, "AZUL") != 0 && 
                                          strcmp(str, "AMARELO") != 0 && 
                                          strcmp(str, "VERDE") != 0 && 
                                          strcmp(str, "CINZENTO") != 0 && 
                                          strcmp(str, "CASTANHO") != 0 && 
                                          strcmp(str, "PRETO") != 0 && 
                                          strcmp(str, "CYAN") != 0 && 
                                          strcmp(str, "ROXO") != 0 && 
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
** cria o identificador da ferrovia do comboio da opção 5
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
** cria o identificador do ponto do comboio da opção 5
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
** cria as viagens do comboio da opção 5
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

/*
**  insere um ponto no fim da lista
*/
int inserirponto(LINHA *ferrovia, PONTO ponto, char id[]) {

    // declaração de variáveis
    LINHA *atual = ferrovia;

    // ir até ao fim da lista
    while (atual->proximo1 != NULL) {
        atual = atual->proximo1;
    }

    atual->proximo1 = (LINHA *)malloc(sizeof(LINHA));
    if (atual->proximo1 == NULL) {
        printf("\nErro na alocação de memória\n");
        return -1;
    }

    strcpy(atual->proximo1->identificador, id);
    strcpy(atual->proximo1->dados.identificador, ponto.identificador);
    atual->proximo1->dados.x = ponto.x;
    atual->proximo1->dados.y = ponto.y;
    strcpy(atual->proximo1->dados.cor, ponto.cor);
    strcpy(atual->proximo1->dados.tipo, ponto.tipo);
    atual->proximo1->dados.entrada = 1;
    atual->proximo1->dados.saida = 1;
    atual->proximo1->proximo1 = NULL;
    atual->proximo1->proximo2 = NULL;

    return 0;
}

/*
** executa o comando LIGAR do config
*/
int ligalista(LINHA *ferrovia1, LINHA *ferrovia2, char ponto_entrada[], char ponto_saida[]) {

    // declaração de variáveis
    LINHA *atual1 = ferrovia1;
    LINHA *atual2 = ferrovia2;
    int verifica_entrada = 0; // diz se o ponto de entrada existe na ferrovia
    int verifica_saida = 0; // diz se o ponto de saida existe na ferrovia
    //int verifica_memoria = 0; // verifica a alocação de memória

    // verifica a existência dos pontos nas ferrovias
    if (strcmp(atual1->dados.identificador, ponto_entrada) == 0) {
        verifica_entrada = 1;
    }
    while (strcmp(atual1->dados.identificador, ponto_entrada) != 0) {
        atual1 = atual1->proximo1;
        if (strcmp(atual1->dados.identificador, ponto_entrada) == 0 && atual2->dados.entrada == 1) {
            verifica_entrada = 1;
        }
    }

    if (strcmp(atual2->dados.identificador, ponto_saida) == 0) {
        verifica_saida = 1;
    }
    while (strcmp(atual2->dados.identificador, ponto_saida) != 0) {
        atual2 = atual2->proximo1;
        if (strcmp(atual2->dados.identificador, ponto_saida) == 0 && atual2->dados.saida == 1) {
            verifica_saida = 1;
        }
    }

    // liga as duas ferrovias caso os pontos existam
    if (verifica_entrada == 1 && verifica_saida == 1) {
        atual1->dados.entrada = 2;
        atual2->dados.saida = 2;
        atual2->proximo2 = (LINHA *)malloc(sizeof(LINHA));
        if (atual2->proximo2 == NULL) {
            printf("\nErro de alocaçã de memória\n");
            return -1;
        }
        atual2->proximo2 = atual1;
    }


    return 0;
}

/*
** apaga uma lista node a node
*/
void apagalista(LINHA *ferrovia) {

    LINHA *atual = ferrovia;
    LINHA *prox;

    while (atual != NULL) {
        prox = atual->proximo1;
        free(atual);
        atual = prox;
    }
    ferrovia = NULL;

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
    *_font = TTF_OpenFont("FreeSerif.ttf", 22);
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
void renderRailroad (SDL_Renderer * g_pRenderer, LINHA *lista, int num_ferrovias, TTF_Font *_font)
{
    // declaração de variáveis
    LINHA *atual = lista;
    LINHA *ultimo = NULL; // para no fim fechar a ferrovia formando um polígono
    SDL_Color black = { 0, 0, 0 };
    COR cor;
    COORDENADAS local;

    // o renderer fica da cor branca
    SDL_SetRenderDrawColor( g_pRenderer, 255, 255, 255, 255 );

    // limpa a janela
    SDL_RenderClear( g_pRenderer );

    while(atual->proximo1 != NULL) {
        // Cor usada será branca (troços ferroviários serão desta cor)
        SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);

        cor = stringRGB(atual->dados.cor);

        // desenha os troços da ferrovia
        SDL_RenderDrawLine(g_pRenderer, 
                           atual->dados.x, 
                           atual->dados.y,
                           atual->proximo1->dados.x, 
                           atual->proximo1->dados.y);

        if(strcmp(atual->dados.tipo, "VIA") == 0){
           filledCircleRGB(g_pRenderer, atual->dados.x, atual->dados.y, 6, 190, 190, 190); 
        }  
        else if(strcmp(atual->dados.tipo, "EST") == 0){
           filledCircleRGB(g_pRenderer, atual->dados.x, atual->dados.y, 20, cor.r, cor.g, cor.b);
           local.x = atual->dados.x-16;
           local.y = atual->dados.y-18; 
           RenderText(local, atual->dados.identificador, _font, &black, g_pRenderer); 
        }
        else printf("Ponto do tipo errado.\n");

        atual = atual->proximo1;
        ultimo = atual;  // guarda o último valor da lista    
    }

    // fecha a ferrovia
    SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(g_pRenderer, lista->dados.x, lista->dados.y,ultimo->dados.x, ultimo->dados.y);

    // desenha o primeiro ponto de novo (para ficar por cima do troço)
    cor = stringRGB(lista->dados.cor);
    if(strcmp(lista->dados.tipo, "VIA") == 0){
        filledCircleRGB(g_pRenderer, lista->dados.x, lista->dados.y, 6, 190, 190, 190); 
    }  
    else if(strcmp(atual->dados.tipo, "EST") == 0){
        filledCircleRGB(g_pRenderer, lista->dados.x, lista->dados.y, 20, cor.r, cor.g, cor.b);
        local.x = lista->dados.x-16;
        local.y = lista->dados.y-18;
        RenderText(local, lista->dados.identificador, _font, &black, g_pRenderer);  
    }
    
    // desenha o último ponto
    cor = stringRGB(ultimo->dados.cor);
    if(strcmp(ultimo->dados.tipo, "VIA") == 0){
        filledCircleRGB(g_pRenderer, ultimo->dados.x, ultimo->dados.y, 6, 190, 190, 190); 
    }        
    else if(strcmp(ultimo->dados.tipo, "EST") == 0){
        filledCircleRGB(g_pRenderer, ultimo->dados.x, ultimo->dados.y, 20, cor.r, cor.g, cor.b);
        local.x = ultimo->dados.x-16;
        local.y = ultimo->dados.y-18;
        RenderText(local, ultimo->dados.identificador, _font, &black, g_pRenderer);  
    }
    else printf("Ponto do tipo errado.\n");

    return;
}

/*
** renderMenu: Apresenta na janela gráfica um menu com opções para a simulação
*/
void renderMenu (SDL_Renderer *g_pRenderer, int width, int height, TTF_Font *_font)
{
    //declaração das variáveis
    SDL_Rect R1, R2, R3;
    SDL_Color black = { 0, 0, 0 };
    COORDENADAS local;

    R1.x = R2.x = R3.x = width - 220;
    R1.y = 20;
    R2.y = R1.y + 50;
    R3.y = R2.y + 50;
    R1.w = R2.w = R3.w = 200;
    R1.h = R2.h = R3.h = 40;

    SDL_SetRenderDrawColor(g_pRenderer, 255, 255, 153, 255);
    SDL_RenderFillRect(g_pRenderer, &R1);
    SDL_RenderFillRect(g_pRenderer, &R2);
    SDL_RenderFillRect(g_pRenderer, &R3);

    SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(g_pRenderer, &R1);
    SDL_RenderDrawRect(g_pRenderer, &R2);
    SDL_RenderDrawRect(g_pRenderer, &R3);

    local.x = R1.x + 60;
    local.y = R1.y;
    RenderText(local, "Continue", _font, &black, g_pRenderer);
    local.x = R1.x + 65;
    local.y = R2.y;
    RenderText(local, "Suspend", _font, &black, g_pRenderer);
    local.x = R1.x + 80;
    local.y = R3.y;
    RenderText(local, "Stop", _font, &black, g_pRenderer);
}

/*
** renderTrains: apresenta na janela gráfica os comboios a andar
*/
void renderTrains (SDL_Renderer * g_pRenderer, COMBOIO comboios, TTF_Font *_font, COORDENADAS _coords)
{
    // declaração de variáveis
    COR cor;
    SDL_Color black = { 0, 0, 0 };
    COORDENADAS local;

    local.x = _coords.x - comboios.raio;
    local.y = _coords.y - (comboios.raio + 10);

    cor = stringRGB(comboios.cor);
    filledCircleRGB(g_pRenderer, _coords.x, _coords.y, comboios.raio, cor.r, cor.g, cor.b);
    RenderText(local, comboios.identificador, _font, &black, g_pRenderer);
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
    // declaração das variáveis
    int off_x = 0;
    int off_y = 0;
    float degree = 0.0;
    float step = M_PI / (_circleR*8);
    
    SDL_SetRenderDrawColor(g_pRenderer, _r, _g, _b, 255);
    
    // desenha pontos até chegar ao centro do círculo (cruzes cada vez mais pequenas)
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
COR stringRGB (char corstr[LINEMAX])
{
    // declaração de variáveis
    COR rgb_value;

    if(strcmp (corstr, "VERMELHO") == 0){
        rgb_value.r = 255;
        rgb_value.g = 0;
        rgb_value.b = 0;
    }
    else if(strcmp (corstr, "ROXO") == 0){
        rgb_value.r = 102;
        rgb_value.g = 0;
        rgb_value.b = 102;
    }
    else if(strcmp (corstr, "AZUL") == 0){
        rgb_value.r = 0;
        rgb_value.g = 0;
        rgb_value.b = 255;
    }
    else if(strcmp (corstr, "CYAN") == 0){
        rgb_value.r = 0;
        rgb_value.g = 255;
        rgb_value.b = 255;
    }
    else if(strcmp (corstr, "VERDE") == 0){
        rgb_value.r = 0;
        rgb_value.g = 255;
        rgb_value.b = 0;
    }
    else if(strcmp (corstr, "AMARELO") == 0){
        rgb_value.r = 255;
        rgb_value.g = 255;
        rgb_value.b = 0;
    }
    else if(strcmp (corstr, "CASTANHO") == 0){
        rgb_value.r = 102;
        rgb_value.g = 51;
        rgb_value.b = 0;
    }
    else if(strcmp (corstr, "PRETO") == 0){
        rgb_value.r = 0;
        rgb_value.g = 0;
        rgb_value.b = 0;
    }
    else if(strcmp (corstr, "BRANCO") == 0){
        rgb_value.r = 255;
        rgb_value.g = 255;
        rgb_value.b = 255;
    }
    else if(strcmp (corstr, "CINZENTO") == 0){
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
 * RenderText: Dá render a texto na janela gráfica
 **\param x e y coordenadas onde vai aparecer o texto
 **\param string com o texto que se pretende apresentar na janela
 **\param _font TTF fonte usada
 **\param _color cor do texto
 */
void RenderText(COORDENADAS local, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* g_pRenderer)
{
    //declaração de variáveis
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = local.x;
    solidRect.y = local.y;

    // cria a superfície a partir da string com a fonte certa
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // cria a textura
    text_texture = SDL_CreateTextureFromSurface(g_pRenderer, text_surface);
    // obtém o tamanho
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // efetua o render
    SDL_RenderCopy(g_pRenderer, text_texture, NULL, &solidRect);
    // liberta a memória
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return;
}

/*
** getCoords: devolve x e y para desenhar os comboios nas coordenadas certas
** \param *lista são retirados da lista as coordenadas dos pontos
** \param num_ferrovias usado para verificar todas as ferrovias
** \param _indent identificação da ferrovia onde o comboio se encontra inicialmente
** \param _ponto identificação do ponto da ferrovia onde o comboio se encontra inicialmente
*/
COORDENADAS getCoords(LINHA *lista, int num_ferrovias, char _ident[LINEMAX], char _ponto[LINEMAX])
{
    // declaração das variáveis
    COORDENADAS ponto = {0,0}; // onde estarão guardadas as coordenadas pretendidas
    int verifica_identidade=0;
    int verifica_ponto = 0;
    LINHA *atual1 = lista;
    LINHA *atual2 = lista;

    // verifica se existe esse identificador de ferrovia
    if (strcmp(atual1->identificador, _ident) == 0) {
        verifica_identidade = 1;
    }
    while (strcmp(atual1->identificador, _ident) != 0) {
        atual1 = atual1->proximo1;
        if (strcmp(atual1->identificador, _ident) == 0) {
            verifica_identidade = 1;
        }
    }

    // verifica se existe esse identificador do ponto
    if (strcmp(atual2->dados.identificador, _ponto) == 0) {
        verifica_ponto = 1;
    }
    while (strcmp(atual2->dados.identificador, _ponto) != 0) {
        atual2 = atual2->proximo1;
        if (strcmp(atual2->dados.identificador, _ponto) == 0) {
            verifica_ponto = 1;
        }
    }
    
    // devolve as coordenadas do ponto  pretendido
    if (verifica_identidade == 1 && verifica_ponto == 1){
        ponto.x = atual2->dados.x;
        ponto.y = atual2->dados.y;
        return ponto;
    }

    return ponto;
}
