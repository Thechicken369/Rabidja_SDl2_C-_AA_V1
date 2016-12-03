#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SDL.h"
//#include "Menu.h"
//#include "Input.h"
#include "Palyer.h"
#include "Map.h"



int main(int argc, char * argv[])
{

	unsigned int frameLimit = SDL_GetTicks() + 16;
	int go = 1;

	//Instentiation des objet 
	Input monInput;
	
	SDL maSDL = SDL("Bonjour", monInput);

	Map myMap = Map(&maSDL);
	myMap.initHUD(&maSDL);

	Menu menu = Menu(&maSDL);

	Plateforme mesPlateformes = Plateforme(&maSDL);
	Palyer player = Palyer(&maSDL);

	Sound mySound;

	mySound.loadSong("music/Caviator.mp3");
	mySound.LoadSound();
	

	//myMap.loadMap("map/map1.txt");
	myMap.changeLevel(&maSDL,&player);

	
	player.setNombreDeVie(3);

	player.initializePlayer(0, &myMap,&mesPlateformes);
	// Validate the parameters
	if (argc != 2) {
		printf("usage: %s server-name\n", argv[0]);
		getchar();
		return 1;
	}


	while (go == 1)
	{
		//Gestion des input du jeu 
		monInput.gestionInputs();

		if (menu.getOnMenu() == 0)
		{
			/* On met à jour le jeu */
			player.updatePlayer(&monInput, &myMap, &maSDL, &mesPlateformes, &menu,&mySound);

			mesPlateformes.doPlateforme(&player);



		}
		else
		{
			if (menu.getOnMenu() == 1)
			{ 
				menu.updateStartMenu(&monInput, &player, &myMap, &mesPlateformes, &maSDL);
			}
			
			else if (menu.getOnMenu() == 2)
			{
				menu.updatePauseMenu(&monInput);
			}
		}
		if (menu.getOnMenu() == 0) {

			maSDL.drawImage(myMap.getBackground(), 0, 0);


			myMap.drawMap(2, &maSDL, &mesPlateformes);
			myMap.drawMap(1, &maSDL, &mesPlateformes);
			player.drawPlayer(maSDL.getRenderer(), myMap.getStartX(), myMap.getStartY());
			mesPlateformes.drawPlateforme(&maSDL, myMap.getStartX(), myMap.getStartY());

			myMap.drawMap(3, &maSDL, &mesPlateformes);

			myMap.drawHud(&player, &maSDL);
			SDL_RenderPresent(maSDL.getRenderer());

		}
		else
		{
			if (menu.getOnMenu() == 1)
			{
				menu.drawStartMenu(&maSDL,player.getLevel());
				SDL_RenderPresent(maSDL.getRenderer());
				SDL_Delay(1);
			}
			else if (menu.getOnMenu() == 2)
			{
				maSDL.drawImage(myMap.getBackground(), 0, 0);


				myMap.drawMap(2, &maSDL, &mesPlateformes);
				myMap.drawMap(1, &maSDL, &mesPlateformes);
				player.drawPlayer(maSDL.getRenderer(), myMap.getStartX(), myMap.getStartY());
				mesPlateformes.drawPlateforme(&maSDL, myMap.getStartX(), myMap.getStartY());

				myMap.drawMap(3, &maSDL, &mesPlateformes);

				myMap.drawHud(&player, &maSDL);
				
				menu.drawPauseMenu(&maSDL,player.getLevel());
				SDL_RenderPresent(maSDL.getRenderer());
			}
				

		}

		

		//Dessin du Background 

		maSDL.delay(frameLimit);
		frameLimit = SDL_GetTicks() + 16;
		
	}
	

	

	getchar();
	return 0;
}