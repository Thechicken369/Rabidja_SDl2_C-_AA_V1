#include "Map.h"



Map::Map(SDL *nsdl)
{
	// Charge l'image du fond (background)  
	background = nsdl->loadImage("graphics/background.png");

	//On initialise le timer
	mapTimer = TIME_BETWEEN_2_FRAMES * 3;
	tileSetNumber = 0;
}

SDL_Texture * Map::getBackground() const
{
	return background;
}

int Map::getBeginX(void)
{
	return beginx;
}

int Map::getBeginY(void)
{
	return beginy;
}

int Map::getStartX(void)
{
	return startX;
}

int Map::getStartY(void)
{
	return startY;
}

int Map::getScreenWidth(void) const
{
	return SCREEN_WIDTH;
}

int Map::getScreenHeight(void) const
{
	return SCREEN_HEIGHT;
}

void Map::setStartY(int valeur)
{
	this->startY = valeur;
}

void Map::setStartX(int valeur)
{
	this->startX = valeur;
}

void Map::loadMap(char *name)
{
	int x, y;
	

	fstream  fichier(name, ios::in | ios::out | ios::app);

	//Ouvre le fichier en lecture (renvoie une erreur s'il n'existe pas)
	if (!fichier)
	{
		fprintf(stderr, "Le fichier map n'a pas pu etre ouvert.\n");
		exit(1);
	}


	fichier >> beginx;
	fichier >> beginy;

	/* lit la val de tileset */

	fichier >> tilesetAffiche;

	maxX = maxY = 0;

	for (y = 0; y < MAX_MAP_Y; y++)
	{
		for (x = 0; x < MAX_MAP_X; x++)
		{
			/* On lit le num�ro de la tile et on le copie dans notre tableau */

			fichier >> tile[y][x];

			/* Permet de d�terminer la taille de la map (voir plus bas) */
			if (tile[y][x] > 0)
			{
				if (x > maxX)
				{
					maxX = x;
				}

				if (y > maxY)
				{
					maxY = y;
				}
			}
		}
	}

	//Deuxi�me couche de tiles
	for (y = 0; y < MAX_MAP_Y; y++)
	{
		for (x = 0; x < MAX_MAP_X; x++)
		{
			/* On lit le num�ro de la tile et on le copie dans notre tableau */

			fichier >> tile2[y][x];
		}
	}

	//Troisi�me couche de tiles
	for (y = 0; y < MAX_MAP_Y; y++)
	{
		for (x = 0; x < MAX_MAP_X; x++)
		{
			/* On lit le num�ro de la tile et on le copie dans notre tableau */

			fichier >> tile3[y][x];
		}
	}

	/* maxX et maxY sont les coordonn�es de fin de la 
	On les trouve d�s qu'il n'y a plus que des z�ros � la suite.
	Comme �a, on peut faire des maps de tailles diff�rentes avec la m�me
	structure de fichier. */
	maxX = (maxX + 1) * TILE_SIZE;
	maxY = (maxY + 1) * TILE_SIZE;

	/* Et on referme le fichier */
	fichier.close();

}

void Map::drawMap(int layer,SDL *nsdl,Plateforme *nPlat)
{
	int x, y, mapX, x1, x2, mapY, y1, y2, xsource, ysource, a;

	/* On initialise mapX � la 1�re colonne qu'on doit blitter.
	Celle-ci correspond au x de la map (en pixels) divis�s par la taille d'une tile (32)
	pour obtenir la bonne colonne de notre map
	Exemple : si x du d�but de la map = 1026, on fait 1026 / 32
	et on sait qu'on doit commencer par afficher la 32eme colonne de tiles de notre map */
	mapX = startX / TILE_SIZE;

	/* Coordonn�es de d�part pour l'affichage de la map : permet
	de d�terminer � quels coordonn�es blitter la 1�re colonne de tiles au pixel pr�s
	(par exemple, si la 1�re colonne n'est visible qu'en partie, on devra commencer � blitter
	hors �cran, donc avoir des coordonn�es n�gatives - d'o� le -1). */
	x1 = (startX % TILE_SIZE) * -1;

	/* Calcul des coordonn�es de la fin de la map : jusqu'o� doit-on blitter ?
	Logiquement, on doit aller � x1 (d�part) + SCREEN_WIDTH (la largeur de l'�cran).
	Mais si on a commenc� � blitter en dehors de l'�cran la premi�re colonne, il
	va falloir rajouter une autre colonne de tiles sinon on va avoir des pixels
	blancs. C'est ce que fait : x1 == 0 ? 0 : TILE_SIZE qu'on pourrait traduire par:
	if(x1 != 0)
	x2 = x1 + SCREEN_WIDTH + TILE_SIZE , mais forc�ment, c'est plus long ;)*/
	x2 = x1 + SCREEN_WIDTH + (x1 == 0 ? 0 : TILE_SIZE);

	/* On fait exactement pareil pour calculer y */
	mapY = startY / TILE_SIZE;
	y1 = (startY % TILE_SIZE) * -1;
	y2 = y1 + SCREEN_HEIGHT + (y1 == 0 ? 0 : TILE_SIZE);


	//On met en place un timer pour animer la map 
	if (mapTimer <= 0)
	{
		if (tileSetNumber == 0)
		{
			tileSetNumber = 1;
			mapTimer = TIME_BETWEEN_2_FRAMES * 3;
		}
		else
		{
			tileSetNumber = 0;
			mapTimer = TIME_BETWEEN_2_FRAMES * 3;
		}
	}
	else
		mapTimer--;


	/* Dessine la carte en commen�ant par startX et startY */

	/* On dessine ligne par ligne en commen�ant par y1 (0) jusqu'� y2 (480)
	A chaque fois, on rajoute TILE_SIZE (donc 32), car on descend d'une ligne
	de tile (qui fait 32 pixels de hauteur) */
	if (layer == 1)
	{
		for (y = y1; y < y2; y += TILE_SIZE)
		{
			/* A chaque d�but de ligne, on r�initialise mapX qui contient la colonne
			(0 au d�but puisqu'on ne scrolle pas) */
			mapX = startX / TILE_SIZE;

			/* A chaque colonne de tile, on dessine la bonne tile en allant
			de x = 0 � x = 640 */
			for (x = x1; x < x2; x += TILE_SIZE)
			{

				//Si la tile � dessiner n'est pas une tile vide
				if (tile[mapY][mapX] != 0)
				{

					/*On teste si c'est une tile plateforme flottante */
					if (tile[mapY][mapX] >= TILE_PLATEFORME_DEBUT
						&& tile[mapY][mapX] <= TILE_PLATEFORME_FIN)
					{
						//On initialise une plateforme flottante
						//Pour obtenir le type (dernier arg), on enl�ve le num�ro de la 1�re tile plateforme
						//(TILE_PLATEFORME_DEBUT) au num�ro de la til en cours (tile[mapY][mapX])
						//et on rajoute 1 pour que le premier type soit le 1 et pas le 0 ;)
						nPlat->initPlateforme(mapX * TILE_SIZE, mapY * TILE_SIZE, tile[mapY][mapX] - TILE_PLATEFORME_DEBUT + 1);

						//Et on efface cette tile de notre tableau pour �viter un spawn de plateformes
						//infini !
						tile[mapY][mapX] = 0;
					}
				}

				/* Suivant le num�ro de notre tile, on d�coupe le tileset (a = le num�ro
				de la tile */
				a = tile[mapY][mapX];

				/* Calcul pour obtenir son y (pour un tileset de 10 tiles
				par ligne, d'o� le 10 */
				ysource = a / 10 * TILE_SIZE;
				/* Et son x */
				xsource = a % 10 * TILE_SIZE;

				/* Fonction qui blitte la bonne tile au bon endroit suivant le timer */
				if (tileSetNumber == 0)
					nsdl->drawTile(tileSet, x, y, xsource, ysource);
				else
					nsdl->drawTile(tileSetB, x, y, xsource, ysource);

				mapX++;
			}

			mapY++;
		}
	}

	else if (layer == 2)
	{
		//Deuxi�me couche de tiles ;)
		for (y = y1; y < y2; y += TILE_SIZE)
		{
			mapX = startX / TILE_SIZE;

			for (x = x1; x < x2; x += TILE_SIZE)
			{
				//Si la tile � dessiner n'est pas une tile vide
				if (tile2[mapY][mapX] != 0)
				{


					/*On teste si c'est une tile plateforme flottante */
					if (tile2[mapY][mapX] >= TILE_PLATEFORME_DEBUT
						&& tile2[mapY][mapX] <= TILE_PLATEFORME_FIN)
					{
						//On initialise une plateforme flottante
						//Pour obtenir le type (dernier arg), on enl�ve le num�ro de la 1�re tile plateforme
						//(TILE_PLATEFORME_DEBUT) au num�ro de la til en cours (tile[mapY][mapX])
						//et on rajoute 1 pour que le premier type soit le 1 et pas le 0 ;)
						nPlat->initPlateforme(mapX * TILE_SIZE, mapY * TILE_SIZE, tile2[mapY][mapX] - TILE_PLATEFORME_DEBUT + 1);
						//Et on efface cette tile de notre tableau pour �viter un spawn de plateformes
						//infini !
						tile2[mapY][mapX] = 0;
					}
				}

				/* Suivant le num�ro de notre tile, on d�coupe le tileset (a = le num�ro
				de la tile */
				a = tile2[mapY][mapX];

				/* Calcul pour obtenir son y (pour un tileset de 10 tiles
				par ligne, d'o� le 10 */
				ysource = a / 10 * TILE_SIZE;
				/* Et son x */
				xsource = a % 10 * TILE_SIZE;

				/* Fonction qui blitte la bonne tile au bon endroit suivant le timer */
				if (tileSetNumber == 0)
					nsdl->drawTile(tileSet, x, y, xsource, ysource);
				else
					nsdl->drawTile(tileSetB, x, y, xsource, ysource);

				mapX++;
			}
			mapY++;
		}
	}

	else if (layer == 3)
	{
		//Troisi�me couche de tiles ;)
		for (y = y1; y < y2; y += TILE_SIZE)
		{
			mapX = startX / TILE_SIZE;

			for (x = x1; x < x2; x += TILE_SIZE)
			{
				//Si la tile � dessiner n'est pas une tile vide
				if (tile3[mapY][mapX] != 0)
				{


					/*On teste si c'est une tile plateforme flottante */
					if (tile3[mapY][mapX] >= TILE_PLATEFORME_DEBUT
						&& tile3[mapY][mapX] <= TILE_PLATEFORME_FIN)
					{
						//On initialise une plateforme flottante
						//Pour obtenir le type (dernier arg), on enl�ve le num�ro de la 1�re tile plateforme
						//(TILE_PLATEFORME_DEBUT) au num�ro de la til en cours (tile[mapY][mapX])
						//et on rajoute 1 pour que le premier type soit le 1 et pas le 0 ;)
						nPlat->initPlateforme(mapX * TILE_SIZE, mapY * TILE_SIZE, tile3[mapY][mapX] - TILE_PLATEFORME_DEBUT + 1);
						//Et on efface cette tile de notre tableau pour �viter un spawn de plateformes
						//infini !
						tile3[mapY][mapX] = 0;
					}
				}

				/* Suivant le num�ro de notre tile, on d�coupe le tileset (a = le num�ro
				de la tile */
				a = tile3[mapY][mapX];

				/* Calcul pour obtenir son y (pour un tileset de 10 tiles
				par ligne, d'o� le 10 */
				ysource = a / 10 * TILE_SIZE;
				/* Et son x */
				xsource = a % 10 * TILE_SIZE;

				/* Fonction qui blitte la bonne tile au bon endroit suivant le timer */
				if (tileSetNumber == 0)
					nsdl->drawTile(tileSet, x, y, xsource, ysource);
				else
					nsdl->drawTile(tileSetB, x, y, xsource, ysource);

				mapX++;
			}
			mapY++;
		}
	}


}

void Map::changeLevel(SDL *nsdl,Palyer *entity)
{

	char file[200];

	/* Charge la map depuis le fichier */

	sprintf_s(file, sizeof(file), "map/map%d.txt", entity->getLevel());
	loadMap(file);

	//Charge le tileset
	if (tileSet != NULL)
	{
		SDL_DestroyTexture(tileSet);
	}
	if (tileSetB != NULL)
	{
		SDL_DestroyTexture(tileSetB);
	}

	
	sprintf_s(file, sizeof(file), "graphics/tileset%d.png", tilesetAffiche);
	tileSet = nsdl->loadImage(file);

	sprintf_s(file, sizeof(file), "graphics/tileset%dB.png", tilesetAffiche);
	tileSetB = nsdl->loadImage(file);

}

int Map::getTileSize() const
{
	return TILE_SIZE;
}

int Map::getMaxMapY() const
{
	return MAX_MAP_Y;
}

int Map::getMaxMapX() const
{
	return MAX_MAP_X;
}

int Map::getMaxX() const
{
	return maxX;
}

int Map::getMaxY() const
{
	return maxY;
}

void Map::mapCollision(Palyer *entity,Plateforme *nplat,SDL *nsdl,Sound *nsound)
{

	int i, x1, x2, y1, y2;

	/* D'abord, on consid�re le joueur en l'air jusqu'� temps
	d'�tre s�r qu'il touche le sol */
	//entity->setOnGround(0);

	/* Ensuite, on va tester les mouvements horizontaux en premier
	(axe des X). On va se servir de i comme compteur pour notre boucle.
	En fait, on va d�couper notre sprite en blocs de tiles pour voir
	quelles tiles il est susceptible de recouvrir.
	On va donc commencer en donnant la valeur de Tile_Size � i pour qu'il
	teste la tile o� se trouve le x du joueur mais aussi la suivante SAUF
	dans le cas o� notre sprite serait inf�rieur � 	la taille d'une tile.
	Dans ce cas, on lui donnera la vraie valeur de la taille du sprite
	Et on testera ensuite 2 fois la m�me tile. Mais comme �a notre code
	sera op�rationnel quelle que soit la taille de nos sprites ! */

	if (entity->getH() > TILE_SIZE)
		i = TILE_SIZE;
	else
		i = entity->getH();


	//On lance alors une boucle for infinie car on l'interrompra selon 
	//les r�sultats de nos calculs
	for (;;)
	{
		//On va calculer ici les coins de notre sprite � gauche et � 
		//droite pour voir quelle tile ils touchent.
		x1 = (entity->getX() + entity->getDirX()) / TILE_SIZE;
		x2 = (entity->getX() + entity->getDirX() + entity->getW() - 1) / TILE_SIZE;

		//M�me chose avec y, sauf qu'on va monter au fur et � mesure 
		//pour tester toute la hauteur de notre sprite, gr�ce � notre 
		//fameuse variable i.
		y1 = (entity->getY()) / TILE_SIZE;
		y2 = (entity->getY() + i - 1) / TILE_SIZE;

		//De l�, on va tester les mouvements initi�s dans updatePlayer 
		//gr�ce aux vecteurs dirX et dirY, tout en testant avant qu'on 
		//se situe bien dans les limites de l'�cran.
		if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
		{
			//Si on a un mouvement � droite
			if (entity->getDirX() > 0)
			{

				//Test des tiles Power-up
				if (tile[y1][x2] >= TILE_POWER_UP_DEBUT
					&& tile[y1][x2] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					entity->getItem(tile[y1][x2] - TILE_POWER_UP_DEBUT + 1, nsound);

					//On remplace la tile power-up par une tile transparente
					tile[y1][x2] = 0;
				}
				else if (tile[y2][x2] >= TILE_POWER_UP_DEBUT
					&& tile[y2][x2] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction entity->getItem()
					entity->getItem(tile[y2][x2] - TILE_POWER_UP_DEBUT + 1, nsound);

					//On remplace la tile power-up par une tile transparente
					tile[y2][x2] = 0;
				}


				//Test de la tile checkpoint
				if (tile[y1][x2] == TILE_CHECKPOINT)
				{
					//On active le bool�en checkpoint
					entity->setCheckPoint(1);

					//On enregistre les coordonn�es
					entity->setRespawn(x2 * TILE_SIZE, (y1 * TILE_SIZE) - entity->getH());
					

					//On change la tile
					tile[y1][x2] += 1;
				}
				else if (tile[y2][x2] == TILE_CHECKPOINT)
				{
					//On active le bool�en checkpoint
					entity->setCheckPoint(1);

					//On enregistre les coordonn�es
					entity->setRespawn(x2 * TILE_SIZE, (y2 * TILE_SIZE) - entity->getH());
					
					//On change la tile
					tile[y2][x2] += 1;
				}


				//On v�rifie si les tiles recouvertes sont solides
				if (tile[y1][x2] > BLANK_TILE || tile[y2][x2] > BLANK_TILE)
				{
					// Si c'est le cas, on place le joueur aussi pr�s que possible
					// de ces tiles, en mettant � jour ses coordonn�es. Enfin, on 
					//r�initialise son vecteur d�placement (dirX).

					entity->setX(x2 * TILE_SIZE);
					entity->setX(entity->getX() - (entity->getW() + 1));
					entity->setDirX(0);

				}
			}

			//M�me chose � gauche
			else if (entity->getDirX() < 0)
			{

				//Test des tiles Power-up : Etoile et vie
				if (tile[y1][x1] >= TILE_POWER_UP_DEBUT
					&& tile[y1][x1] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction entity->getItem()
					entity->getItem(tile[y1][x1] - TILE_POWER_UP_DEBUT + 1, nsound);

					//On remplace la tile power-up par une tile transparente
					tile[y1][x1] = 0;
				}
				else if (tile[y2][x1] >= TILE_POWER_UP_DEBUT
					&& tile[y2][x1] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					entity->getItem(tile[y2][x1] - TILE_POWER_UP_DEBUT + 1, nsound);

					//On remplace la tile power-up par une tile transparente
					tile[y2][x1] = 0;
				}

				//Test de la tile checkpoint
				if (tile[y1][x1] == TILE_CHECKPOINT)
				{
					//On active le bool�en checkpoint
					entity->setCheckPoint(1);

					//On enregistre les coordonn�es
					entity->setRespawn(x1 * TILE_SIZE, (y1 * TILE_SIZE) - entity->getH());
				
					//On change la tile
					tile[y1][x1] += 1;
				}
				else if (tile[y2][x1] == TILE_CHECKPOINT)
				{
					//On active le bool�en checkpoint
					entity->setCheckPoint(1);

					//On enregistre les coordonn�es
					entity->setRespawn(x1 * TILE_SIZE, (y2 * TILE_SIZE) - entity->getH());
					

					//On change la tile
					tile[y2][x1] += 1;
				}


				if (tile[y1][x1] > BLANK_TILE || tile[y2][x1] > BLANK_TILE)
				{
					entity->setX((x1 + 1) * TILE_SIZE);
					entity->setDirX(0);
				}

			}

		}

		//On sort de la boucle si on a test� toutes les tiles le long de la hauteur du sprite.
		if (i == entity->getH())
		{
			break;
		}

		//Sinon, on teste les tiles sup�rieures en se limitant � la heuteur du sprite.
		i += TILE_SIZE;

		if (i > entity->getH())
		{
			i = entity->getH();
		}
	}

	//On recommence la m�me chose avec le mouvement vertical (axe des Y)
	if (entity->getW() > TILE_SIZE)
		i = TILE_SIZE;
	else
		i = entity->getW();


	for (;;)
	{
		x1 = (entity->getX()) / TILE_SIZE;
		x2 = (entity->getX() + i) / TILE_SIZE;

		y1 = (entity->getY() + entity->getDirY()) / TILE_SIZE;
		y2 = (entity->getY() + entity->getDirY() + entity->getH()) / TILE_SIZE;

		if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
		{
			if (entity->getDirY() > 0)
			{
				/* D�placement en bas */

				//Test des tiles Power-up)
				if (tile[y2][x1] >= TILE_POWER_UP_DEBUT
					&& tile[y2][x1] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					entity->getItem(tile[y2][x1] - TILE_POWER_UP_DEBUT + 1, nsound);

					//On remplace la tile power-up par une tile transparente
					tile[y2][x1] = 0;
				}
				else if (tile[y2][x2] >= TILE_POWER_UP_DEBUT
					&& tile[y2][x2] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					entity->getItem(tile[y2][x2] - TILE_POWER_UP_DEBUT + 1, nsound);

					//On remplace la tile power-up par une tile transparente
					tile[y2][x2] = 0;
				}

				//Test de la tile checkpoint
				if (tile[y2][x1] == TILE_CHECKPOINT)
				{
					//On active le bool�en checkpoint
					entity->setCheckPoint(1);

					//On enregistre les coordonn�es
					entity->setRespawn(x1 * TILE_SIZE, (y2 * TILE_SIZE) - entity->getH());
				

					//On change la tile
					tile[y2][x1] += 1;
				}
				else if (tile[y2][x2] == TILE_CHECKPOINT)
				{
					//On active le bool�en checkpoint
					entity->setCheckPoint(1);

					//On enregistre les coordonn�es
					entity->setRespawn(x2 * TILE_SIZE, (y2 * TILE_SIZE) - entity->getH());
					

					//On change la tile
					tile[y2][x2] += 1;
				}


				/* Gestion des pics */
				if ((tile[y2][x1] == TILE_PIKES) || (tile[y2][x2] == TILE_PIKES))
				{

					//On joue le son
					nsound->playSoundFx(DESTROY);
					//On fait sauter le joueur
					entity->setDirY(-entity->getJumpHeight());

					if (entity->getLife() > 1)
					{
						//Si le timer d'invincibilit� est � 0
						//on perd un coeur
						if (entity->getInvincibleTimer() == 0)
						{
							entity->setLife(entity->getLife()-1);
							entity->setInvincibleTimer(80);
						}
					}
					else
					{
						//On met le timer � 1 pour tuer le joueur intantan�ment
						entity->setTimerMort(1);
						//On joue le son
						nsound->playSoundFx(DESTROY);
					}
				}

				/* Gestion du ressort */
				else if ((tile[y2][x1] == TILE_RESSORT) || (tile[y2][x2] == TILE_RESSORT))
				{
					entity->setDirY(-20);
					//On indique au jeu qu'il a atterri pour r�initialiser le double saut
					entity->setOnGround(1);
					nsound->playSoundFx(BUMPER);
				}


				//Gestion des plateformes traversables : elles se situent juste avant 
				//les tiles bloquantes dans notre tileset (dont la valeur butoire est 
				//BLANK_TILE). Il suffit donc d'utiliser le num�ro de la premi�re tile 
				//traversable au lieu de BLANK_TILE pour bloquer le joueur, 
				//seulement quand il tombe dessus (sinon, il passe au-travers
				//et le test n'est donc pas effectu� dans les autres directions 
				else if (tile[y2][x1] > TILE_TRAVERSABLE || tile[y2][x2] > TILE_TRAVERSABLE)
				{
					//Si la tile est une plateforme ou une tile solide, on y colle le joueur et
					//on le d�clare sur le sol (onGround).
					entity->setY( y2 * TILE_SIZE);
					entity->setY(entity->getY() - entity->getH());
					entity->setDirY(0);
					entity->setOnGround(1);
					entity->getDirX();
				}


				// Test de collision avec la plateforme mobile 
				if (nplat->getPlateformeNumber() > 0)
					nplat->checkCollisionsWithPlateforms(entity);

			}

			else if (entity->getDirY() < 0)
			{

				/* D�placement vers le haut */

				//Test des tiles Power-up 
				if (tile[y1][x1] >= TILE_POWER_UP_DEBUT
					&& tile[y1][x1] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					entity->getItem(tile[y1][x1] - TILE_POWER_UP_DEBUT + 1, nsound);

					//On remplace la tile power-up par une tile transparente
					tile[y1][x1] = 0;
				}
				if (tile[y1][x2] >= TILE_POWER_UP_DEBUT
					&& tile[y1][x2] <= TILE_POWER_UP_FIN)
				{
					//On appelle la fonction getItem()
					entity->getItem(tile[y1][x2] - TILE_POWER_UP_DEBUT + 1, nsound);

					//On remplace la tile power-up par une tile transparente
					tile[y1][x2] = 0;
				}

				//Test de la tile checkpoint
				if (tile[y1][x1] == TILE_CHECKPOINT)
				{
					//On active le bool�en checkpoint
					entity->setCheckPoint(1);

					//On enregistre les coordonn�es
					entity->setRespawn(x1 * TILE_SIZE, (y2 * TILE_SIZE) - entity->getH());
					

					//On change la tile
					tile[y1][x1] += 1;
				}
				else if (tile[y1][x2] == TILE_CHECKPOINT)
				{
					//On active le bool�en checkpoint
					entity->setCheckPoint(1);

					//On enregistre les coordonn�es
					entity->setRespawn(x2 * TILE_SIZE, (y1 * TILE_SIZE) - entity->getH());
					

					//On change la tile
					tile[y1][x2] += 1;
				}


				if (tile[y1][x1] > BLANK_TILE || tile[y1][x2] > BLANK_TILE)
				{
					entity->setY((y1 + 1) * TILE_SIZE);
					entity->setDirY(0);
				}

			}
		}

		//On teste la largeur du sprite (m�me technique que pour la hauteur pr�c�demment)
		if (i == entity->getW())
		{
			break;
		}

		i += TILE_SIZE;

		if (i > entity->getW())
		{
			i = entity->getW();
		}
	}

	/* Maintenant, on applique les vecteurs de mouvement si le sprite n'est pas bloqu� */
	entity->setX(entity->getX() + entity->getDirX());
	entity->setY(entity->getY() + entity->getDirY());

	//Et on contraint son d�placement aux limites de l'�cran.
	if (entity->getX() < 0)
	{
		entity->setX(0);
	}

	else if (entity->getX() + entity->getW() >= maxX)
	{
		//Si on touche le bord droit de l'�cran, on passe au niveau sup
		entity->SetValeurDuNiveau(entity->getLevel() + 1);
		//Si on d�passe le niveau max, on annule et on limite le d�placement du joueur
		if (entity->getLevel() > entity->getLevelMax())
		{
			entity->SetValeurDuNiveau(entity->getLevelMax());
			entity->setX(maxX - entity->getW() - 1);
		}
		//Sinon, on passe au niveau sup, on charge la nouvelle map et on r�initialise le joueur
		else
		{
			//On d�sactive le checkpoint
			entity->setCheckPoint(0);

			changeLevel(nsdl,entity);
			entity->initializePlayer(1, this,nplat);
			entity->SetValeurDuNiveau(2);
		}
	}

	//Maintenant, s'il sort de l'�cran par le bas (chute dans un trou sans fond), on lance le timer
	//qui g�re sa mort et sa r�initialisation (plus tard on g�rera aussi les vies).
	if (entity->getY() > maxY)
	{
		entity->setTimerMort(60);
	}
}

void Map::drawHud(Palyer *entity,SDL *nsdl)
{
	//On cr�e une varuiable qui contiendra notre texte (jusqu'� 200 caract�res, y'a de la marge ;) ).
	char text[200];

	int i;

	//Affiche le nombre de coeurs
	//On cr�e une boucle pour afficher de 1 � 3 coeurs
	//selon la vie, avec un d�calage de 32 pixels
	for (i = 0; i < entity->getLife(); i++)
	{
		// Calcul pour d�couper le tileset comme dans la fonction drawMap()
		int ysource = TILE_POWER_UP_COEUR / 10 * TILE_SIZE;
		int xsource = TILE_POWER_UP_COEUR % 10 * TILE_SIZE;

		nsdl->drawTile(tileSet, 20 + i * 32, 20, xsource, ysource);
	}

	/* Affiche le nombre de vies en bas � droite - Adaptation � la fen�tre auto */
	nsdl->drawImage(HUD_vie, SCREEN_WIDTH - 120, SCREEN_HEIGHT - 70);

	//Pour afficher le nombre de vies, on formate notre string pour qu'il prenne la valeur de la variable
	//sprintf(text, "x %d", getNombreDeVies());   //Version d�pr�ci�e remplac�e par :
	sprintf_s(text, sizeof(text), "x %d", entity->getNombreDeVie());


	//Puis on utilise notre fonction cr��e pr�c�demment pour �crire en noir (0, 0, 0, 255)
	//et en blanc (255, 255, 255, 255) afin de surligner le texte et de le rendre plus
	//visible
	nsdl->drawString(text, SCREEN_WIDTH - 80, SCREEN_HEIGHT - 60, 0, 0, 0, 255);
	nsdl->drawString(text, SCREEN_WIDTH - 82, SCREEN_HEIGHT - 62, 255, 255, 255, 255);

	/* Affiche le nombre d'�toiles en haut � gauche */
	nsdl->drawImage(HUD_etoiles, 60, 60);

	sprintf_s(text, sizeof(text), "%d", entity->getNombreDetoiles());
	nsdl->drawString(text, 100, 57, 0, 0, 0, 255);
	nsdl->drawString(text, 98, 55, 255, 255, 255, 255);

}

void Map::initHUD(SDL *nsdl)
{
	/* On charge les images du HUD */
	HUD_etoiles = nsdl->loadImage("graphics/stars.png");
	HUD_vie = nsdl->loadImage("graphics/life.png");
}
	

Map::~Map()
{
	// Lib�re la texture du background 
	if (background != NULL)
	{
		SDL_DestroyTexture(background);
		background = NULL;
	}

	// Lib�re les textures des tilesets 
	if (tileSet != NULL)
	{
		SDL_DestroyTexture(tileSet);
		tileSet = NULL;
	}

	if (tileSetB != NULL)
	{
		SDL_DestroyTexture(tileSetB);
		tileSetB = NULL;
	}
}
