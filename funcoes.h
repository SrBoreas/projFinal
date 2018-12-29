#ifndef FUNCOES_H
#define FUNCOES_H // Include guards

#define FILEMAX 10000 // número máximo de linhas lidas num ficheiro
#define LINEMAX 100 // número máximo de caractéres lidos numa linha de um ficheiro
#define CMAX 50 // número máximo de comboios
#define FMAX 50 // número máximo de ferrovias

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
    char identificador[2];
    int raio;
    char cor[20];
    char ferrovia[4];
    char ponto[4];
    int viagens;
}COMBOIO;

typedef struct linha{
    PONTO dados;
    struct linha *proximo;
}LINHA; // lista que servirá para formar linhas ferroviárias

/*
** protótipos de funções
*/
void flushinput(void); // função que dá flush ao stdin
int menu(void); // função que apresenta o menu e retorna a opcao escolhida
int lerconfig(char *config[], LINHA ***ferrovias, COMBOIO ***comboios); // função que processa os dados do ficheiro config

#endif
