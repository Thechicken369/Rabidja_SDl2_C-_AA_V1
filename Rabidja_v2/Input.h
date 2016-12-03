#pragma once


#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//#include "Menu.h"




class Input
{

struct mapingXbox
	{
		const int  BOUTON_HAUT = 0;
		const int  BOUTON_BAS = 1;
		const int  BOUTON_GAUCHE = 2;
		const int  BOUTON_DROITE = 3;
		const int  BOUTON_SAUT = 10;
		const int  BOUTON_ATTAQUE = 12;
		const int  BOUTON_PAUSE = 4;
		const int  BOUTON_QUIT = 5;
		const int DEAD_ZONE = 8000;

}mapingXbox;


private:
	SDL_Joystick *joystick;
	int DPADinUse = 0;
	int left, right, up, down, jump, attack, enter, erase, pause;


public:
	Input();
	void openJoystick(void);
	void gestionInputs();
	void getInput();
	void getJoystick();

	//Mutateur
	int getLeft() const;
	void setLeft(int value);
	int getRight() const;
	void setRight(int value);
	int getUp() const;
	void setUp(int value);
	int getDown() const;
	void setDown(int value);
	int getJump() const;
	int getAttack() const;
	int getEnter() const;
	void setEnter(int value);
	int getErase() const;
	int getPause() const;

	void setJump(int j);

	
	
	~Input();
};

