#pragma once
#include "Map.h"

class Map;

class Sound
{
private:
	Mix_Chunk  *bumper_sound, *destroy_sound, *jump_sound, *star_sound;
	//Musique
	Mix_Music  *musique;
public:
	Sound();
	~Sound();
	void loadSong(char filename[200]);
	void LoadSound(void);
	void playSoundFx(int type);
};

