#ifndef __MODULEWELCOMEPAGE_H__
#define __MODULEWELCOMEPAGE_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "SDL_Mixer/include/SDL_mixer.h" //Arreglar

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
	SDL_Texture* graphics = nullptr;
	Mix_Music *music = nullptr;
	SDL_Rect background;
};

#endif // __MODULEWELCOMEPAGE_H__