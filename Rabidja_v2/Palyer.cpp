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
	//On met le timer � 1 pour tuer le joueur intantan�ment
	this->timerMort = 1;
	//On joue le son
	nsound->playSoundFx(DESTROY);
}

void Palyer::initializePlayer(int newLevel,Map * nMap,Plateforme *nplat)
{

	//PV � 3
	life = 3;

	//Timer d'invincibilit� � 0
	invincibleTimer = 0;


	//Nombre de plateformes flottantes � 0
	nplat->resetPlateformes();

	//Indique l'�tat et la direction de notre h�ros
	direction = RIGHT;
	etat = IDLE;

	//Indique le num�ro de la frame o� commencer
	frameNumber = 0;
	//...la valeur de son chrono ou timer
	frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
	//... et son nombre de frames max (8 pour l'anim' IDLE
	// = ne fait rien)
	frameMax = 8;

	/* Coordonn�es de d�marrage/respawn de notre h�ros */
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

	//On r�initiliase les coordonn�es de la cam�ra 
	//si on change de niveau
	if (newLevel == 1)
	{
		nMap->setStartX(nMap->getBeginX());
		nMap->setStartY(nMap->getBeginY());
	}

	/* Hauteur et largeur de notre h�ros */
	w = PLAYER_WIDTH;
	h = PLAYER_HEIGTH;

	//Variables n�cessaires au fonctionnement de la gestion des collisions
	timerMort = 0;
	onGround = 0;

	
}

void Palyer::updatePlayer(Input *input,Map *nmap,SDL *nsdl,Plateforme *nplat,Menu *menu,Sound *nsound)
{
	//On rajoute un timer au cas o� notre h�ros mourrait lamentablement en tombant dans un trou...
	//Si le timer vaut 0, c'est que tout va bien, sinon, on le d�cr�mente jusqu'� 0, et l�,
	//on r�initialise.
	//C'est pour �a qu'on ne g�re le joueur que si ce timer vaut 0.
	if (timerMort == 0)
	{
		//On g�re le timer de l'invincibilit�
		if (invincibleTimer > 0)
			invincibleTimer--;

		//On r�initialise notre vecteur de d�placement lat�ral (X), pour �viter que le perso
		//ne fonce de plus en plus vite pour atteindre la vitesse de la lumi�re ! ;)
		//Essayez de le d�sactiver pour voir !
		dirX = 0;

		// La gravit� fait toujours tomber le perso : on incr�mente donc le vecteur Y
		dirY += GRAVITY_SPEED;

		//Mais on le limite pour ne pas que le joueur se mette � tomber trop vite quand m�me
		if (dirY >= MAX_FALL_SPEED)
		{
			dirY = MAX_FALL_SPEED;
		}

		//Voil�, au lieu de changer directement les coordonn�es du joueur, on passe par un vecteur
		//qui sera utilis� par la fonction mapCollision(), qui regardera si on peut ou pas d�placer
		//le joueur selon ce vecteur et changera les coordonn�es du player en fonction.
		if (input->getLeft() == 1)
		{
			dirX -= PLAYER_SPEED;
			//Et on indique qu'il va � gauche (pour le flip
			//de l'affichage, rappelez-vous).
			direction = LEFT;

			//Si ce n'�tait pas son �tat auparavant et qu'il est bien sur
			//le sol (car l'anim' sera diff�rente s'il est en l'air)
			if (etat != WALK && onGround == 1)
			{
				//On enregistre l'anim' de la marche et on l'initialise � 0
				etat = WALK;
				frameNumber = 0;
				frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
				frameMax = 8;
			}
		}

		//Si on d�tecte un appui sur la touche fl�ch�e droite
		else if (input->getRight() == 1)
		{
			//On augmente les coordonn�es en x du joueur
			dirX += PLAYER_SPEED;
			//Et on indique qu'il va � droite (pour le flip
			//de l'affichage, rappelez-vous).
			direction = RIGHT;

			//Si ce n'�tait pas son �tat auparavant et qu'il est bien sur
			//le sol (car l'anim' sera diff�rente s'il est en l'air)
			if (etat != WALK && onGround == 1)
			{
				//On enregistre l'anim' de la marche et on l'initialise � 0
				etat = WALK;
				frameNumber = 0;
				frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
				frameMax = 8;
			}
		}

		//Si on n'appuie sur rien et qu'on est sur le sol, on charge l'animation marquant l'inactivit� (Idle)
		else if (input->getRight() == 0 && input->getLeft() == 0 && onGround == 1)
		{
			//On teste si le joueur n'�tait pas d�j� inactif, pour ne pas recharger l'animation
			//� chaque tour de boucle
			if (etat != IDLE)
			{
				//On enregistre l'anim' de l'inactivit� et on l'initialise � 0
				etat = IDLE;
				frameNumber = 0;
				frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;
				frameMax = 8;
			}

		}


		//Et voici la fonction de saut tr�s simple :
		//Si on appuie sur la touche saut et qu'on est sur le sol, alors on attribue une valeur
		//n�gative au vecteur Y
		//parce que sauter veut dire se rapprocher du haut de l'�cran et donc de y=0.
		if (input->getJump() == 1)
		{
			if (onGround == 1)
			{
				dirY = -JUMP_HEIGHT;
				onGround = 0;
				jump = 1;
				nsound->playSoundFx(JUMP);
			}
			/* Si on est en saut 1, on peut faire un deuxi�me bond et on remet jump1 � 0 */
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



		/* R�active la possibilit� de double saut si on tombe sans sauter */
		if (onGround == 1)
			jump = 1;


		//On g�re l'anim du saut
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

		//On rajoute notre fonction de d�tection des collisions qui va mettre � 
		//jour les coordonn�es de notre super lapin.
		nmap->mapCollision(this,nplat,nsdl,nsound);

		//On g�re le scrolling (fonction ci-dessous)
		centerScrollingOnPlayer(nmap);

	}

	//Gestion de la mort quand le h�ros tombe dans un trou :
	//Si timerMort est diff�rent de 0, c'est qu'il faut r�initialiser le joueur.
	//On ignore alors ce qui pr�c�de et on joue cette boucle (un wait en fait) jusqu'� ce que
	// timerMort == 1. A ce moment-l�, on le d�cr�mente encore -> il vaut 0 et on r�initialise
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

			//Sinon on r�initialise le niveau
			nmap->changeLevel(nsdl,this);
			initializePlayer(0,nmap,nplat);

		}
	}
}

void Palyer::drawPlayer(SDL_Renderer * nrenderer,int startX,int startY)
{
	/* Gestion du timer */
	// Si notre timer (un compte � rebours en fait) arrive � z�ro
	if (frameTimer <= 0)
	{
		//On le r�initialise
		frameTimer = TIME_BETWEEN_2_FRAMES_PLAYER;

		//Et on incr�mente notre variable qui compte les frames de 1 pour passer � la suivante
		frameNumber++;

		//Mais si on d�passe la frame max, il faut revenir � la premi�re :
		if (frameNumber >= frameMax)
			frameNumber = 0;

	}
	//Sinon, on d�cr�mente notre timer
	else
		frameTimer--;


	//Ensuite, on peut passer la main � notre fonction

	/* Rectangle de destination � dessiner */
	SDL_Rect dest;

	// On soustrait des coordonn�es de notre h�ros, ceux du d�but de la map, pour qu'il colle
	//au scrolling :
	dest.x = x - startX;
	dest.y = y - startY;
	dest.w = w;
	dest.h = h;

	/* Rectangle source */
	SDL_Rect src;

	//Pour conna�tre le X de la bonne frame � dessiner, il suffit de multiplier
	//la largeur du sprite par le num�ro de la frame � afficher -> 0 = 0; 1 = 40; 2 = 80...
	src.x = frameNumber * w;
	src.w = w;
	src.h = h;

	//On calcule le Y de la bonne frame � dessiner, selon la valeur de l'�tat du h�ros :
	//Aucun Mouvement (Idle) = 0, marche (walk) = 1, etc...
	//Tout cela en accord avec notre spritesheet, of course ;)
	src.y = etat * h;

	//Si on a �t� touch� et qu'on est invincible
	if (invincibleTimer > 0)
	{
		//On fait clignoter le h�ros une frame sur deux
		//Pour �a, on calcule si le num�ro de la frame en 
		//cours est un multiple de deux
		if (frameNumber % 2 == 0)
		{
			//Gestion du flip (retournement de l'image selon que le sprite regarde � droite ou � gauche
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
		//Gestion du flip (retournement de l'image selon que le sprite regarde � droite ou � gauche
		if (direction == LEFT)
			SDL_RenderCopyEx(nrenderer, playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_HORIZONTAL);
		else
			SDL_RenderCopyEx(nrenderer, playerSpriteSheet, &src, &dest, 0, 0, SDL_FLIP_NONE);
	}

}

void Palyer::centerScrollingOnPlayer(Map *nMap)
{
	// Nouveau scrolling � sous-bo�te limite :
	//Pour �viter les effets de saccades dus � une cam�ra qui se 
	//centre automatiquement et constamment sur le joueur (ce qui 
	//peut en rendre malade certains...), on cr�e une "bo�te" imaginaire 
	//autour du joueur. Quand on d�passe un de ses bords (en haut, en bas,
	//� gauche ou � droite), on scrolle.
	//Mais l� encore, au lieu de centrer sur le joueur, on d�place simplement
	//la cam�ra jusqu'� arriver au joueur. On a chang� ici la valeur � 4 pixels 
	//pour que le jeu soit plus rapide. 
	int cxperso = x + w / 2;
	int cyperso = y + h / 2;
	int xlimmin = nMap->getStartX() + LIMITE_X;
	int xlimmax = xlimmin + LIMITE_W;
	int ylimmin = nMap->getStartY() + LIMITE_Y;
	int ylimmax = ylimmin + LIMITE_H;

	//Effet de retour en arri�re quand on est mort :
	//Si on est tr�s loin de la cam�ra, plus loin que le bord
	//de la map, on acc�l�re le scrolling :
	if (cxperso < nMap->getStartX())
	{
		nMap->setStartX(nMap->getStartX() - 30);
	}

	//Si on d�passe par la gauche, on recule la cam�ra
	else if (cxperso < xlimmin)
	{
		nMap->setStartX(nMap->getStartX() - 4);
		int i;
	}

	//Effet de retour en avant quand on est mort (au
	//cas o� le joueur s'amuse � faire le niveau � rebours
	//apr�s une checkpoint) :
	//Si on est tr�s loin de la cam�ra, plus loin que le bord
	//de la map, on acc�l�re le scrolling :
	if (cxperso > nMap->getStartX() + nMap->getScreenWidth())
	{
		nMap->setStartX(nMap->getStartX() + 30);
	}

	//Si on d�passe par la droite, on avance la cam�ra
	else if (cxperso > xlimmax)
	{
		nMap->setStartX(nMap->getStartX() + 4);
	}

	//Si on arrive au bout de la map � gauche, on stoppe le scrolling
	if (nMap->getStartX() < 0)
	{
		nMap->setStartX(0);
	}

	//Si on arrive au bout de la map � droite, on stoppe le scrolling � la 
	//valeur Max de la map - la moiti� d'un �cran (pour ne pas afficher du noir).
	else if (nMap->getStartX() + nMap->getScreenWidth() >= nMap->getMaxX())
	{
		nMap->setStartX(nMap->getMaxX() - nMap->getScreenWidth());
	}

	//Si on d�passe par le haut, on remonte la cam�ra 
	if (cyperso < ylimmin)
	{
		nMap->setStartY(nMap->getStartY() - 4);
	}

	//Si on d�passe par le bas, on descend la cam�ra
	if (cyperso > ylimmax)
	{
		//Sauf si on tombe tr�s vite, auquel cas, on acc�l�re la cam�ra :
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

	//Si on arrive au bout de la map en bas, on stoppe le scrolling � la 
	//valeur Max de la map - la moiti� d'un �cran (pour ne pas afficher du noir ;) ).
	else if (nMap->getStartY() + nMap->getScreenHeight() >= nMap->getMaxY())
	{
		nMap->setStartY(nMap->getMaxY() - nMap->getScreenHeight());
	}

}

void Palyer::getItem(int itemNumber,Sound * nsound)
{
	switch (itemNumber)
	{
		//Gestion des �toiles
	case 1:
		//On incr�mente le compteur Etoile
		etoiles++;
		nsound->playSoundFx(STAR);

		//On teste s'il y a 100 �toiles : on remet le compteur � 0 et on rajoute une vie ;)
		if (etoiles >= 100)
		{
			etoiles = 0;
			//On incr�mente le nombre de vies (max 99)
			if (vies < 99)
				vies++;
		}
		break;

		//Gestion des coeurs
	case 2:
		//On incr�mente le compteur Etoile
		if (life < 3)
			life++;

		nsound->playSoundFx(STAR);
		break;

		//Gestion des vies
	case 3:
		//On incr�mente le nombre de vies (max 99)
		if (vies < 99)
			vies++;

		nsound->playSoundFx(STAR);
		break;

	default:
		break;
	}


}