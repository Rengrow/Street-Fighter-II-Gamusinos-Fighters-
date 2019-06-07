#ifndef __MODULEVSSCREEN_H__
#define __MODULEVSSCREEN_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "SDL_Mixer/include/SDL_mixer.h" //Arreglar

struct SDL_Texture;

class ModuleVsScreen : public Module
{
public:
	ModuleVsScreen();
	~ModuleVsScreen();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	SDL_Texture* portraits = nullptr;
	SDL_Rect portrait1;
	SDL_Rect portrait2;
	SDL_Rect vs;
	Mix_Chunk* music = nullptr;
	Uint32 timeScreen;
	bool isFading;
	int typography;
};

#endif // __MODULEENDBATTLE_H__