#include "Globals.h"
#include "Application.h"
#include "ModuleAudio.h"
#include "SDL/include/SDL.h"
#pragma comment(lib, "SDL_Mixer/libx86/SDL2_mixer.lib")

ModuleAudio::ModuleAudio() : Module()
{
	for (int i = 0; i < MAX_SONGS; i++)
	{
		songs[i] = nullptr;
	}

	for (int i = 0; i < MAX_CHUNKS; i++)
	{
		chunks[i] = nullptr;
	}
}

// Destructor
ModuleAudio::~ModuleAudio()
{}

// Called before render is available
bool ModuleAudio::Init()
{
	LOG("Init Image library");
	bool ret = true;

	// load support for the OGG AND WAV
	int flags = MIX_INIT_OGG | MIX_INIT_MOD;
	int initted = Mix_Init(flags);

	if (initted&flags != flags) {
		LOG("Mix_Init: Failed to init required ogg and mod support!\n");
		LOG("Mix_Init: %s\n", Mix_GetError());
		ret = false;
	}
	else
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, AUDIO_CHANNELS, 1024) == -1) {
			LOG("Mix_OpenAudio: %s\n", Mix_GetError());
			ret = false;
		}

	return ret;
}

// Called before quitting
bool ModuleAudio::CleanUp()
{
	LOG("Freeing textures and Image library");

	for (int i = MAX_SONGS - 1; i >= 0; i--) {
		if (songs[i] != nullptr)
			Mix_FreeMusic(songs[i]);
	}


	for (int i = MAX_CHUNKS - 1; i >= 0; i--) {
		if (chunks[i] != nullptr)
			Mix_FreeChunk(chunks[i]);
	}

	Mix_Quit();
	return true;
}

// Load new chunk from file path
Mix_Chunk* const ModuleAudio::LoadChunk(const char* path)
{
	int position = 0;
	bool occuped = true;

	do {
		if (chunks[position] == nullptr)
			occuped = false;
		else
			position++;
	} while (occuped && position < MAX_CHUNKS);

	chunks[position] = Mix_LoadWAV(path);

	if (!chunks[position]) {
		LOG("Mix_LoadWAV: %s\n", Mix_GetError());
		return nullptr;
	}
	else
		return chunks[position];
}

// Load new song from file path
Mix_Music* const ModuleAudio::LoadSong(const char* path)
{
	int position = 0;
	bool occuped = true;

	do {
		if (songs[position] == nullptr)
			occuped = false;
		else
			position++;
	} while (occuped && position < MAX_SONGS);

	songs[position] = Mix_LoadMUS(path);

	if (!songs[position]) {
		LOG("Mix_LoadMUS(\"music.mp3\"): %s\n", Mix_GetError());
		return nullptr;
	}
	else
		return songs[position];
}

bool const ModuleAudio::PlaySong(Mix_Music* music) {
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	if (Mix_PlayMusic(music, -1) == -1) {
		LOG("Mix_PlayMusic: %s\n", Mix_GetError());
		return false;
	}
	return true;
}

bool const ModuleAudio::PlaySongDelay(Mix_Music * song, int loops, int ms) {
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	if (Mix_FadeInMusic(song, loops, ms) == -1) {
		LOG("Mix_FadeInMusic: %s\n", Mix_GetError());
		return false;
	}
	return true;
}

bool const ModuleAudio::PlayChunk(Mix_Chunk* chunk) {
	if (Mix_PlayChannel(-1, chunk, 0) == -1) {
		LOG("Mix_PlayChannel: %s\n", Mix_GetError());
		return false;
	}
	return true;
}

bool ModuleAudio::UnloadSong(Mix_Music * song)
{
	bool ret = false;

	if (song != nullptr)
	{
		Mix_FreeMusic(song);
		for (int i = 0; i < MAX_SONGS && !ret; ++i)
		{
			if (songs[i] == song)
			{
				songs[i] = nullptr;
				ret = true;
			}
		}

		Mix_FadeOutMusic(3000);

	}

	return ret;
}
bool ModuleAudio::UnloadChunk(Mix_Chunk * chunk)
{
	bool ret = false;

	if (chunk != nullptr)
	{
		for (int i = 0; i < MAX_CHUNKS && !ret; ++i)
		{
			if (chunks[i] == chunk)
			{
				chunks[i] = nullptr;
				ret = true;
			}
		}
		Mix_FreeChunk(chunk);
	}
	return ret;
}