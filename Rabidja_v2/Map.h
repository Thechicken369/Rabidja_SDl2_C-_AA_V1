#pragma once


#include <fstream>
#include <iostream>
#include "SDL.h"
#include "Menu.h"
#include "Palyer.h"
#include "Plateforme.h"
#include "Sound.h"

class Sound;
class Plateforme;
class Palyer;
class Menu;

enum
{
	BUMPER,
	DESTROY,
	JUMP,
	STAR
};

using namespace std;


class Map
{
private:
	SDL_Texture * background;
	SDL_Texture *tileSet, *tileSetB;

	//Numéro du tileset à utiliser
	int tilesetAffiche;

	/* Coordonnées de départ du héros, lorsqu'il commence le niveau */
	int beginx, beginy;

	/* Coordonnées de début, lorsqu'on doit dessiner la map */
	int startX, startY;

	/* Coordonnées max de fin de la map */
	int maxX, maxY;

	/* Tableau à double dimension représentant la map de tiles */
	int tile[150][400];

	//Deuxième couche de tiles
	int tile2[150][400];

	//Troisième couche de tiles
	int tile3[150][400];

	/* Timer et numéro du tileset à afficher pour animer la map */
	int mapTimer, tileSetNumber;

	const int TIME_BETWEEN_2_FRAMES = 20;
	const int MAX_MAP_X = 400;
	const int MAX_MAP_Y = 150;
	const int TILE_SIZE = 32;
	const int SCREEN_WIDTH = 800;
	const int  SCREEN_HEIGHT = 480;


	//Tiles Power-ups
	const int  TILE_POWER_UP_DEBUT = 67;
	const int  TILE_POWER_UP_FIN = 69;
	const int   TILE_POWER_UP_COEUR = 68;
	const int TILE_CHECKPOINT = 23;
	const int BLANK_TILE = 89;
	const int TILE_RESSORT = 115;
	
	const int TILE_MONSTRE = 126;
	const int TILE_PIKES = 117;

	const int TILE_TRAVERSABLE = 70;

	//Tiles plateformes mobiles
	const int TILE_PLATEFORME_DEBUT = 120;
	const int TILE_PLATEFORME_FIN = 121;
public:
	
	Map(SDL *nsdl);
	SDL_Texture * getBackground() const;
	SDL_Texture *HUD_vie, *HUD_etoiles;
	int getBeginX(void);
	int getBeginY(void);
	int getStartX(void);
	int getStartY(void);
	int getScreenWidth(void) const;
	int getScreenHeight(void) const;
	void setStartY(int valeur);
	void setStartX(int valeur);
	void loadMap(char * name);
	void drawMap(int layer,SDL *nsdl,Plateforme * nPlat);
	void changeLevel(SDL * nsdl, Palyer * entity);
	
	int getTileSize() const;
	int getMaxMapY() const;
	int getMaxMapX() const;
	int getMaxX() const;
	int getMaxY() const;

	void mapCollision(Palyer * entity, Plateforme * nplat, SDL * nsdl, Sound * nsound);
	
	void drawHud(Palyer * entity, SDL * nsdl);

	void initHUD(SDL * nsdl);

	~Map();
};

