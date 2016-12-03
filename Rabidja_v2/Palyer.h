#pragma once
#include "GameObject.h"
#include "Map.h"

class Map;
class Plateforme;
class SDL;
class Menu;
class Sound;

class Palyer :
	public GameObject
{
private:
	int vies, etoiles, level;
	SDL_Texture *playerSpriteSheet;
	const int LEVEL_MAX = 2;

public:
	
	Palyer(SDL * nsdl);
	~Palyer();
	void SetValeurDuNiveau(int value);
	int getLevelMax() const;
	int getLevel() const;
	int getNombreDeVie()const;
	int getNombreDetoiles() const;
	void setNombreDeVie(int value);
	void setNombreDetoiles(int value);

	void killPlayer(Sound * nsound);
	
	void initializePlayer(int newLevel, Map * nMap,Plateforme *nplat);

	void updatePlayer(Input * input, Map * nmap, SDL * nsdl, Plateforme * nplat, Menu * menu, Sound * nsound);

	void drawPlayer(SDL_Renderer * nrenderer, int startX, int startY);

	void getItem(int itemNumber, Sound * nsound);

	void centerScrollingOnPlayer(Map * nMap);




	



	

	
};

