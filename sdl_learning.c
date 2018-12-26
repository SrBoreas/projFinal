#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b)    (((a)>(b))?(a):(b))
#define M_PI 3.14159265
#define STRING_SIZE 100       // max size for some strings
#define WINDOW_WIDTH 1080     // window width
#define WINDOW_HEIGHT 720     // window height
#define WINDOW_POSX 200       // initial position of the window: x
#define WINDOW_POSY 200       // initial position of the window: y

typedef struct{
	char *name;
	int carriages;
	int size;
	int color;
	int initialpos;
	int finalpos;
}train;

void RenderWindow (SDL_Renderer *, int);

int main(int argc, char *argv[]){
	
	int quit = 0;					//If 1, closes the window

	SDL_Window **window;			//Declare a window
	SDL_Renderer *renderer;			//Declare a renderer

	SDL_Init(SDL_INIT_EVERYTHING);  //Initialize SDL2



 	SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Event event;
    int delay = 300;
    int quit = 0;
    int width = WINDOW_WIDTH;
    int height = WINDOW_HEIGHT;
    int pt_x = 0, pt_y = 0;




	//Create the application window with the following settings:
	window = SDL_CreateWindow("Train Simulation", 			//Window Title
							  SDL_WINDOWPOS_UNDEFINED,		//Initial x position of the window
							  SDL_WINDOWPOS_UNDEFINED,		//Initial x position of the window
							  1080,							//Window width, in pixels
							  720,							//Window height, in pixels
							  0								//Flags
							  );

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	RenderWindow(renderer, quit);

	return 0;
}


RenderWindow(SDL_Renderer *renderer, int quit){

	//Set the drawing color to blue
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

	//Clear the screen (to the color specified before)
	SDL_RenderClear(renderer);

	//Set the drawing color to white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	//Draw 3 white rectangles with black outline
	SDL_Rect rect;
	int height = 10;
	for(int i = 0; i<3; i++){
		height = height + (i * 10) + 2;
		rect = {1040, height, 30, height};
		SDL_RenderFillRect(renderer, &rect);
		SDL_RenderDrawRect(renderer, &rect);
		SDL_RenderPresent(renderer);
	}
	while (quit != 1){
		SDL_Delay();
	}
	
	

}



/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _font font that will be used to render the text
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}


