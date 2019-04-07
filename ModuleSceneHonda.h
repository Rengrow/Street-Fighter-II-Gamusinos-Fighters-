#ifndef __MODULESCENEHONDA_H__
#define __MODULESCENEHONDA_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "SDL_Mixer/include/SDL_mixer.h" //Arreglar

struct SDL_Texture;
struct _Mix_Music;

class ModuleSceneHonda : public Module
{
public:
	ModuleSceneHonda();
	~ModuleSceneHonda();

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
	SDL_Rect wall, wall2;
	Animation water;
	Collider *collider = nullptr;
	Collider *collider2 = nullptr;
};

#endif // __MODULESCENEHONDA_H__