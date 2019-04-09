#ifndef __MODULESCENEKEN_H__
#define __MODULESCENEKEN_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "SDL_Mixer/include/SDL_mixer.h" //Arreglar

struct SDL_Texture;
struct _Mix_Music;

class ModuleSceneKen : public Module
{
public:
	ModuleSceneKen();
	~ModuleSceneKen();

	bool Start();
	update_status Update();
	bool CleanUp();

public:
	
	SDL_Texture* graphics = nullptr;
	Mix_Music *music = nullptr;
	SDL_Rect ground;
	SDL_Rect ground2;
	SDL_Rect foreground;
	SDL_Rect background;
	SDL_Rect wall;
	SDL_Rect wall2;
	SDL_Rect box;
	SDL_Rect little;
	SDL_Rect big;
	Animation flag;
	Animation girl;
	Animation prGuy;
	Animation baldGuy;
	Animation blueGuy;
	Animation greyHat;
	Animation brownHat;
	Animation blueGuy2;
	Collider *collider = nullptr;
	Collider *collider2 = nullptr;
	

	float foreground_pos;
	bool forward;
};

#endif // __MODULESCENEKEN_H__