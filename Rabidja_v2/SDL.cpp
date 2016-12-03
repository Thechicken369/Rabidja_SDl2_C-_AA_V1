#include "SDL.h"


SDL_Window * SDL::getSDLWidows() const
{
	return this->screen;
}

SDL::SDL(char *title, Input &input)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {

		fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
		exit(1);
	}

	screen = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	//On crée un renderer pour la SDL et on active la synchro verticale : VSYNC
	renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_PRESENTVSYNC);

	// Si on n'y arrive pas, on quitte en enregistrant l'erreur dans stdout.txt
	if (screen == NULL || renderer == NULL)
	{
		fprintf(stderr, "Impossible d'initialiser le mode écran à %d x %d: %s\n", SCREEN_WIDTH,
			SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}

	//Initialisation du chargement des images png avec SDL_Image 2
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		fprintf(stderr, "SDL_image n'a pu être initialisée! SDL_image Error: %s\n", IMG_GetError());
		exit(1);
	}

	//On cache le curseur de la souris 
	SDL_ShowCursor(SDL_DISABLE);


	//On initialise SDL_TTF 2 qui gérera l'écriture de texte
	if (TTF_Init() < 0)
	{
		fprintf(stderr, "Impossible d'initialiser SDL TTF: %s\n", TTF_GetError());
		exit(1);
	}

	font = TTF_OpenFont("font/GenBasB.ttf", 32);

	if (font == NULL)
	{
		fprintf(stderr, "Failed to open Font %s: %s\n", "font/GenBasB.ttf", TTF_GetError());
		exit(1);
	}

	//On initialise SDL_Mixer 2, qui gérera la musique et les effets sonores
	int flags = MIX_INIT_MP3;
	int initted = Mix_Init(flags);
	if ((initted & flags) != flags)
	{
		fprintf(stderr, "Mix_Init: Failed to init SDL_Mixer\n");
		fprintf(stderr, "Mix_Init: %s\n", Mix_GetError());
		exit(1);
	}

	/* Open 44.1KHz, signed 16bit, system byte order,
	stereo audio, using 1024 byte chunks (voir la doc pour plus d'infos ;) ) */
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		fprintf(stderr, "Mix_OpenAudio: %s\n", Mix_GetError());
		exit(1);
	}

	// Définit le nombre de pistes audio (channels) à mixer
	Mix_AllocateChannels(32);

	// Initialise le sous-sytème de la SDL gérant les joysticks
	SDL_InitSubSystem(SDL_INIT_JOYSTICK);

	//On cherche s'il y a des joysticks
	if (SDL_NumJoysticks() > 0)
	{
		input.openJoystick();
	}
}

SDL_Texture * SDL::loadImage(char *name)
{
	/* Charge les images avec SDL Image dans une SDL_Surface */

	SDL_Surface *loadedImage = NULL;
	SDL_Texture *texture = NULL;
	loadedImage = IMG_Load(name);

	if (loadedImage != NULL)
	{
		// Conversion de l'image en texture
		texture = SDL_CreateTextureFromSurface(renderer, loadedImage);

		// On se débarrasse du pointeur vers une surface
		SDL_FreeSurface(loadedImage);
		loadedImage = NULL;
	}
	else
		fprintf(stderr, "L'image n'a pas pu être chargée! SDL_Error :  %s\n", SDL_GetError());

	return texture;
}

void SDL::drawImage(SDL_Texture *image, int x, int y)
{
	SDL_Rect dest;

	/* Règle le rectangle à dessiner selon la taille de l'image source */
	dest.x = x;
	dest.y = y;

	/* Dessine l'image entière sur l'écran aux coordonnées x et y */
	SDL_QueryTexture(image, NULL, NULL, &dest.w, &dest.h);
	SDL_RenderCopy(renderer, image, NULL, &dest);
}

void SDL::drawTile(SDL_Texture *image, int destx, int desty, int srcx, int srcy)
{
	/* Rectangle de destination à dessiner */
	SDL_Rect dest;

	dest.x = destx;
	dest.y = desty;
	dest.w = TILE_SIZE;
	dest.h = TILE_SIZE;

	/* Rectangle source */
	SDL_Rect src;

	src.x = srcx;
	src.y = srcy;
	src.w = TILE_SIZE;
	src.h = TILE_SIZE;

	/* Dessine la tile choisie sur l'écran aux coordonnées x et y */
	SDL_RenderCopy(renderer, image, &src, &dest);
}

void SDL::drawString(char *text, int x, int y, int r, int g, int b, int a)
{
	SDL_Rect dest;
	SDL_Surface *surface; //Pour écrire le texte
	SDL_Texture *texture; //Pour convertir la surface en texture
	SDL_Color foregroundColor;

	/* Couleur du texte RGBA */
	foregroundColor.r = r;
	foregroundColor.g = g;
	foregroundColor.b = b;
	foregroundColor.a = a;


	/* On utilise SDL_TTF pour générer une SDL_Surface à partir d'une chaîne de caractères (string) */
	surface = TTF_RenderUTF8_Blended(font, text, foregroundColor);

	if (surface != NULL)
	{
		/* NOUS MODIFIONS QUELQUE PEU NOTRE CODE POUR PROFITER DE LA MEMOIRE GRAPHIQUE
		QUI GERE LES TEXTURES  */
		// Conversion de l'image en texture
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		// On se débarrasse du pointeur vers une surface
		/* On libère la SDL_Surface temporaire (pour éviter les fuites de mémoire - cf. chapitre
		dédié) */
		SDL_FreeSurface(surface);
		surface = NULL;

		// On dessine cette texture à l'écran 
		dest.x = x;
		dest.y = y;

		SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
		SDL_RenderCopy(renderer, texture, NULL, &dest);

		//On supprime la texture
		SDL_DestroyTexture(texture);

	}
	else
	{
		fprintf(stderr, "La chaine n'a pu être écrite %s: %s\n", text, TTF_GetError());

		exit(0);
	}
}
void SDL::delay(unsigned int frameLimit)
{
	// Gestion des 60 fps (images/stories/seconde)
	unsigned int ticks = SDL_GetTicks();

	if (frameLimit < ticks)
	{
		return;
	}

	if (frameLimit > ticks + 16)
	{
		SDL_Delay(16);
	}

	else
	{
		SDL_Delay(frameLimit - ticks);
	}
}

SDL_Renderer * SDL::getRenderer() const
{
	return renderer;
}



SDL::~SDL()
{
}
