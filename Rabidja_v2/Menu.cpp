#include "Menu.h"





Menu::Menu(SDL * nsdl)
{
	titlescreen = nsdl->loadImage("graphics/title.png");
	onMenu = 1;
}

Menu::~Menu()
{
	 //Libère la texture de l'écran-titre 
	if (titlescreen != NULL)
	{
		SDL_DestroyTexture(titlescreen);
		titlescreen = NULL;
	}

}

int Menu::getPause() const
{
	return PAUSE;
}

int Menu::getChoice(void) const
{
	return choice;
}

int Menu::getOnMenu(void)
{
	return onMenu;
}

int Menu::getMenuType(void)
{
	return menuType;
}

void Menu::setOnMenu(int valeur, int type)
{
	onMenu = valeur;
	menuType = type;
}

void Menu::setChoise(int valeur)
{
	choice = valeur;
}

void Menu::drawStartMenu(SDL *nsdl,int level)
{

	//On crée une variable qui contiendra notre texte
	char text[200];

	//On affiche l'écran-titre
	nsdl->drawImage(titlescreen, 0, 0);

	//Si l'option n'est pas en surbrillance, on l'affiche normalement
	if (choice != 0)
	{

		sprintf_s(text, sizeof(text), "START: Lvl %d", level);
		//Ombrage en noir
		nsdl->drawString(text, 375, 252, 0, 0, 0, 255);
		nsdl->drawString(text, 373, 250, 255, 255, 255, 255);
	}
	if (choice != 1)
	{

		sprintf_s(text, sizeof(text), "QUIT");
		//Ombrage en noir
		nsdl->drawString(text, 425, 292, 0, 0, 0, 255);
		nsdl->drawString(text, 422, 290, 255, 255, 255, 255);
	}

	//Si l'option est en surbrillance, on change la couleur
	if (choice == 0)
	{

		sprintf_s(text, sizeof(text), "START: Lvl %d", level);
		//Ombrage en noir
		nsdl->drawString(text, 375, 252, 0, 0, 0, 255);
		nsdl->drawString(text, 373, 250, 255, 255, 0, 255);
	}
	else if (choice == 1)
	{

		sprintf_s(text, sizeof(text), "QUIT");
		//Ombrage en noir
		nsdl->drawString(text, 425, 292, 0, 0, 0, 255);
		nsdl->drawString(text, 422, 290, 255, 255, 0, 255);
	}


}

void Menu::updateStartMenu(Input *input,Palyer *entity,Map *nmap,Plateforme *nplat,SDL *nsdl)
{
	//Si on appuie sur BAS
	if (input->getDown() == 1)
	{
		//Si choice = O (il est sur start), on le met à 1 (quit)
		if (choice == 0)
			choice++;

		input->setDown(0);
	}

	//Si on appuie sur HAUT
	if (input->getUp() == 1)
	{
		//Si choice = 1 (il est sur Quit), on le met à 0 (Start)
		if (choice == 1)
			choice--;

		input->setUp(0);
	}

	//Choix du level
	if (input->getRight() == 1)
	{
		//Si choice = 1 (il est sur Quit), on le met à 0 (Start)
		if (entity->getLevel() >= entity->getLevelMax())
			entity->SetValeurDuNiveau(1);
		else
			entity->SetValeurDuNiveau(entity->getLevel() + 1);

		input->setRight(0);
	}

	if (input->getLeft() == 1)
	{
		//Si choice = 1 (il est sur Quit), on le met à 0 (Start)
		if (entity->getLevel() <= 1)
			entity->SetValeurDuNiveau(entity->getLevelMax());
		else
			entity->SetValeurDuNiveau(entity->getLevel() - 1);

		input->setLeft(0);
	}



	//Si on appuie sur Enter ou A (manette Xbox 360) et qu'on est sur Start, on recharge le jeu et on quitte l'état menu
	if (input->getEnter() || input->getJump())
	{
		if (choice == 0)
		{
			entity->setCheckPoint(0);
			entity->initializePlayer(1,nmap,nplat);
			nmap->changeLevel(nsdl,entity);
			/* On réinitialise les variables du jeu */
			entity->setNombreDeVie(3);
			entity->setNombreDetoiles(0);
			onMenu = 0;
		}
		//Sinon, on quitte le jeu
		else if (choice == 1)
		{
			exit(0);
		}
		input->setEnter(0);
		input->setJump(0);
	}


}

void Menu::drawPauseMenu(SDL *nsdl, int level)
{
	char text[200];

	//On écrit PAUSE

	sprintf_s(text, sizeof(text), "** PAUSE **");
	nsdl->drawString(text, 322, 200, 0, 0, 0, 255);
	nsdl->drawString(text, 320, 198, 255, 255, 255, 255);


	//Si l'option n'est pas en surbrillance, on l'affiche normalement
	if (choice != 0)
	{

		sprintf_s(text, sizeof(text), "Continue");
		//Ombrage en noir
		nsdl->drawString(text, 346, 252, 0, 0, 0, 255);
		nsdl->drawString(text, 344, 250, 255, 255, 255, 255);
	}
	if (choice != 1)
	{

		sprintf_s(text, sizeof(text), "Exit");
		//Ombrage en noir
		nsdl->drawString(text, 386, 292, 0, 0, 0, 255);
		nsdl->drawString(text, 384, 290, 255, 255, 255, 255);
	}

	//Si l'option est en surbrillance, on change la couleur
	if (choice == 0)
	{

		sprintf_s(text, sizeof(text), "Continue");
		//Ombrage en noir
		nsdl->drawString(text, 346, 252, 0, 0, 0, 255);
		nsdl->drawString(text, 344, 250, 255, 255, 0, 255);
	}
	else if (choice == 1)
	{

		sprintf_s(text, sizeof(text), "Exit");
		//Ombrage en noir
		nsdl->drawString(text, 386, 292, 0, 0, 0, 255);
		nsdl->drawString(text, 384, 290, 255, 255, 0, 255);
	}


}

void Menu::updatePauseMenu(Input *input)
{

	//Si on appuie sur BAS
	if (input->getDown() == 1)
	{
		//Si choice = O (il est sur start), on le met à 1 (quit)
		if (choice == 0)
			choice++;

		input->setDown(0);
	}

	//Si on appuie sur HAUT
	if (input->getUp() == 1)
	{
		//Si choice = 1 (il est sur Quit), on le met à 0 (Start)
		if (choice == 1)
			choice--;

		input->setUp(0);
	}

	//Si on appuie sur Enter ou A (manette Xbox 360) et qu'on est sur Start, on recharge le jeu et on quitte l'état menu
	if (input->getEnter() || input->getJump())
	{
		if (choice == 0)
		{
			//Si on appuie sur Enter on quitte l'état menu
			onMenu = 0;
		}
		//Sinon, on quitte le jeu
		else if (choice == 1)
		{
			choice = 0;
			onMenu = 1;
		}
		input->setEnter(0);
		input->setJump(0);
	}

}
