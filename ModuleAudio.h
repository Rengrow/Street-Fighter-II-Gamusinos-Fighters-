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
	bool const PlaySong(const int song);
	bool const PlayChunk(const int chunk);

public:
	Mix_Music * songs[MAX_SONGS];
	Mix_Chunk * chunks[MAX_CHUNKS];

};

#endif // __ModuleAudio_H__