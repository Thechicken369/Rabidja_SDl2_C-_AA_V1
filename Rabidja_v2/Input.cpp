#include "Input.h"



Input::Input()
{
}


Input::~Input()
{
	/* Ferme la prise en charge du joystick */
	if (SDL_JoystickGetAttached(joystick))
		SDL_JoystickClose(joystick);

}


void Input::openJoystick(void)
{
	//On ouvre le joystick
	joystick = SDL_JoystickOpen(0);

	if (!joystick)
		fprintf(stderr, "Le joystick 0 n'a pas pu être ouvert !\n");
}

void Input::gestionInputs()
{
	/* On prend en compte les inputs (clavier, joystick... */
	if (joystick != NULL)
	{
		//On vérifie si le joystick est toujours connecté
		if (SDL_NumJoysticks() > 0)
			getJoystick();
		//Sinon on retourne au clavier
		else
		{
			SDL_JoystickClose(joystick);
			joystick = NULL;
		}

	}
	//S'il n'y a pas de manette on gère le clavier
	else
	{
		//On vérifie d'abord si une nouvelle manette a été branchée
		if (SDL_NumJoysticks() > 0)
		{
			//Si c'est le cas, on ouvre le joystick, qui sera opérationnel au prochain tour de boucle
			joystick = SDL_JoystickOpen(0);
			if (!joystick)
				fprintf(stderr, "Couldn't open Joystick 0\n");
		}
		//On gère le clavier
		getInput();
	}
}

void Input::getInput()
{
	SDL_Event event;

	/* Keymapping : gère les appuis sur les touches et les enregistre
	dans la structure input */

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{

		case SDL_QUIT:
			exit(0);
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				exit(0);
				break;

			case SDLK_DELETE:
				erase = 1;
				break;

			case SDLK_c:
				jump = 1;
				break;

			case SDLK_v:
				attack = 1;
				break;

			case SDLK_q:
				left = 1;
				break;

			case SDLK_d:
				right = 1;
				break;

			case SDLK_s:
				down = 1;
				break;

			case SDLK_z:
				up = 1;
				break;


			case SDLK_RETURN:
				enter = 1;
				break;


			default:
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_DELETE:
				erase = 0;
				break;

			case SDLK_c:
				jump = 0;
				break;

			case SDLK_q:
				left = 0;
				break;

			case SDLK_d:
				right = 0;
				break;

			case SDLK_s:
				down = 0;
				break;

			case SDLK_z:
				up = 0;
				break;

			case SDLK_RETURN:
				enter = 0;
				break;

			default:
				break;
			}
			break;

		}

	}
}

void Input::getJoystick()
{
	SDL_Event event;

	//Si on ne touche pas au D-PAD, on le désactive (on teste les 4 boutons du D-PAD)
	if (SDL_JoystickGetButton(joystick, mapingXbox.BOUTON_HAUT ) == 0 && SDL_JoystickGetButton(joystick,mapingXbox.BOUTON_BAS) == 0
		&& SDL_JoystickGetButton(joystick, mapingXbox.BOUTON_DROITE) == 0 && SDL_JoystickGetButton(joystick, mapingXbox.BOUTON_GAUCHE) == 0)
		DPADinUse = 0;

	/* On passe les events en revue */
	while (SDL_PollEvent(&event))
	{


		if (event.type == SDL_QUIT)
			exit(0);


		else if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				exit(0);
				break;

			default:
				break;

			}
		}

		else if (event.type == SDL_JOYBUTTONDOWN)
		{

			if (event.jbutton.button == mapingXbox.BOUTON_SAUT)
				jump = 1;

			else if (event.jbutton.button == mapingXbox.BOUTON_ATTAQUE)
				attack = 1;

			else if (event.jbutton.button == mapingXbox.BOUTON_PAUSE)
				enter = 1;

			else if (event.jbutton.button == mapingXbox.BOUTON_QUIT)
				exit(0);

			else if (event.jbutton.button == mapingXbox.BOUTON_HAUT)
			{
				up = 1;
				DPADinUse = 1;
			}

			else if (event.jbutton.button == mapingXbox.BOUTON_BAS)
			{
				down = 1;
				DPADinUse = 1;
			}

			else if (event.jbutton.button == mapingXbox.BOUTON_GAUCHE)
			{
				left = 1;
				DPADinUse = 1;
			}

			else if (event.jbutton.button == mapingXbox.BOUTON_DROITE)
			{
				right = 1;
				DPADinUse = 1;
			}

		}

		else if (event.type == SDL_JOYBUTTONUP)
		{
			if (event.jbutton.button == mapingXbox.BOUTON_PAUSE)
				enter = 0;

			else if (event.jbutton.button == mapingXbox.BOUTON_SAUT)
				jump = 0;

			else if (event.jbutton.button == mapingXbox.BOUTON_HAUT)
				up = 0;

			else if (event.jbutton.button == mapingXbox.BOUTON_BAS)
				down = 0;

			else if (event.jbutton.button == mapingXbox.BOUTON_GAUCHE)
				left = 0;

			else if (event.jbutton.button == mapingXbox.BOUTON_DROITE)
				right = 0;

		}

		//Gestion du thumbpad, seulement si on n'utilise pas déjà le D-PAD
		else if (event.type == SDL_JOYAXISMOTION && DPADinUse == 0)
		{
			//Si le joystick a détecté un mouvement
			if (event.jaxis.which == 0)
			{
				//Si le mouvement a eu lieu sur l'axe des X
				if (event.jaxis.axis == 0)
				{
					//Si l'axe des X est neutre ou à l'intérieur de la "dead zone"
					if ((event.jaxis.value > -mapingXbox.DEAD_ZONE) && (event.jaxis.value < mapingXbox.DEAD_ZONE))
					{
						right = 0;
						left = 0;
					}
					//Sinon, de quel côté va-t-on ?
					else
					{
						//Si sa valeur est négative, on va à gauche
						if (event.jaxis.value < -mapingXbox.DEAD_ZONE)
						{
							right = 0;
							left = 1;
						}
						//Sinon, on va à droite
						else if (event.jaxis.value > mapingXbox.DEAD_ZONE)
						{
							right = 1;
							left = 0;
						}
					}
				}

				//Si le mouvement a eu lieu sur l'axe des Y
				else if (event.jaxis.axis == 1)
				{
					//Si l'axe des Y est neutre ou à l'intérieur de la "dead zone"
					if ((event.jaxis.value > -mapingXbox.DEAD_ZONE) && (event.jaxis.value < mapingXbox.DEAD_ZONE))
					{
						up = 0;
						down = 0;
					}
					//Sinon, de quel côté va-t-on ?
					else
					{
						//Si sa valeur est négative, on va en haut
						if (event.jaxis.value < 0)
						{
							up = 1;
							down = 0;
						}
						//Sinon, on va en bas
						else
						{
							up = 0;
							down = 1;
						}
					}
				}
			}
		}

	}

}

int Input::getLeft() const
{
	return left;
}

void Input::setLeft(int value)
{
	left = value;
}



int Input::getRight() const
{
	return right;
}

void Input::setRight(int value)
{
	right = value;
}

int Input::getUp() const
{
	return up;
}

void Input::setUp(int value)
{
	up = value;
}

int Input::getDown() const
{
	return down;
}

void Input::setDown(int value)
{
	down = value;
}

int Input::getJump() const
{
	return jump;
}

int Input::getAttack() const
{
	return attack;
}

int Input::getEnter() const
{
	return enter;
}

void Input::setEnter(int value)
{
	enter = value;
}

int Input::getErase() const
{
	return erase;
}

int Input::getPause() const
{
	return pause;
}

void Input::setJump(int j)
{
	jump = j;
}




