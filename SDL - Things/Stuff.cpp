#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <Stuff.h>
#include <tuple>
using namespace std;
Player player1;
Texture texture1;

//Collision stuff
tuple <bool, bool, bool, bool> collision = make_tuple(false, false, false, false);

//The X and Y offsets of the dot
double mPosX;
double mPosY;

//The velocity of the dot
double mVelX;
double mVelY;

//projectile things
double projectileVelX = 0;
double projectileVelY = 0;

double projectilePosX;
double projectilePosY;

Texture::Texture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

Texture::~Texture()
{
	//Deallocate
	free();
}

void Texture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	//Color key image
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

	//Create texture from surface pixels
	mTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);

	//Get image dimensions
	mWidth = loadedSurface->w;
	mHeight = loadedSurface->h;

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);
}

void Texture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void Texture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void Texture::render(SDL_Renderer*, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth()
{
	return mWidth;
}

int Texture::getHeight()
{
	return mHeight;
}

//---------------------------Player Class-----------------------------\\

void Player::handleEvent(SDL_Event& e)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
		jump = true;
	} else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_SPACE) {
		jump = false;
	}
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_a) {
		left = true;
	} else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_a) {
		left = false;
	}
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_d) {
		right = true;
	} else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_d) {
		right = false;
	}
}

void Player::move(SDL_Rect& wall) {
	//Gravity
	if (mVelY < DOT_VEL && (mPosY + DOT_HEIGHT < SCREEN_HEIGHT) && get<0>(collision) == false ) {
		mVelY += 2;
	}

	//Jumping and movement
	if (jump == true && (mPosY + DOT_HEIGHT == SCREEN_HEIGHT || get<0>(collision) == true) && mVelY == 0) {
		mVelY -= DOT_VEL;
	}
	if (left == true && (abs(mVelX) <= DOT_VEL / 2 && get<2>(collision) == false)) {
		mVelX -= 3;
	}
	if (right == true && (abs(mVelX) <= DOT_VEL / 2 && get<3>(collision) == false)) {
		mVelX += 3;
	}
	if (left == false && right == false && (get<0>(collision) == true || mPosY + DOT_HEIGHT == SCREEN_HEIGHT)) {
		mVelX *= .85;
	}

	mCollider.w = DOT_WIDTH;
	mCollider.h = DOT_HEIGHT;

	//Move the dot left or right by the value of VelX and moves the hitbox to the new position of the dot
	mPosX += mVelX;
	mCollider.x = mPosX;

	if (mPosX < 0) {
		mPosX = 0;
	}
	if (mPosX + DOT_WIDTH > SCREEN_WIDTH) {
		mPosX = SCREEN_WIDTH - DOT_WIDTH;
	}
	//Move the dot up or down by the value of VelY and moves the hitbox to the new position of the dot
	mPosY += mVelY;
	mCollider.y = mPosY;

	if (mPosY < 0) {
		mPosY = 0;
		mVelY = 0;
	}
	if (mPosY + DOT_HEIGHT > SCREEN_HEIGHT) {
		mPosY = SCREEN_HEIGHT - DOT_HEIGHT;
		mVelY = 0;
	}
	if (player1.checkCollision(mCollider, wall)) {
		if (get<0>(collision) == true) {
			mPosY = wall.y - DOT_HEIGHT;
			mVelY = 0;
		}
		else if (get<1>(collision) == true) {
			mPosY = wall.y + wall.h;
			mVelY = 0;
		}
		else if (get<2>(collision) == true) {
			mPosX = wall.x - DOT_WIDTH - 1;
			mVelX = 0;
		}
		else if (get<3>(collision) == true) {
			mPosX = wall.x + wall.w;
			mVelX = 0;
		}
	}
}

bool Player::checkCollision(SDL_Rect a, SDL_Rect b) {
	collision = make_tuple(false, false, false, false);
	//if the dot is colliding with the top of the platform
	if (a.y + a.h >= b.y && a.y < b.y && a.x + a.w > b.x && a.x < b.x + b.w) {
		get<0>(collision) = true;
		return true;
	}
	//if the dot is colliding with the bottom of the platform
	else if (a.y < b.y + b.h && a.y > b.y && a.x + a.w > b.x && a.x < b.x + b.w) {
		get<1>(collision) = true;
		return true;
	}
	//if the dot is colliding with the left side of the platform
	else if (a.x + a.w >= b.x && a.x <= b.x && a.y < b.y + b.h && a.y + a.h > b.y) {
		get<2>(collision) = true;
		printf("1");
		return true;
	}
	//if the dot is colliding with the right side of the platform
	else if (a.x <= b.x + b.w && a.x + a.x >= b.x + b.w && a.y < b.y + b.h && a.y + a.h > b.y) {
		get<3>(collision) = true;
		return true;
	}
	return false;
}

void Player::mouseEvent(SDL_Event& e) {
	//maybe split up int player.move and player.handleEvent
	if (e.type == SDL_MOUSEMOTION) {
		SDL_GetMouseState(&mousePos.x, &mousePos.y);
	}

	//Projectile thingos
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		player1.makeProjectile(e);
	}

	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(gRenderer, mousePos.x, mousePos.y, mPosX + DOT_WIDTH / 2, mPosY + DOT_HEIGHT / 2);
	texture1.loadFromFile("Cursor.png");
	texture1.render(gRenderer, mousePos.x, mousePos.y);
}

void Player::makeProjectile(SDL_Event& e) {

	projectileVelX = mVelX;
	projectileVelY = mVelY;

	projectilePosX = mPosX + 12;
	projectilePosY = mPosY + 12;
}

void Player::projectile() {
	
	if (projectileVelY < DOT_VEL && projectilePosY < SCREEN_HEIGHT) {
		projectileVelY += 2;
	}

	//projectile things
	projectileVelX *= 0.975;
	projectilePosX += projectileVelX;
	projectileCollider.x = projectilePosX;
	projectileCollider.w = 8;

	projectilePosY += projectileVelY;
	projectileCollider.y = projectilePosY;
	projectileCollider.h = 8;

	if (projectileCollider.y + projectileCollider.h > SCREEN_HEIGHT) {
		projectileVelY *= -0.9;
		projectilePosY = SCREEN_HEIGHT - projectileCollider.h;
	} else if (projectileCollider.y < 0) {
		projectileVelY *= -0.9;
		projectilePosY = 0;
	}
	if (projectileCollider.x + projectileCollider.w > SCREEN_WIDTH) {
		projectileVelX *= -0.9;
		projectilePosX = SCREEN_WIDTH - projectileCollider.w;
	} else if (projectileCollider.x < 0) {
		projectileVelX *= -0.9;
		projectilePosX = 0;
	}

	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	texture1.loadFromFile("Square.png");
	texture1.render(gRenderer, projectilePosX, projectilePosY);

}

double Player::getPosX() {
	return mPosX;
}
double Player::getPosY() {
	return mPosY;
}

double Player::getVelX() {
	return mVelX;
}
double Player::getVelY() {
	return mVelY;
}