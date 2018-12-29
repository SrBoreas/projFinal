#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funcoes.h"

/*
** função que dá flush ao stdin
*/
void flushinput(void) {

    int k = 0;

    while ((k = getchar()) != '\n' && k != EOF) { }

    return;
}

/*
** função que apresenta o menu e retorna a opcao escolhida
*/
int menu(void) {

    // declaração de variáveis
    int opcao = 0; // opcao escolhida pelo utilizador
    int k = 0; // verificador do valor de retorno do scanf

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
** função que processa os dados do ficheiro config
*/
int lerconfig(char *config[], LINHA ***ferrovias, COMBOIO ***comboios) {

    // declaração de variáveis
    int i = 0; // contador do número de linhas com dados válidos do ficheiro config
    FILE *fp; // apontador para um ficheiro
    char **dados; // matriz dinâmica para guardar os dados do ficheiro
    int j = 0; // contador

    // alocação de memória para a matriz dos dados
    dados = (char **)calloc(FILEMAX, sizeof(char *));
    if (dados == NULL) {
        printf("\nErro ao alocar memória\n");
        return -1;
    }

    //alocação de memória para cada elemento da matriz dos dados
    for (j = 0; j < FILEMAX; j++) {
        dados[j] = (char *)calloc(LINEMAX, sizeof(char));
        if (dados[j] == NULL) {
            printf("\nErro ao alocar memória\n");
            return -1;
        }
    }


    // abertura do ficheiro config
    fp = fopen(*config, "r");

    // verificação da abertura com sucesso
    if (fp == NULL) {
        printf("\nNão foi possível abrir o ficheiro config\n");
        return -1;
    }

    // leitura do ficheiro
    do {
        fgets(dados[i], LINEMAX, fp);
        if ( ( strncmp(dados[i], "%", sizeof(char)) != 0 ) && ( strspn(dados[i], " \n\t") != strlen(dados[i]) ) )
            i++;
    }while(feof(fp) == 0);

    // processamento dos dados

    // libertação de memória
    for (j = 0; j < FILEMAX; j++) {
        free(dados[j]);
    }
    free(dados);

    // fecho do ficheiro
    fclose(fp);

    return 0; // caso de sucesso na abertura do ficheiro
}
