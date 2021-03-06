/*
    Projeto Final de Programação:
    -Programa que consiste na simulação de uma linha férrea em que
    diversos parâmetros podem ser alterados através do input
    do utilizador

    Pedro Machado ( ist190157 ) | 
                                |   07/01/2019
    Rafael Ferreira (ist190173) | 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include "funcoes.h"

int main(int argc, char *argv[])
{
    //declaração de variáveis
    int opcao; // opção selecionada no menu
    int verificador; // verifica a abertura com sucesso do ficheiro config e das alocações de memória
    COMBOIO **comboios; // matriz dinâmica de comboios
    LINHA **ferrovias; // matriz dinâmica de ferrovias
    int i; // contador
    int num_ferrovias = 0; // guarda o número de ferrovias
    int num_comboios = 0; // guarda o número de comboios
    char **dados; // matriz dinâmica para guardar os dados do ficheiro
    int num_linhas = 0; // número de linhas do ficheiro config

    JANELA janela; //será definido no ficheiro config
    SDL_Renderer* g_pRenderer = NULL; //inicialização do renderer
    SDL_Window *window = NULL; // inicialização da janela gráfica     
    TTF_Font *serif = NULL; // inicialização da fonte textual
    SDL_Event event;
    int delay = 150; // tempo entre cada atualização da janela
    int quit = 0; // usado para sair do programa assim que a janela é fechada
    COORDENADAS coords; // coordenadas dos comboios

    // alocação de memória para os comboios
    comboios = (COMBOIO **)calloc(CMAX, sizeof(COMBOIO *));
    if (comboios == NULL) {
        printf("Erro na alocação de memória");
        return -1;
    }
    for (i = 0; i < CMAX; i++) {
        comboios[i] = (COMBOIO *)malloc(sizeof(COMBOIO));
        if (comboios[i] == NULL) {
            printf("Erro na alocação de memória");
            return -1;
        }
    }

    // alocação de memória para as ferrovias
    ferrovias = (LINHA **)calloc(FMAX, sizeof(LINHA *));
    if (ferrovias == NULL) {
        printf("Erro na alocação de memória");
        return -1;
    }
    for (i = 0; i < FMAX; i++) {
        ferrovias[i] = (LINHA *)malloc(sizeof(LINHA));
        if (ferrovias[i] == NULL) {
            printf("Erro na alocação de memória");
            return -1;
        }
    }

    // verificação do número de argumentos
    if (argc != 2) {
        printf("Número de argumentos inválido. Inserir na forma ./programacompilado nomedoficheiro\n");
        // desalocação de memória
        for (i = 0; i < CMAX; i++) {
            free(comboios[i]);
        }
        free(comboios);
        for (i = 0; i < FMAX; i++) {
            free(ferrovias[i]);
        }
        free(ferrovias);
        return -1;
    }

    // alocação de memória para a matriz dos dados
    dados = (char **)calloc(FILEMAX, sizeof(char *));
    if (dados == NULL) {
        printf("\nErro ao alocar memória\n");
        return -1;
    }
    for (i = 0; i < FILEMAX; i++) {
        dados[i] = (char *)calloc(LINEMAX, sizeof(char));
        if (dados[i] == NULL) {
            printf("\nErro ao alocar memória\n");
            return -1;
        }
    }

    // leitura do config
    verificador = lerconfig(argv[1], dados, &num_linhas);

    // dimensões da janela gráfica
    janela = tamanhoJanela(janela, num_linhas, dados);

    // validação da leitura
    if (verificador == -1) {
        // desalocação de memória
        for (i = 0; i < CMAX; i++) {
            free(comboios[i]);
        }
        free(comboios);

        for (i = 0; i < FMAX; i++) {
            free(ferrovias[i]);
        }
        free(ferrovias);

        for (i = 0; i < FILEMAX; i++) {
        free(dados[i]);
        }
        free(dados);

        return -1;
    }

    // processamento dos dados
    processacomboio(comboios, &num_comboios, num_linhas, dados);

    verificador = processaferrovias(ferrovias, &num_ferrovias, num_linhas, dados);
    if (verificador == -1) {
        // desalocação de memória
        for (i = 0; i < CMAX; i++) {
            free(comboios[i]);
        }
        free(comboios);

        for (i = 0; i < FMAX; i++) {
            free(ferrovias[i]);
        }
        free(ferrovias);

        for (i = 0; i < FILEMAX; i++) {
        free(dados[i]);
        }
        free(dados);

        return -1;
    }

    // processamento das ligações
    verificador = processaligar(ferrovias, num_ferrovias, num_linhas, dados);
    if (verificador == -1) {
        // desalocação de memória
        for (i = 0; i < CMAX; i++) {
            free(comboios[i]);
        }
        free(comboios);

        for (i = 0; i < FMAX; i++) {
            free(ferrovias[i]);
        }
        free(ferrovias);

        for (i = 0; i < FILEMAX; i++) {
        free(dados[i]);
        }
        free(dados);

        return -1;
    }

    // libertação de memória dos dados
    for (i = 0; i < num_linhas; i++) {
        free(dados[i]);
    }
    free(dados);

    // ciclo onde o programa corre até ser escholhida a opção 0
    while (1) {

        // apresentação do menu e escolha da opção
        opcao = menu();

        // realização de função com base na opção
        switch(opcao) {
            case 0:
                // desalocação de memória
                for (i = 0; i < num_comboios; i++) {
                    free(comboios[i]);
                }
                free(comboios);

                for (i = 0; i < num_ferrovias; i++) {
                    free(ferrovias[i]);
                }
                free(ferrovias);

                printf("\n"); // por questões estéticas

                return 0; // saída do programa
            case 1:
                verificador = menu1(ferrovias, num_ferrovias); // apresenta o menu e imprime a ferrovia
                if (verificador == -1) {

                    // desalocação de memória
                    for (i = 0; i < num_comboios; i++) {
                        free(comboios[i]);
                    }
                    free(comboios);

                    for (i = 0; i < num_ferrovias; i++) {
                        free(ferrovias[i]);
                    }
                    free(ferrovias);

                    printf("\n"); // por questões estéticas

                    return -1; // saída do programa

                }

                break;
            case 2:
                break;
            case 3:
                verificador = menu3(comboios, num_comboios);

                if (verificador == -1) {
                    // desalocação de memória
                    for (i = 0; i < num_comboios; i++) {
                        free(comboios[i]);
                    }
                    free(comboios);

                    for (i = 0; i < num_ferrovias; i++) {
                        free(ferrovias[i]);
                    }
                    free(ferrovias);

                    printf("\n"); // por questões estéticas

                    return -1; // saída do programa

                }

                break;
            case 4:
                verificador = menu4(comboios, &num_comboios);

                if (verificador == -1) {
                    // desalocação de memória
                    for (i = 0; i < num_comboios; i++) {
                        free(comboios[i]);
                    }
                    free(comboios);

                    for (i = 0; i < num_ferrovias; i++) {
                        free(ferrovias[i]);
                    }
                    free(ferrovias);

                    printf("\n"); // por questões estéticas

                    return -1; // saída do programa

                }

                break;
            case 5:
                verificador = menu5(comboios, &num_comboios);
                if (verificador == -1) {

                    // desalocação de memória
                    for (i = 0; i < num_comboios; i++) {
                        free(comboios[i]);
                    }
                    free(comboios);

                    for (i = 0; i < num_ferrovias; i++) {
                        free(ferrovias[i]);
                    }
                    free(ferrovias);

                    printf("\n"); // por questões estéticas

                    return -1; // saída do programa

                }

                break;
            case 6:
                InitEverything(janela.dimx, janela.dimy, &serif, &window, &g_pRenderer);
                while( quit == 0 )
                {
                    // enquanto houverem eventos
                    while( SDL_PollEvent( &event ) )
                    {
                        if( event.type == SDL_QUIT )
                        {
                            quit = 1;
                            return 0;
                        }
                    }  
                    // o renderer fica da cor branca
				    SDL_SetRenderDrawColor( g_pRenderer, 255, 255, 255, 255 );

				    // limpa a janela
				    SDL_RenderClear( g_pRenderer );
                    // render das ferrovias
                    renderRailroad(g_pRenderer, ferrovias[0], serif, num_ferrovias);
                    renderRailroad(g_pRenderer, ferrovias[1], serif, num_ferrovias);
                    // render do menu interativo
                    renderMenu (g_pRenderer, janela.dimx, janela.dimy, serif);
                    // render dos comboios
                    for(i = 0; i < num_comboios; i++){
                        coords = getCoords(ferrovias[0], comboios[i]->nome_ferrovia, comboios[i]->nome_ponto);
                        renderTrains(g_pRenderer, *comboios[i], serif, coords);
                    }
                    // aparece no ecrã todas as alterações feitas previamente
                    SDL_RenderPresent(g_pRenderer);
                    // adiciona-se um delay (responsável pela velocidade dos comboios)
                    SDL_Delay( delay );
                }

                // libertação de toda a memória, incluindo as fontes
                TTF_CloseFont(serif);
                SDL_DestroyRenderer(g_pRenderer);
                SDL_DestroyWindow(window);
                SDL_Quit();            
                break;
        }
    }
}
