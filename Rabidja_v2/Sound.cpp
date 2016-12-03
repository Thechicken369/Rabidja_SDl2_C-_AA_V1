#include "Sound.h"



Sound::Sound()
{
}


Sound::~Sound()
{
	/* On libère la musique */
	if (musique != NULL)
		Mix_FreeMusic(musique);

	Mix_FreeChunk(bumper_sound);
	Mix_FreeChunk(destroy_sound);
	Mix_FreeChunk(jump_sound);
	Mix_FreeChunk(star_sound);
}

void Sound::loadSong(char filename[200])
{

	/* On libère la chanson précédente s'il y en a une */
	if (musique != NULL)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(musique);
	}

	/* On charge la nouvelle chanson */
	musique = Mix_LoadMUS(filename);
	if (musique == NULL)
	{
		fprintf(stderr, "Can't read the music \n");
		exit(1);
	}

	/* On active la répétition de la musique à l'infini */
	if (Mix_PlayMusic(musique, -1) == -1)
	{
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
	}

}

void Sound::LoadSound(void)
{

	bumper_sound = Mix_LoadWAV("sounds/bumper.wav");
	if (bumper_sound == NULL)
	{
		fprintf(stderr, "Can't read the bumper sound FX \n");
		exit(1);
	}

	destroy_sound = Mix_LoadWAV("sounds/destroy.wav");
	if (destroy_sound == NULL)
	{
		fprintf(stderr, "Can't read the destroy sound FX \n");
		exit(1);
	}

	jump_sound = Mix_LoadWAV("sounds/jump.wav");
	if (jump_sound == NULL)
	{
		fprintf(stderr, "Can't read the jump sound FX \n");
		exit(1);
	}

	star_sound = Mix_LoadWAV("sounds/star.wav");
	if (star_sound == NULL)
	{
		fprintf(stderr, "Can't read the star sound FX \n");
		exit(1);
	}

}

void Sound::playSoundFx(int type)
{

	switch (type)
	{

	case BUMPER:
		Mix_PlayChannel(-1, bumper_sound, 0);
		break;

	case DESTROY:
		Mix_PlayChannel(-1, destroy_sound, 0);
		break;

	case JUMP:
		Mix_PlayChannel(-1, jump_sound, 0);
		break;

	case STAR:
		Mix_PlayChannel(-1, star_sound, 0);
		break;

	}


}