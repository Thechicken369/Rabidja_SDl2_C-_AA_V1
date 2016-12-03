#include "Palyer.h"



Palyer::Palyer(SDL *nsdl)
{
	playerSpriteSheet = nsdl->loadImage("graphics/rabidja.png");
	level = 1;
}


Palyer::~Palyer()
{
	if (playerSpriteSheet != NULL)
	{
		SDL_DestroyTexture(playerSpriteSheet);
		playerSpriteSheet = NULL;
	}
}

void Palyer::SetValeurDuNiveau(int value)
{
	level = value;
}

int Palyer::getLevelMax() const
{
	return LEVEL_MAX;
}

int Palyer::getLevel() const
{
	return level;
}

int Palyer::getNombreDeVie() const
{
	return vies;
}

int Palyer::getNombreDetoiles() const
{
	return etoiles;
}

void Palyer::setNombreDeVie(int value)
{
	vies = value;
}

void Palyer::setNombreDetoiles(int value)
{
	etoiles = value;
}

void Palyer::killPlayer(Sound * nsound)
{
	//On met le timer à 1 pour tuer le joueur intantanément
	this->timerMort = 1;
	//On joue le son
	nsound->playSoundFx(DESTROY);
}

void Palyer::initializePlayer(int newLevel,Map * nMap,Plateforme *nplat)
{

	//PV à 3
	life = 3;

	//Timer d'invincibilité à 0
	invincibleTimer = 0;


	//Nombre de plateformes flottantes à 0
	nplat->resetPlateformes();

	//Indique l'état et la direction de notre héros
	direction = RIGHT;
	etat = IDLE;

	//Indique le numéro de la frame où commencer
	frameNumber = 0;
	//...la valeur de son chrono ou timer
	frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
	//... et son nombre de frames max (8 pour l'anim' IDLE
	// = ne fait rien)
	frameMax = 8;

	/* Coordonnées de démarrage/respawn de notre héros */
	if (checkpointActif == 1)
	{
		x = respawnX;
		y = respawnY;
	}
	else
	{
		x = nMap->getBeginX();
		y = nMap->getBeginY();
	}

	//On réinitiliase les coordonnées de la caméra 
	//si on change de niveau
	if (newLevel == 1)
	{
		nMap->setStartX(nMap->getBeginX());
		nMap->setStartY(nMap->getBeginY());
	}

	/* Hauteur et largeur de notre héros */
	w = PLAYER_WIDTH;
	h = PLAYER_HEIGTH;

	//Variables nécessaires au fonctionnement de la gestion des collisions
	timerMort = 0;
	onGround = 0;

	
}

void Palyer::updatePlayer(Input *input,Map *nmap,SDL *nsdl,Plateforme *nplat,Menu *menu,Sound *nsound)
{
	//On rajoute un timer au cas où notre héros mourrait lamentablement en tombant dans un trou...
	//Si le timer vaut 0, c'est que tout va bien, sinon, on le décrémente jusqu'à 0, et là,
	//on réinitialise.
	//C'est pour ça qu'on ne gère le joueur que si ce timer vaut 0.
	if (timerMort == 0)
	{
		//On gère le timer de l'invincibilité
		if (invincibleTimer > 0)
			invincibleTimer--;

		//On réinitialise notre vecteur de déplacement latéral (X), pour éviter que le perso
		//ne fonce de plus en plus vite pour atteindre la vitesse de la lumière ! ;)
		//Essayez de le désactiver pour voir !
		dirX = 0;

		// La gravité fait toujours tomber le perso : on incrémente donc le vecteur Y
		dirY += GRAVITY_SPEED;

		//Mais on le limite pour ne pas que le joueur se mette à tomber trop vite quand même
		if (dirY >= MAX_FALL_SPEED)
		{
			dirY = MAX_FALL_SPEED;
		}

		//Voilà, au lieu de changer directement les coordonnées du joueur, on passe par un vecteur
		//qui sera utilisé par la fonction mapCollision(), qui regardera si on peut ou pas déplacer
		//le joueur selon ce vecteur et changera les coordonnées du player en fonction.
		if (input->getLeft() == 1)
		{
			dirX -= PLAYER_SPEED;
			//Et on indique qu'il va à gauche (pour le flip
			//de l'affichage, rappelez-vous).
			direction = LEFT;

			//Si ce n'était pas son état auparavant et qu'il est bien sur
			//le sol (car l'anim' sera différente s'il est en l'air)
			if (etat != WALK && onGround == 1)
			{
				//On enregistre l'anim' de la marche et on l'initialise à 0
				etat = WALK;
				frameNumber = 0;
				frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
				frameMax = 8;
			}
		}

		//Si on détecte un appui sur la touche fléchée droite
		else if (input->getRight() == 1)
		{
			//On augmente les coordonnées en x du joueur
			dirX += PLAYER_SPEED;
			//Et on indique qu'il va à droite (pour le flip
			//de l'affichage, rappelez-vous).
			direction = RIGHT;

			//Si ce n'était pas son état auparavant et qu'il est bien sur
			//le sol (car l'anim' sera différente s'il est en l'air)
			if (etat != WALK && onGround == 1)
			{
				//On enregistre l'anim' de la marche et on l'initialise à 0
				etat = WALK;
				frameNumber = 0;
				frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
				frameMax = 8;
			}
		}

		//Si on n'appuie sur rien et qu'on est sur le sol, on charge l'animation marquant l'inactivité (Idle)
		else if (input->getRight() == 0 && input->getLeft() == 0 && onGround == 1)
		{
			//On teste si le joueur n'était pas déjà inactif, pour ne pas recharger l'animation
			//à chaque tour de boucle
			if (etat != IDLE)
			{
				//On enregistre l'anim' de l'inactivité et on l'initialise à 0
				etat = IDLE;
				frameNumber = 0;
				frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
				frameMax = 8;
			}

		}


		//Et voici la fonction de saut très simple :
		//Si on appuie sur la touche saut et qu'on est sur le sol, alors on attribue une valeur
		//négative au vecteur Y
		//parce que sauter veut dire se rapprocher du haut de l'écran et donc de y=0.
		if (input->getJump() == 1)
		{
			if (onGround == 1)
			{
				dirY = -JUMP_HEIGHT;
				onGround = 0;
				jump = 1;
				nsound->playSoundFx(JUMP);
			}
			/* Si on est en saut 1, on peut faire un deuxième bond et on remet jump1 à 0 */
			else if (jump == 1)
			{
				dirY = -JUMP_HEIGHT;
				jump = 0;
				nsound->playSoundFx(JUMP);
			}
			input->setJump(0);
		}

		//Si on appuie sur Enter
		if (input->getEnter() == 1)
		{
			//On met le jeu en pause
			menu->setOnMenu(2, 1);
			input->setEnter(0);
		}



		/* Réactive la possibilité de double saut si on tombe sans sauter */
		if (onGround == 1)
			jump = 1;


		//On gère l'anim du saut
		if (onGround == 0)
		{
			//Si on est en saut 1, on met l'anim' du saut normal
			if (jump == 1)
			{
				if (etat != JUMP1)
				{
					etat = JUMP1;
					frameNumber = 0;
					frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
					frameMax = 2;
				}
			}
			else
			{
				if (etat != JUMP2)
				{
					etat = JUMP2;
					frameNumber = 0;
					frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
					frameMax = 4;
				}
			}
		}

		//On rajoute notre fonction de détection des collisions qui va mettre à 
		//jour les coordonnées de notre super lapin.
		nmap->mapCollision(this,nplat,nsdl,nsound);

		//On gère le scrolling (fonction ci-dessous)
		centerScrollingOnPlayer(nmap);

	}

	//Gestion de la mort quand le héros tombe dans un trou :
	//Si timerMort est différent de 0, c'est qu'il faut réinitialiser le joueur.
	//On ignore alors ce qui précède et on joue cette boucle (un wait en fait) jusqu'à ce que
	// timerMort == 1. A ce moment-là, on le décrémente encore -> il vaut 0 et on réinitialise
	//le jeu avec notre bonne vieille fonction d'initialisation ;) !
	if (timerMort > 0)
	{
		timerMort--;

		if (timerMort == 0)
		{
			// Si on est mort, on perd une vie 
			vies -= 1; 

			//Sauf si on a plus de vies...
			if (vies < 0)
			{
				//Dans ce cas on retourne au menu start
				menu->setOnMenu(1, 1);
			}

			//Sinon on réinitialise le niveau
			nmap->changeLevel(nsdl,this);
			initializePlayer(0,nmap,nplat);

		}
	}
}

void Palyer::drawPlayer(SDL_Renderer * nrenderer,int startX,int startY)
{
	/* Gestion du timer */
	// Si notre timer (un compte à rebours en fait) arrive à zéro
	if (frameTimer <= 0)
	{
		//On le réinitialise
		frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;

		//Et on incrémente notre variable qui compte les frames de 1 pour passer à la suivante
		frameNumber++;

		//Mais si on dépasse la frame max, il faut revenir à la première :
		if (frameNumber >= frameMax)
			frameNumber = 0;

	}
	//Sinon, on décrémente notre timer
	else
		frameTimer--;


	//Ensuite, on peut passer la main à notre fonction

	/* Rectangle de destination à dessiner */
	SDL_Rect dest;

	// On soustrait des coordonnées de notre héros, ceux du début de la map, pour qu'il colle
	//au scrolling :
	dest.x = x - startX;
	dest.y = y - startY;
	dest.w = w;
	dest.h = h;

	/* Rectangle source */
	SDL_Rect src;

	//Pour connaître le X de la bonne frame à dessiner, il suffit de multiplier
	//la largeur du sprite par le numéro de la frame à afficher -> 0 = 0; 1 = 40; 2 = 80...
	src.x = frameNumber * w;
	src.w = w;
	src.h = h;

	//On calcule le Y de la bonne frame à dessiner, selon la valeur de l'état du héros :
	//Aucun Mouvement (Idle) = 0, marche (walk) = 1, etc...
	//Tout cela en accord avec notre spritesheet, of course ;)
	src.y = etat * h;

	//Si on a été touché et qu'on est invincible
	if (invincibleTimer > 0)
	{
		//On fait clignoter le héros une frame sur deux
		//Pour ça, on calcule si le numéro de la frame en 
		//cours est un multiple de deux
		if (frameNumber % 2 == 0)
		{
			//Gestion du flip (retournement de l'image selon que le sprite regarde à droite ou à gauche
			if (direction == LEFT)
				SDL_RenderCopyEx(nrenderer, playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_HORIZONTAL);
			else
				SDL_RenderCopyEx(nrenderer, playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_NONE);
		}
		//Sinon, on ne dessine rien, pour le faire clignoter
	}

	//Sinon, on dessine normalement
	else
	{
		//Gestion du flip (retournement de l'image selon que le sprite regarde à droite ou à gauche
		if (direction == LEFT)
			SDL_RenderCopyEx(nrenderer, playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_HORIZONTAL);
		else
			SDL_RenderCopyEx(nrenderer, playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_NONE);
	}

}

void Palyer::centerScrollingOnPlayer(Map *nMap)
{
	// Nouveau scrolling à sous-boîte limite :
	//Pour éviter les effets de saccades dus à une caméra qui se 
	//centre automatiquement et constamment sur le joueur (ce qui 
	//peut en rendre malade certains...), on crée une "boîte" imaginaire 
	//autour du joueur. Quand on dépasse un de ses bords (en haut, en bas,
	//à gauche ou à droite), on scrolle.
	//Mais là encore, au lieu de centrer sur le joueur, on déplace simplement
	//la caméra jusqu'à arriver au joueur. On a changé ici la valeur à 4 pixels 
	//pour que le jeu soit plus rapide. 
	int cxperso = x + w / 2;
	int cyperso = y + h / 2;
	int xlimmin = nMap->getStartX() + LIMITE_X;
	int xlimmax = xlimmin + LIMITE_W;
	int ylimmin = nMap->getStartY() + LIMITE_Y;
	int ylimmax = ylimmin + LIMITE_H;

	//Effet de retour en arrière quand on est mort :
	//Si on est très loin de la caméra, plus loin que le bord
	//de la map, on accélère le scrolling :
	if (cxperso < nMap->getStartX())
	{
		nMap->setStartX(nMap->getStartX() - 30);
	}

	//Si on dépasse par la gauche, on recule la caméra
	else if (cxperso < xlimmin)
	{
		nMap->setStartX(nMap->getStartX() - 4);
		int i;
	}

	//Effet de retour en avant quand on est mort (au
	//cas où le joueur s'amuse à faire le niveau à rebours
	//après une checkpoint) :
	//Si on est très loin de la caméra, plus loin que le bord
	//de la map, on accélère le scrolling :
	if (cxperso > nMap->getStartX() + nMap->getScreenWidth())
	{
		nMap->setStartX(nMap->getStartX() + 30);
	}

	//Si on dépasse par la droite, on avance la caméra
	else if (cxperso > xlimmax)
	{
		nMap->setStartX(nMap->getStartX() + 4);
	}

	//Si on arrive au bout de la map à gauche, on stoppe le scrolling
	if (nMap->getStartX() < 0)
	{
		nMap->setStartX(0);
	}

	//Si on arrive au bout de la map à droite, on stoppe le scrolling à la 
	//valeur Max de la map - la moitié d'un écran (pour ne pas afficher du noir).
	else if (nMap->getStartX() + nMap->getScreenWidth() >= nMap->getMaxX())
	{
		nMap->setStartX(nMap->getMaxX() - nMap->getScreenWidth());
	}

	//Si on dépasse par le haut, on remonte la caméra 
	if (cyperso < ylimmin)
	{
		nMap->setStartY(nMap->getStartY() - 4);
	}

	//Si on dépasse par le bas, on descend la caméra
	if (cyperso > ylimmax)
	{
		//Sauf si on tombe très vite, auquel cas, on accélère la caméra :
		if (dirY >= MAX_FALL_SPEED - 2)
		{
			nMap->setStartY(nMap->getStartY() + MAX_FALL_SPEED + 1);
		}
		else
		{
			nMap->setStartY(nMap->getStartY() + 4);
		}
	}

	//Si on arrive au bout de la map en haut, on stoppe le scrolling
	if (nMap->getStartY() < 0)
	{
		nMap->setStartY(0);
	}

	//Si on arrive au bout de la map en bas, on stoppe le scrolling à la 
	//valeur Max de la map - la moitié d'un écran (pour ne pas afficher du noir ;) ).
	else if (nMap->getStartY() + nMap->getScreenHeight() >= nMap->getMaxY())
	{
		nMap->setStartY(nMap->getMaxY() - nMap->getScreenHeight());
	}

}

void Palyer::getItem(int itemNumber,Sound * nsound)
{
	switch (itemNumber)
	{
		//Gestion des étoiles
	case 1:
		//On incrémente le compteur Etoile
		etoiles++;
		nsound->playSoundFx(STAR);

		//On teste s'il y a 100 étoiles : on remet le compteur à 0 et on rajoute une vie ;)
		if (etoiles >= 100)
		{
			etoiles = 0;
			//On incrémente le nombre de vies (max 99)
			if (vies < 99)
				vies++;
		}
		break;

		//Gestion des coeurs
	case 2:
		//On incrémente le compteur Etoile
		if (life < 3)
			life++;

		nsound->playSoundFx(STAR);
		break;

		//Gestion des vies
	case 3:
		//On incrémente le nombre de vies (max 99)
		if (vies < 99)
			vies++;

		nsound->playSoundFx(STAR);
		break;

	default:
		break;
	}


}