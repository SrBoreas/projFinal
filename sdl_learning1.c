/*
COMPILE USING THE CODE:

gcc sdl_learning1.c -g -I/usr/include/SDL2 -D_REENTRANT -Wall -pedantic -std=c99 
-L/usr/local/lib -lm -lSDL2 -lSDL2_ttf -lSDL2_image -o sdl_learning1

TO RUN:

./sdl_learning1
*/

#include <SDL2/SDL_ttf.h> 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL.h>


#define M_PI 3.14159265
#define WINDOW_POSX 200       // initial position of the window: x
#define WINDOW_POSY 200       // initial position of the window: y

SDL_Window* g_pWindow = NULL;
SDL_Renderer* g_pRenderer = NULL;

typedef struct{
    char identificador[4];
    int x;
    int y;
    int raio;
    char cor[20];
    char tipo[3];
}PONTO; // pontos que unidos formam as ferroests

void renderRailroad (SDL_Renderer * g_pRenderer, PONTO _via[], int NUMVIA, PONTO _estacoes[], int NUMESTACAO);
//void filledCircleRGBA(SDL_Renderer * g_pRenderer, int _circleX, int _circleY, int _circleR, int _r, int _g, int _b);
void InitEverything(int width, int height, TTF_Font **_font, SDL_Window** _window, SDL_Renderer** g_pRenderer);
SDL_Window* CreateWindow(int width, int height);
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window);
void InitFont();



void filledCircleRGBA(SDL_Renderer * g_pRenderer, int _circleX, int _circleY, int _circleR, int _r, int _g, int _b)
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
}

int main(int argc, char* args[])
{
	int dimx, dimy; //serão definidos no ficheiro config;

	//--->TEMPORÁRIO<---//
	dimx = 1080;
	dimy = 720;

	//initialize SDL
	if(SDL_Init(SDL_INIT_EVERYTHING) >= 0){
		//if init ok, create window
		g_pWindow = SDL_CreateWindow("Linha Ferroviária", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED
			, dimx, dimy, SDL_WINDOW_SHOWN);

		//if the createwindow ok, create renderer
		if(g_pWindow != 0){
			g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);
		}
	}
	else{
		return 1;  //big problem time :(
	}

	//Let's get this bread

	//set to black <- expects R G B and alpha
	SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 0, 255);

	//clear the window to black
	SDL_RenderClear(g_pRenderer);

	
	//--->TEMPORÁRIO<---//
	//valores terão de ser originários do ficheiro config

	PONTO p1, p2, p3, p4;   //<------PONTOS DE VIA (não constam na janela gráfica)

	p1.x = 50;
	p1.y = 50;

	p2.x = 200;
	p2.y = 100;

	p3.x = 500;
	p3.y = 200;

	p4.x = 300;
	p4.y = 500;


	PONTO est1, est2, est3;

	est1.x = 75;
	est1.y = 75;

	est2.x = 125;
	est2.y = 150;

	est3.x = 100;
	est3.y = 500;

	PONTO pontos_de_via[3] = {p1, p2, p3};
	PONTO estacoes[3] = {est1, est2, est3};

	//Set render color to blue (rectangle will be rendered in this color)
	//SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 255, 255);

	//SDL_RenderDrawLine(g_pRenderer, est1.x, est1.y, est2.x, est2.y);

	//render das estações (círculos)
	filledCircleRGBA(g_pRenderer, est1.x, est1.y, 20, 255, 0, 0);

	filledCircleRGBA(g_pRenderer, est2.x, est2.y, 20, 255, 0, 0);

	filledCircleRGBA(g_pRenderer, est3.x, est3.y, 20, 255, 0, 0);

	filledCircleRGBA(g_pRenderer, p1.x, p1.y, 5, 255, 255, 0);

	filledCircleRGBA(g_pRenderer, p2.x, p2.y, 5, 255, 255, 0);

	filledCircleRGBA(g_pRenderer, p3.x, p3.y, 5, 255, 255, 0);

	filledCircleRGBA(g_pRenderer, p4.x, p4.y, 5, 255, 255, 0);
	

//	void renderRailroad (SDL_Renderer * g_pRenderer, PONTO _via[], int NUMVIA, PONTO _estacoes[], int NUMESTACAO){
//		for(int i = 0; i < NUMVIA-1; i++){
//			//Set render color to blue (rectangle will be rendered in this color)
//			SDL_SetRenderDrawColor(g_pRenderer, 0, 0, 255, 255);
//
//			SDL_RenderDrawLine(g_pRenderer, _estacoes[i].x, _estacoes[i].y, _estacoes[i+1].x, _estacoes[i+1].y);
//
//			//show the window
//			SDL_RenderPresent(g_pRenderer);
//		}
//	}

	//SDL_SetRenderDrawColor(g_pRenderer, 255, 0, 0, 255);
	//SDL_RenderDrawLine(g_pRenderer, x1, y1, x2, y2);
	//SDL_RenderDrawLine(g_pRenderer, x2, y2, x3, y3);

	//show the window
	SDL_RenderPresent(g_pRenderer);

	//renderRailroad(g_pRenderer, pontos_de_via, 3, estacoes, 3);

	//set a delay before quitting
	SDL_Delay(10000);

	//clean up SDL
	SDL_Quit();
	return 0;
}


/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _font font that will be used to render the text
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
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

    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "Simulação Ferroviária", WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *g_pRenderer;
    // init renderer
    g_pRenderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( g_pRenderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( g_pRenderer, width, height );

    return g_pRenderer;
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}


int delay = 150;
while( quit == 0 )
    {
        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                quit = 1;
                return 0;
            }
          
        // render game table
        square_size_px = RenderTable( board_pos_x, board_pos_y, board_size_px, serif, imgs, renderer);
        // render board
        renderRailroad(g_pRenderer, pontos_de_via, 3, estacoes, 3);
        // render in the screen all changes above
        SDL_RenderPresent(renderer);
        // add a delay (defines the speed of the trains)
        SDL_Delay( delay );
    }