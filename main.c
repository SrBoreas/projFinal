#include <stdio.h>
#include <stdlib.h>
#include "funcoes.h"

int main (int argc, char *argv[])
{
    //declaração de variáveis
    int opcao = 0; // opção selecionada no menu
    int verabertura = 0; // verifica a abertura com sucesso do ficheiro config
    COMBOIO **comboios; // matriz dinâmica de comboios
    LINHA **ferrovias; // matriz dinâmica de ferrovias
    int i = 0; // contador

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
        // libertação de memória
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

    // leitura do config
    verabertura = lerconfig(argv, &ferrovias, &comboios);

    // validação da leitura
    if (verabertura == -1) {
        // libertação de memória
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

    // ciclo onde o programa corre até ser escholhida a opção 0
    while (1) {

        // apresentação do menu e escolha da opção
        opcao = menu();

        // realização de função com base na opção
        switch(opcao) {
            case 0:
                // desalocação de memória
                for (i = 0; i < CMAX; i++) {
                    free(comboios[i]);
                }
                free(comboios);

                for (i = 0; i < FMAX; i++) {
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
                break;
        }
    }
}
