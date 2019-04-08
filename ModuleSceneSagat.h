#ifndef __MODULESCENESAGAT_H__
#define __MODULESCENESAGAT_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "SDL_Mixer/include/SDL_mixer.h" //Arreglar

struct SDL_Texture;
struct _Mix_Music;

class ModuleSceneSagat : public Module
{
public:
	ModuleSceneSagat();
	~ModuleSceneSagat();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	SDL_Texture* graphics = nullptr;
	Mix_Music *music = nullptr;
	SDL_Rect ground;
	SDL_Rect roof;
	SDL_Rect foreground;
	SDL_Rect background;
	Animation water;
};

#endif // __MODULESCENESAGAT_H__