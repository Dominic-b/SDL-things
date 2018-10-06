#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <tuple>
using namespace std;
#pragma once

//The window renderer
//\/ - Solution to dot not rendering
extern SDL_Renderer* gRenderer;

class Texture
{
public:

	//Initializes variables
	Texture();

	//Deallocates memory
	~Texture();

	//Loads image at specified path
	void loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(SDL_Renderer*, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//private:
		//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

//---------------------------Player Class-----------------------------\\

class Player {
public:
	//Screen Sizey thingo
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	//The dimensions of the dot
	static const int DOT_WIDTH = 32;
	static const int DOT_HEIGHT = 32;

	//Maximum axis velocity of the dot
	static const int DOT_VEL = 30;
	// and projectilos
	static const int projectileVel = 30;

	//Takes key presses and adjusts the dot's velocity
	void handleEvent(SDL_Event& e);

	//Handles cursor events
	void mouseEvent(SDL_Event& e);

	//Makes new projectiles
	void makeProjectile(SDL_Event& e);
	//Handles projectiles
	void projectile();

	//Moves the dot and checks collision
	void move(SDL_Rect& wall);

	//Box collision detector
	bool checkCollision(SDL_Rect a, SDL_Rect b);

	double getPosX();
	double getPosY();

	double getVelX();
	double getVelY();

private:

	//Movement booleans
	bool jump = false;
	bool left = false;
	bool right = false;

	//The Cursors position
	SDL_Point mousePos;

	//Dot's collision box
	SDL_Rect mCollider;

	SDL_Rect projectileCollider;

};