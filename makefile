#makefile versão completa?
all: 
	gcc main1.c funcoes1.c -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99 -L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image -o simulacao
