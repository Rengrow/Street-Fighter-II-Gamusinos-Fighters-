#ifndef __MODULEWELCOMEPAGE_H__
#define __MODULEWELCOMEPAGE_H__

#include "Module.h"
#include "Animation.h"
#include "Globals.h"
#include "p2Point.h"
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
private:
	void PrintInsertCoin();

public:
	SDL_Rect screen;
	SDL_Texture* intro = nullptr;
	SDL_Texture* graphics1 = nullptr;
	SDL_Texture* graphics2 = nullptr;
	Mix_Music* music = nullptr;
	Mix_Chunk* start = nullptr;
	Mix_Chunk* fist = nullptr;
	SDL_Rect logo;

	bool endedIntro;
	iPoint cameraPosition;
	iPoint p1Position;
	iPoint p2Position;
	SDL_Rect building;
	SDL_Rect sky1;
	SDL_Rect sky2;
	Animation people;
	SDL_Rect p11;
	SDL_Rect p12;
	Animation p1Face;
	SDL_Rect p21;
	SDL_Rect p22;
	SDL_Rect p23;	
	SDL_Rect pHit;
	bool p1PostionUp;
	bool p2PostionUp;
	int alpha;

	int fila;
	int columna;
	bool segundaSheet;
	bool isFading;
	int typography;
	Uint32 timeLetters;
	Uint32 pageTimer;
	bool lettersOn;
};

#endif // __MODULEWELCOMEPAGE_H__