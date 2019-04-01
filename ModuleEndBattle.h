#ifndef __MODULEENDBATTLE_H__
#define __MODULEENDBATTLE_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "SDL_Mixer/include/SDL_mixer.h" //Arreglar

struct SDL_Texture;

class ModuleEndBattle : public Module
{
public:
	ModuleEndBattle();
	~ModuleEndBattle();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	SDL_Texture* graphics = nullptr;
	Mix_Music *music = nullptr;
	SDL_Rect background;
};

#endif // __MODULEENDBATTLE_H__