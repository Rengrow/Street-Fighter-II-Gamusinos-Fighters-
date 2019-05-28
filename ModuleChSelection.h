#ifndef __MODULECHSELECTION_H__
#define __MODULECHSELECTION_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "SDL_Mixer/include/SDL_mixer.h" //Arreglar

struct SDL_Texture;

class ModuleChSelection : public Module
{
public:
	ModuleChSelection();
	~ModuleChSelection();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	SDL_Texture* portraits = nullptr;
	SDL_Rect portrait1;
	SDL_Rect portrait2;
	SDL_Rect vs;
	Uint32 timeScreen;
	bool isFading;
};

#endif // __MODULEENDBATTLE_H__