#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Module.h"
#include "Globals.h"
#include "SDL_Mixer/include/SDL_mixer.h"

#define MAX_SONGS 50
#define MAX_CHUNKS 50

class ModuleAudio : public Module
{
public:

	ModuleAudio();
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	Mix_Chunk * const LoadChunk(const char* path);
	Mix_Music * const LoadSong(const char* path);

	bool UnloadSong(Mix_Music * song);
	bool UnloadChunk(Mix_Chunk * chunk);

	bool const PlaySong(Mix_Music * song);
	bool const PlaySongDelay(Mix_Music * song, int loops, int ms);
	bool const PlayChunk(Mix_Chunk* chunk);

public:
	Mix_Music * songs[MAX_SONGS];
	Mix_Chunk * chunks[MAX_CHUNKS];
	Mix_Chunk * hdk;
};

#endif // __ModuleAudio_H__