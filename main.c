#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL2/SDL_ttf.h>
#include "funcoes.h"
#include <math.h>


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

    int dimx, dimy; //serão definidos no ficheiro config;
    TTF_Font *serif = NULL;
    SDL_Event event;
    int delay = 150;
    int quit = 0;

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

    // validação da leitura
    if (verificador == -1) {
        // desalocação de memória
        for (i = 0; i < CMAX; i++) {
            free(comboios[i]);
        }
        free(comboios);

        for (i = 0; i < FMAX; i++) {
            free(ferrovias[i]->dados);
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
            free(ferrovias[i]->dados);
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
                    free(ferrovias[i]->dados);
                    free(ferrovias[i]);
                }
                free(ferrovias);

                printf("\n"); // por questões estéticas
                return 0; // saída do programa
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                break;
            case 6:

                tamanhoJanela(&dimx, &dimy, num_linhas, dados);

                InitEverything(dimx, dimy, &serif, &window, &g_pRenderer);
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
                    // render da ferrovia
                    renderRailroad(g_pRenderer, *ferrovias, num_ferrovias);
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
