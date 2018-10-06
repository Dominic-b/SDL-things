#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <Stuff.h>

/*
To Do:

COLLISION

collision of the sides of the platform needs to be fixed
maybe collision of the dot could be handled using its radius so that
the collision box would be an actual circle, not a square

making it fullscreen and such

ability to have multiple platforms at once
*/
//Screen dimensions
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
void init();

//Frees media and shuts down SDL
void close();

//The window to be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

Texture texture;
Player player;

void init()
{

	//Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	//Create window
	gWindow = SDL_CreateWindow("SDL Testing things", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	//Create vsynced renderer for window
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//Initialize renderer color
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	(IMG_Init(imgFlags) & imgFlags);
}

void close()
{
	//Free loaded images
	texture.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Starts up the SDL and creates th'window
	init();

	//loads image
	texture.loadFromFile("Dot.png");

	//Hides cursor
	SDL_ShowCursor(SDL_DISABLE);

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//Set the wall
	SDL_Rect wall;
	wall.x = SCREEN_WIDTH / 3;
	wall.y = SCREEN_HEIGHT - 200;
	wall.w = SCREEN_WIDTH / 3;
	wall.h = 125;
	//screen width - 640, screen height - 480
	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			//Handle input for the dot
			player.handleEvent(e);
		}

		//Move the dot and check collision
		player.move(wall);

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Render wall
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderDrawRect(gRenderer, &wall);

		//does curser stuff
		player.mouseEvent(e);

		//stuff
		player.projectile();

		//Render dot
		texture.render(gRenderer, player.getPosX(), player.getPosY());

		//Update screen
		SDL_RenderPresent(gRenderer);
	}
	//Free resources and close SDL
	close();
	return 0;
}