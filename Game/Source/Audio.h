#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:

	Audio(App* application, bool start_enabled = true);

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	//A�adele al PlayMusic un volumen
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME, int volume = 128);

	// Load a music file
	unsigned int LoadMusic(const char* path);

	//Unload a music file
	bool UnloadMusic(const char* path);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Unload WAV
	bool UnloadFx(unsigned int id);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	// Change music volume (from 0 to 128)
	void ChangeVolume(int volume);

	// Change sfx volume (from 0 to 128)
	void ChangeFXVolume(int volume);

	//Get volume
	int GetVolume();

	int volume = 128;

private:

	_Mix_Music* music;
	List<Mix_Chunk *>	fx;
};

#endif // __AUDIO_H__