#include "Plateforme.h"



Plateforme::Plateforme(SDL *nsdl)
{

	//On charge l'image de la plateforme
	plateformeTexture = nsdl->loadImage("graphics/plateforme.png");

	/* Si on ne peut pas, on quitte, et on l'indique en erreur ;) */
	if (plateformeTexture == NULL)
	{
		printf("Impossible de charger l'image de la plateforme : graphics/plateforme.png/n");
		exit(1);
	}
}


Plateforme::~Plateforme()
{
	if (plateformeTexture != NULL)
	{
		SDL_DestroyTexture(plateformeTexture);
	}
}

int Plateforme::getPlateformeNumber() const
{
	return nombrePlateformes;
}


void Plateforme::resetPlateformes(void)
{
	nombrePlateformes = 0;
}


void Plateforme::initPlateforme(int x, int y, int type)
{

	/* On rajoute une plateforme à la liste des plateformes */
	nombrePlateformes++;

	//S'il y en a trop, on fait une erreur et on quitte
	if (nombrePlateformes > PLATEFORMES_MAX)
	{
		printf("Trop de plateformes initialisees !\nEnlevez-en ou changez leur nombre max.\n");
		exit(1);
	}

	//On remplit notre structure
	plateforme[nombrePlateformes].setX(x);
	plateforme[nombrePlateformes].setBeginX(x);
	plateforme[nombrePlateformes].setY(y);
	plateforme[nombrePlateformes].setBeginY(y);
	
	//On enregistre la taille de l'image
	int w, h;
	SDL_QueryTexture(plateformeTexture, NULL, NULL, &w, &h);
	plateforme[nombrePlateformes].setW(w);
	plateforme[nombrePlateformes].setH(h);

	plateforme[nombrePlateformes].setType(type);

	//Si elle est du type 2, elle monte, sinon elle va à droite
	if (plateforme[nombrePlateformes].getType() == 2)
		plateforme[nombrePlateformes].setDirection(UP);
	else
		plateforme[nombrePlateformes].setDirection(RIGHT);

}

void Plateforme::drawPlateforme(SDL * nsdl,int startx,int starty)
{

	int i;

	for (i = 1; i <= nombrePlateformes; i++)
	{
		nsdl->drawImage(plateformeTexture, plateforme[i].getX() - startx, plateforme[i].getY() - starty);
	}

}

void Plateforme::doPlateforme(Palyer * nplayer)
{

	int i;

	for (i = 1; i <= nombrePlateformes; i++)
	{
		/* Déplacement UP/DOWN (haut/bas) */
		if (plateforme[i].getType() == 2)
		{
			if (plateforme[i].getDirection() == UP)
			{
				plateforme[i].setY(plateforme[i] .getY()- PLATEFORM_SPEED);
				/* Si le joueur est dessus, on le déplace avec
				pour éviter qu'il ne reste immobile et que la
				plateforme se barre comme sur certains vieux
				(mauvais) jeux...*/
				if (plateforme[i].getPlayerDessu() == 1)
					nplayer->setY(nplayer->getY() - PLATEFORM_SPEED);
			}
			else
			{
				plateforme[i].setY(plateforme[i].getY() + PLATEFORM_SPEED);
				/* Si le joueur est dessus, on le déplace avec */
				if (plateforme[i].getPlayerDessu() == 1)
					nplayer->setY(nplayer->getY() - PLATEFORM_SPEED);
			}

			if (plateforme[i].getY() > plateforme[i].getBeginY() + 5 * TILE_SIZE)
				plateforme[i].setDirection(UP);


			if (plateforme[i].getY()< plateforme[i].getBeginY())
				plateforme[i].setDirection(DOWN);
		}

		/* Déplacement RIGHT/LEFT */
		else
		{
			if (plateforme[i].getDirection() == RIGHT)
			{
				plateforme[i].setX(plateforme[i].getX() + PLATEFORM_SPEED);
				/* Si le joueur est dessus, on le déplace avec */
				if (plateforme[i].getPlayerDessu() == 1)
					nplayer->setX(nplayer->getX() + PLATEFORM_SPEED);
			}
			else
			{
				plateforme[i].setX(plateforme[i].getX() - PLATEFORM_SPEED);
				/* Si le joueur est dessus, on le déplace avec */
				if (plateforme[i].getPlayerDessu() == 1)
					nplayer->setX(nplayer->getX() - PLATEFORM_SPEED);
			}

			//Test : si la plateforme dépasse 5 tiles de longueur,
			//on lui fait faire demi-tour pour ne pas qu'elle
			//fasse toute la map ! :D
			if (plateforme[i].getX() > plateforme[i].getBeginX() + 5 * TILE_SIZE)
				plateforme[i].setDirection(LEFT);

			if (plateforme[i].getX() < plateforme[i].getBeginX())
				plateforme[i].setDirection(RIGHT);
		}

	}

}

void Plateforme::checkCollisionsWithPlateforms(Palyer *entity)
{
	int j;

	for (j = 1; j <= nombrePlateformes; j++)
	{
		int i = entity->getX();
		int lel = plateforme[j].getX() + plateforme[j].getW();
		int merde = 3;
		if (entity->getX() + entity->getW() >= plateforme[j].getX()
			&& entity->getX() <= plateforme[j].getX() + plateforme[j].getW()
			&& entity->getY() + entity->getH() >= plateforme[j].getY()
			&& entity->getY() + entity->getH() < plateforme[j].getY() + 32)
		{
			// Place the player as close to the plateform as possible 
			entity->setY(plateforme[j].getY() - entity->getH());
			entity->setDirY(0);
			entity->setOnGround(1);

			plateforme[j].setPlayerDessu(1);
		}
		else
			plateforme[j].setPlayerDessu(0);
	}

}
