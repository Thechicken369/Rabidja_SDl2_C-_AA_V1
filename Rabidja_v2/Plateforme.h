#pragma once
#include "GameObject.h"
#include "Map.h"

class Map;
class Palyer;

class Plateforme 
{
private:
	SDL_Texture *plateformeTexture;
	int nombrePlateformes;
	GameObject plateforme[50];

	/* Gestion des plateformes mobiles */
	const int PLATEFORMES_MAX = 50;
	const int PLATEFORM_SPEED = 2;
	const int UP = 3;
	const int DOWN = 4;

	const int TILE_SIZE = 32;
	const int  RIGHT = 1;
	const int  LEFT = 2;

public:
	Plateforme(SDL * nsdl);
	~Plateforme();
	int getPlateformeNumber() const;
	void resetPlateformes(void);
	
	void initPlateforme(int x, int y, int type);
	void drawPlateforme(SDL * nsdl, int startx, int starty);
	void doPlateforme(Palyer * nplayer);

	void checkCollisionsWithPlateforms(Palyer * entity);

};

