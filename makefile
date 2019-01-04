#makefile versão completa?
all: simulacao
funcoes.o: funcoes.c funcoes.h
	gcc funcoes.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99 
	-L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image -c funcoes.c
main.o: main.c funcoes.h
	gcc main.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99 
	-L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image -c main.c
simulacao: main.o funcoes.o
	gcc -g -Wall -pedantic -std=c99 -c main.o funcoes.o -o
	simulacao
clean:
	rm funcoes.o main.o simulacao