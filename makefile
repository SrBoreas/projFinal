#makefile versão completa?
all: 
	gcc main.c funcoes.c -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99 -L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image -o simulacao
