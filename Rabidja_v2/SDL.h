#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Input.h"
//#include "Menu.h"



/* On inclut les libs supplémentaires */
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_thread.h>


class Palyer;

class SDL
{
private:
	SDL_Window *screen;
	SDL_Renderer *renderer;

	TTF_Font *font;

	const int SCREEN_WIDTH = 800;
	const int  SCREEN_HEIGHT = 480;
	const int TILE_SIZE = 32;

public:

	SDL_Window * getSDLWidows() const;
	SDL(char * title, Input &input);

	SDL_Texture * loadImage(char * name);

	void drawImage(SDL_Texture * image, int x, int y);

	void drawTile(SDL_Texture * image, int destx, int desty, int srcx, int srcy);

	void drawString(char * text, int x, int y, int r, int g, int b, int a);

	void delay(unsigned int frameLimit);

	SDL_Renderer * getRenderer() const;


	~SDL();
};

