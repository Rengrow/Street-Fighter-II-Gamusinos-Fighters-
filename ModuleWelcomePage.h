#ifndef __MODULEWELCOMEPAGE_H__
#define __MODULEWELCOMEPAGE_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "SDL_Mixer/include/SDL_mixer.h"

struct Mix_Chunk;
struct SDL_Texture;

class ModuleWelcomePage : public Module
{
public:
	ModuleWelcomePage();
	~ModuleWelcomePage();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	SDL_Texture* graphics1 = nullptr;
	SDL_Texture* graphics2 = nullptr;
	Mix_Music* music = nullptr;
	Mix_Chunk* start = nullptr;
	SDL_Rect background;

	int fila;
	int columna;
	bool segundaSheet;
	int typography;
	Uint32 timeLetters;
	bool lettersOn;
};

#endif // __MODULEWELCOMEPAGE_H__