#pragma once

#include "Map.h"
class Map;
class Plateforme;


class Menu
{
private:
	//Gestion des menus
	int onMenu, menuType, choice;
	SDL_Texture *titlescreen;
	const int PAUSE = 2;
	const int START = 1;

public:

	Menu(SDL *nsdl);
	~Menu();
	int getPause() const;
	int getChoice(void) const;
	int getOnMenu(void);
	int getMenuType(void);
	void setOnMenu(int valeur, int type);
	void setChoise(int valeur);
	void updateStartMenu(Input * input, Palyer * entity, Map * nmap, Plateforme * npla, SDL * nsdl);
	void drawPauseMenu(SDL * nsdl, int level);
	void updatePauseMenu(Input * input);
	void drawStartMenu(SDL * nsdl, int level);

	
	
};

